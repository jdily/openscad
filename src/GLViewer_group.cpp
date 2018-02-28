#include "GLViewer_group.h"

GLViewer_group::GLViewer_group(QWidget *parent): QWidget(parent) {
    m_perpViewer = new QGLView(this);
    m_topViewer = new QGLView(this);
    m_sideViewer = new QGLView(this);
    m_frontViewer = new QGLView(this);
    mainLayout.addWidget(m_perpViewer, 0, 0, nullptr);
    mainLayout.addWidget(m_topViewer, 0, 1, nullptr);
    mainLayout.addWidget(m_sideViewer, 1, 0, nullptr);
    mainLayout.addWidget(m_frontViewer, 1, 1, nullptr);
    setLayout(&mainLayout);

    // set default view of camera for different viewer
    m_topViewer->cam.object_rot << 90,0,0;
    m_topViewer->setOrthoMode(true);
    m_topViewer->updateGL();
    m_frontViewer->cam.object_rot << 0,0,0;
    m_frontViewer->setOrthoMode(true);
    m_frontViewer->updateGL();
    m_sideViewer->cam.object_rot << 0,0,90;
    m_sideViewer->setOrthoMode(true);
    m_sideViewer->updateGL();

    m_topViewer->toggleDrag();
    m_topViewer->displayText(QString("top"));
    m_frontViewer->toggleDrag();
    m_sideViewer->toggleDrag();
}