#pragma once

#include "system-gl.h"
#include <QtGlobal>

#ifdef USE_QOPENGLWIDGET
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif
#include <QLabel>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "GLView.h"
#include "renderer.h"
#include <QPainter>
#include <QGraphicsEllipseItem>

class QGLView :
#ifdef USE_QOPENGLWIDGET
		public QOpenGLWidget,
#else
		public QGLWidget,
#endif
		public GLView
{
	Q_OBJECT
	Q_PROPERTY(bool showFaces READ showFaces WRITE setShowFaces);
	Q_PROPERTY(bool showEdges READ showEdges WRITE setShowEdges);
	Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes);
	Q_PROPERTY(bool showCrosshairs READ showCrosshairs WRITE setShowCrosshairs);
	Q_PROPERTY(bool orthoMode READ orthoMode WRITE setOrthoMode);
	Q_PROPERTY(double showScaleProportional READ showScaleProportional WRITE setShowScaleProportional);

public:
	QGLView(QWidget *parent = nullptr);
#ifdef ENABLE_OPENCSG
	bool hasOpenCSGSupport() { return this->opencsg_support; }
#endif
	// Properties
	bool showFaces() const { return this->showfaces; }
	void setShowFaces(bool enabled) { this->showfaces = enabled; }
	bool showEdges() const { return this->showedges; }
	void setShowEdges(bool enabled) { this->showedges = enabled; }
	bool showAxes() const { return this->showaxes; }
	void setShowAxes(bool enabled) { this->showaxes = enabled; }
	bool showCrosshairs() const { return this->showcrosshairs; }
	void setShowCrosshairs(bool enabled) { this->showcrosshairs = enabled; }
	bool orthoMode() const { return (this->cam.projection == Camera::ProjectionType::ORTHOGONAL); }
	void setOrthoMode(bool enabled);
	bool showScaleProportional() const { return this->showscale; }
	void setShowScaleProportional(bool enabled) { this->showscale = enabled; }
	std::string getRendererInfo() const override;
#if QT_VERSION >= 0x050100
	float getDPI() override { return this->devicePixelRatio(); }
#endif
	
	const QImage & grabFrame();
	bool save(const char *filename) override;
	void resetView();
	void viewAll();

	// ichao added -> for multiple suggest viewer cases
	void set_viewer_id(int id);
	int viewer_id;

public slots:
	void ZoomIn(void);
	void ZoomOut(void);
#ifdef USE_QOPENGLWIDGET
	inline void updateGL() { update(); }
#endif

public:
	QLabel *statusLabel;
#ifdef USE_QOPENGLWIDGET
	inline QImage grabFrameBuffer() { return grabFramebuffer(); }
#endif
private:
	void init();

	bool mouse_drag_active;
	QPoint last_mouse;
	QImage frame; // Used by grabFrame() and save()

	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

	QPainter *painter;
	QPointF last_point;
	bool stroking;
	QPen stroke_pen;
	QPainterPath stroke_path;
	QPainterPathStroker path_stroker;
	QList<QPolygonF> stroke_poly;
	void get_stroke_poly();
	QList<QGraphicsEllipseItem*> test_circles;
	QList<int> covered_circ_ids;
	int cur_width = 0;
	int cur_height = 0;
	bool circ_drawn;

	void gen_random_circles();
	void check_covered();

	void initializeGL() override;
	void resizeGL(int w, int h) override;

	void paintGL() override;
	void normalizeAngle(GLdouble& angle);

#ifdef ENABLE_OPENCSG
	void display_opencsg_warning() override;
private slots:
	void display_opencsg_warning_dialog();
#endif

signals:
	void doAnimateUpdate();
	void exampleSelected(int exp_id);
	void strokeUpdate(QList<QPolygonF> polys);
};
