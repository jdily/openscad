/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "qtgettext.h"
#include "QGLView.h"
#include "Preferences.h"
#include "renderer.h"

#include <QApplication>
#include <QWheelEvent>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QErrorMessage>
#include "OpenCSGWarningDialog.h"
#include "QSettingsCached.h"


#include <stdio.h>
#include <sstream>

#ifdef ENABLE_OPENCSG
#  include <opencsg.h>
#endif

QGLView::QGLView(QWidget *parent) :
#ifdef USE_QOPENGLWIDGET
	QOpenGLWidget(parent)
#else
	QGLWidget(parent)
#endif
{
  init();
}

#if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
static bool running_under_wine = false;
#endif

void QGLView::init()
{
  resetView();

  this->mouse_drag_active = false;
  this->statusLabel = nullptr;

  setMouseTracking(true);

  viewer_id = -1;


#if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
// see paintGL() + issue160 + wine FAQ
#include <windows.h>
  HMODULE hntdll = GetModuleHandle(L"ntdll.dll");
  if (hntdll)
    if ( (void *)GetProcAddress(hntdll, "wine_get_version") )
      running_under_wine = true;
#endif
  stroking = false;
  srand(time(NULL));
  // gen_random_circles();
  circ_drawn = false;
  right_menu = new QMenu(this);
  act_group_geom = new QAction(tr("&Group selected geometry"), this);
  act_trans = new QAction(tr("&Transfer to current"), this);
  right_menu->addAction(act_group_geom);
  right_menu->addAction(act_trans);
  
}

void QGLView::resetView()
{
	cam.resetView();
}

void QGLView::set_viewer_id(int id) {
  viewer_id = id;
}

void QGLView::viewAll()
{
	if (auto renderer = this->getRenderer()) {
		auto bbox = renderer->getBoundingBox();
		cam.object_trans = -bbox.center();
		cam.viewAll(renderer->getBoundingBox());
	}
}

void QGLView::initializeGL()
{
  auto err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
  }
  GLView::initializeGL();
}

std::string QGLView::getRendererInfo() const
{
  std::stringstream info;
  info << glew_dump();
  // Don't translate as translated text in the Library Info dialog is not wanted
#ifdef USE_QOPENGLWIDGET
  info << "\nQt graphics widget: QOpenGLWidget";
  auto qsf = this->format();
  auto rbits = qsf.redBufferSize();
  auto gbits = qsf.greenBufferSize();
  auto bbits = qsf.blueBufferSize();
  auto abits = qsf.alphaBufferSize();
  auto dbits = qsf.depthBufferSize();
  auto sbits = qsf.stencilBufferSize();
  info << boost::format("\nQSurfaceFormat: RGBA(%d%d%d%d), depth(%d), stencil(%d)\n\n") %
    rbits % gbits % bbits % abits % dbits % sbits;
#else
  info << "\nQt graphics widget: QGLWidget";
#endif
  info << glew_extensions_dump();
  return info.str();
}
#ifdef ENABLE_OPENCSG
void QGLView::display_opencsg_warning()
{
	if (Preferences::inst()->getValue("advanced/opencsg_show_warning").toBool()) {
		QTimer::singleShot(0, this, SLOT(display_opencsg_warning_dialog()));
	}
}

void QGLView::display_opencsg_warning_dialog()
{
	auto dialog = new OpenCSGWarningDialog(this);

  QString message;
  if (this->is_opencsg_capable) {
    message += _("Warning: You may experience OpenCSG rendering errors.\n\n");
  }
  else {
    message += _("Warning: Missing OpenGL capabilities for OpenCSG - OpenCSG has been disabled.\n\n");
    dialog->enableOpenCSGBox->hide();
  }
  message += _("It is highly recommended to use OpenSCAD on a system with "
							 "OpenGL 2.0 or later.\n"
							 "Your renderer information is as follows:\n");
  QString rendererinfo(_("GLEW version %1\n%2 (%3)\nOpenGL version %4\n"));
  message += rendererinfo.arg((const char *)glewGetString(GLEW_VERSION),
															(const char *)glGetString(GL_RENDERER),
															(const char *)glGetString(GL_VENDOR),
															(const char *)glGetString(GL_VERSION));

  dialog->setText(message);
  dialog->enableOpenCSGBox->setChecked(Preferences::inst()->getValue("advanced/enable_opencsg_opengl1x").toBool());
  dialog->exec();

  opencsg_support = this->is_opencsg_capable && Preferences::inst()->getValue("advanced/enable_opencsg_opengl1x").toBool();
}
#endif

void QGLView::gen_random_circles() {
  // get width and height
  // int w = QWidget::size().rwidth();
  // int h = QWidget::size().rheight();
  // std::cout << w << " " << h << std::endl;
  int count = 20;
  float x = 0.0, y = 0.0;
  // create random 
  for (int i = 0; i < count; i++) {
    x = rand() % cur_width; 
    y = rand() % cur_height;
    QGraphicsEllipseItem *circ = new QGraphicsEllipseItem(x, y, 5, 5);
    test_circles.append(circ);
  }

}


void QGLView::resizeGL(int w, int h)
{
  std::cout << "resize : " << w << " " << h << std::endl;
  cur_width = w;
  cur_height = h;
  GLView::resizeGL(w,h);
}

void QGLView::paintGL()
{
  painter = new QPainter(this);
  painter->setRenderHints(QPainter::Antialiasing);
  
  painter->beginNativePainting();
  glEnable(GL_DEPTH_TEST);
  GLView::paintGL();

  if (statusLabel) {
    Camera nc(cam);
    nc.gimbalDefaultTranslate();
		auto status = QString("%1 (%2x%3)")
			.arg(QString::fromStdString(nc.statusText()))
			.arg(size().rwidth())
			.arg(size().rheight());
    statusLabel->setText(status);
  }
  glDisable(GL_DEPTH_TEST);
  painter->endNativePainting();

  // pt_painter.setPen({QColor(0,255,100, 255), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
  // for (int i = 0; i < test_circles.length(); i++) {
  //   // test_circles[i]->paint(painter, this);
  //   painter->drawEllipse(test_circles[i]->rect());
  // }
  // pt_painter.end();
  // path_stroker.setWidth(20.0);
  // path_stroker.setCapStyle(Qt::RoundCap);
  // path_stroker.setJoinStyle(Qt::RoundJoin);
  // QPainterPath _path = path_stroker.createStroke(stroke_path);
  // // std::cout << "element count : " << _path.elementCount() << std::endl;
  // for (int i = 0; i < _path.elementCount()-1; i++) {
  //   QPainterPath::Element p1 = _path.elementAt(i);
  //   QPainterPath::Element p2 = _path.elementAt(i+1);
  //   // painter->setPen(QPen(i % 2 == 0 ? Qt::red : Qt::green, 1));
  //   painter->setPen({QColor(135,206,250, 128), 1.0}); 
  //   painter->drawLine(QPointF(p1.x, p1.y), QPointF(p2.x, p2.y));
  // }
  // for (int i = 0; i < stroke_poly.length(); i++) {
  //   painter->setPen({Qt::red, 2.0}); 
  //   painter->drawPolygon(stroke_poly[i]);
  // }

  // test draw projected sample points..
  // painter->setPen({QColor(0,255,100, 255), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
  // int count = 0;
  // float x = 0.0, y = 0.0, z = 0.0;
  // std::cout << "proj sample size : " << this->proj_sample_dict.size() << std::endl;
  // for (int i = 0; i < proj_sample_dict.keys().size(); i++) {
  //   int k = proj_sample_dict.keys()[i];
  // // for (const int& k : proj_sample_dict.keys()) {
  //   count = proj_sample_dict[k].size();
  //   // std::cout << "k : " << k << " " << count << std::endl;
  //   // std::cout << proj_sample_dict[k][0][0] << " " << proj_sample_dict[k][0][1] << std::endl;
  //   // std::cout << cur_width << " " << cur_height << std::endl;
  //   for (int i = 0; i < count; i++) {
  //     x = proj_sample_dict[k][i][0];
  //     y = proj_sample_dict[k][i][1];
  //     // z = proj_sample_dict[k][i][2];
  //     // x = rand() % cur_width; 
  //     // y = rand() % cur_height;
  //     // std::cout << x << " " << y << " " << z << std::endl;
  //     // QGraphicsEllipseItem *circ = new QGraphicsEllipseItem(x, y, 5, 5);
  //     painter->drawEllipse(x, y, 10, 10);
  //   }
  // }
  // painter->end();

  // draw strokes.
  painter->setPen({QColor(0,255,100, 255), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
  painter->setPen({QColor(135,206,250, 100), 20.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin}); 
  painter->drawPath(stroke_path); 
  painter->end(); 

#if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
  if (running_under_wine) swapBuffers();
#endif
}

QMap<int, std::vector<Eigen::Vector3d>> QGLView::project_samples_map(QMap<int, std::vector<Eigen::Vector3d>> sample_map) {
  // this->proj_sample_dict.clear();
  QMap<int, std::vector<Eigen::Vector3d>> out;
  for (auto &k : sample_map.keys()) {
    std::vector<Eigen::Vector3d> p_s = project_samples(sample_map[k], k);
    out.insert(k, p_s);
  }
  updateGL();
  return out;
}


std::vector<Eigen::Vector3d> QGLView::project_samples(std::vector<Eigen::Vector3d> samples, int index) {
  std::vector<Eigen::Vector3d> out;
  int count = (int)samples.size();
  std::cout << "[QGLView::project_samples] count -> " << count << std::endl;
  setupCamera();
	int viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
  
  viewport[2] = cur_width;
  viewport[3] = cur_height;
  std::cout << "index : " << index << std::endl;
  std::cout << "viewport things : " << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << std::endl;
  // GLdouble px, py, pz;
  GLdouble wx, wy, wz;
  for (int i = 0; i < count; i++) {
    auto success = gluProject(samples[i][0], samples[i][1], samples[i][2], modelview, projection, viewport, &wx, &wy, &wz);
    if (success) {
      // std::cout << wx << " " << cur_height-wy << " " << wz << std::endl;
      Vector3d v(wx, cur_height-wy, wz);
      // QGraphicsEllipseItem *circ = new QGraphicsEllipseItem(wx, cur_height-wy, 5, 5);
      // vert_lists.append(circ);
      out.push_back(v);
    }
  }
  std::cout << "[QGLView::project_samples] out count -> " << out.size() << std::endl;
  this->proj_sample_dict.insert(index, out);
  std::cout << "[QGLView::project_samples] proj_sample_dict count -> " << proj_sample_dict.size() << std::endl;
  // std::cout << "[QGLView::project_samples] vert list count -> " << vert_lists.length() << std::endl;
  return out;
}

void QGLView::mouseDoubleClickEvent(QMouseEvent *event) {

	setupCamera();

	int viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetIntegerv( GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	double x = event->pos().x() * this->getDPI();
	double y = viewport[3] - event->pos().y() * this->getDPI();
	GLfloat z = 0;

	glGetError(); // clear error state so we don't pick up previous errors
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	auto glError = glGetError();
	if (glError != GL_NO_ERROR) {
		return;
	}

	if (z == 1) return; // outside object

	GLdouble px, py, pz;

	auto success = gluUnProject(x, y, z, modelview, projection, viewport, &px, &py, &pz);

	if (success == GL_TRUE) {
		cam.object_trans -= Vector3d(px, py, pz);
		updateGL();
		emit doAnimateUpdate();
	}
}

void QGLView::normalizeAngle(GLdouble& angle)
{
  while(angle < 0) angle += 360;
  while(angle > 360) angle -= 360;
}

void QGLView::mousePressEvent(QMouseEvent *event)
{
  mouse_drag_active = true;
  last_mouse = event->globalPos();
  if(event->button() == Qt::RightButton) {
    if (event->modifiers() == Qt::ControlModifier) {
      QString mes = QString("right mouse button is pressed at viewer %1").arg(viewer_id);
      std::cout << mes.toStdString() << std::endl;
      stroking = true;
      stroke_path = QPainterPath(event->pos());
    } else if (event->modifiers() == Qt::ShiftModifier) {
      mouse_drag_active = false;
      // show up the context menu here for choosing actions after geometry selection.
      right_menu->exec(event->globalPos());
    }
  }
}

void QGLView::mouseMoveEvent(QMouseEvent *event)
{
  auto this_mouse = event->globalPos();
  double dx = (this_mouse.x() - last_mouse.x()) * 0.7;
  double dy = (this_mouse.y() - last_mouse.y()) * 0.7;
  if (mouse_drag_active) {
    // std::cout << "mouse drag active " << std::endl;
    if (event->buttons() & Qt::LeftButton
#ifdef Q_OS_MAC
        && !(event->modifiers() & Qt::MetaModifier)GLView::height
#endif
      ) {
      // Left button rotates in xz, Shift-left rotates in xy
      // On Mac, Ctrl-Left is handled as right button on other platforms
      cam.object_rot.x() += dy;
      if ((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        cam.object_rot.y() += dx;
			}
      else {
        cam.object_rot.z() += dx;
			}
      normalizeAngle(cam.object_rot.x());
      normalizeAngle(cam.object_rot.y());
      normalizeAngle(cam.object_rot.z());
    }
    else if (event->buttons() & Qt::RightButton) {
      if (event->modifiers() == Qt::ControlModifier && stroking) {
        // std::cout << "keep stroking " << std::endl;
        stroke_path.lineTo(event->pos());
      }
    }
    else {
      // Right button pans in the xz plane
      // Middle button pans in the xy plane
      // Shift-right and Shift-middle zooms
      if ((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
	      cam.zoom(-12.0 * dy);
      } else {

      double mx = +(dx) * 3.0 * cam.zoomValue() / QWidget::width();
      double mz = -(dy) * 3.0 * cam.zoomValue() / QWidget::height();

      double my = 0;
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
      if (event->buttons() & Qt::MidButton) {
#else
      if (event->buttons() & Qt::MiddleButton) {
#endif
        my = mz;
        mz = 0;
        // actually lock the x-position
        // (turns out to be easier to use than xy panning)
        mx = 0;
      }

      Matrix3d aax, aay, aaz;
      aax = Eigen::AngleAxisd(-(cam.object_rot.x()/180) * M_PI, Vector3d::UnitX());
      aay = Eigen::AngleAxisd(-(cam.object_rot.y()/180) * M_PI, Vector3d::UnitY());
      aaz = Eigen::AngleAxisd(-(cam.object_rot.z()/180) * M_PI, Vector3d::UnitZ());
      Matrix3d tm3 = Matrix3d::Identity();
      tm3 = aaz * (aay * (aax * tm3));

      Matrix4d tm = Matrix4d::Identity();
      for (int i=0;i<3;i++) for (int j=0;j<3;j++) tm(j,i) = tm3(j,i);

      Matrix4d vec;
      vec <<
        0,  0,  0,  mx,
        0,  0,  0,  my,
        0,  0,  0,  mz,
        0,  0,  0,  1
      ;
      tm = tm * vec;
      cam.object_trans.x() += tm(0,3);
      cam.object_trans.y() += tm(1,3);
      cam.object_trans.z() += tm(2,3);
      }
    }
    updateGL();
    emit doAnimateUpdate();
  } 
  last_mouse = this_mouse;
}

void QGLView::mouseReleaseEvent(QMouseEvent*)
{
  mouse_drag_active = false;
  painter = nullptr;
  // create the stroker for checking
  // TODO : check the covered test..
  // check_covered();
  if (stroking) {
    get_stroke_poly();
    emit strokeUpdate(stroke_poly, stroke_path);
    stroking = false;
  }
  releaseMouse();
}


void QGLView::get_stroke_poly() {
  QPainterPathStroker stroker;
  stroker.setWidth(20.0);
  stroker.setCapStyle(Qt::RoundCap);  
  stroker.setJoinStyle(Qt::RoundJoin);
  
  QPainterPath _path = stroker.createStroke(stroke_path);
  stroke_poly = _path.toFillPolygons();
  
  // std::cout << "poly count : " << stroke_poly.length() << std::endl;
}

void QGLView::check_covered() {
  for (int i = 0; i < test_circles.length(); i++) {
    const QPointF pt = test_circles[i]->rect().center();
    bool covered = stroke_poly[0].containsPoint(pt, Qt::WindingFill);
    if (covered) {
      covered_circ_ids.append(i);
    }
  }
  std::cout << covered_circ_ids.length() << " circles are covered." << std::endl;
  covered_circ_ids.clear();
}


const QImage & QGLView::grabFrame()
{
	// Force reading from front buffer. Some configurations will read from the back buffer here.
	glReadBuffer(GL_FRONT);
	this->frame = grabFrameBuffer();
	return this->frame;
}

bool QGLView::save(const char *filename)
{
  return this->frame.save(filename, "PNG");
}

void QGLView::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION >= 0x050000
	this->cam.zoom(event->angleDelta().y());
#else
	this->cam.zoom(event->delta());
#endif
  updateGL();
}

void QGLView::ZoomIn(void)
{
  this->cam.zoom(120);
  updateGL();
}

void QGLView::ZoomOut(void)
{
  this->cam.zoom(-120);
  updateGL();
}

void QGLView::setOrthoMode(bool enabled)
{
	if (enabled) this->cam.setProjection(Camera::ProjectionType::ORTHOGONAL);
	else this->cam.setProjection(Camera::ProjectionType::PERSPECTIVE);
}

void QGLView::clean_stroke() {
  stroke_path =  QPainterPath();
  updateGL();
}