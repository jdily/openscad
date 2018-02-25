#pragma once
#include <QtGlobal>
#include <QWidget>
#include <vector>
#include <QGridLayout>

#ifdef USE_QOPENGLWIDGET
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif
#include <QLabel>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "QGLView.h"
#include "renderer.h"

class GLViewer_group : public QWidget {
    Q_OBJECT
    public:
        GLViewer_group(QWidget *parent = nullptr);
    private:
        QGridLayout mainLayout;
        QGLView *m_perpViewer;
        QGLView *m_topViewer;
        QGLView *m_sideViewer;
        QGLView *m_frontViewer;
};