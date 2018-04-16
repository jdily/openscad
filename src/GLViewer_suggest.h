#pragma once
#include <QtGlobal>
#include <QWidget>
#include <vector>
#include <QGridLayout>
#include <QSize>
#include <QResizeEvent>
// #ifdef USE_QOPENGLWIDGET
// #include <QOpenGLWidget>
// #else
// #include <QGLWidget>
// #endif
#include <QLabel>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "QGLView.h"
#include "renderer.h"

class GLViewer_suggest : public QWidget {
public:
    GLViewer_suggest(QWidget *parent = nullptr);
    QGridLayout mainLayout;
    QGLView *m_sugViewer0;
    QGLView *m_sugViewer1;
    QGLView *m_sugViewer2;
    QGLView *m_mainViewer;
private:
    
};