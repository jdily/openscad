#include "GLViewer_suggest.h"

GLViewer_suggest::GLViewer_suggest(QWidget *parent): QWidget(parent) {
    m_perpViewer = new QGLView(this);
    m_sugViewer0 = new QGLView(this);
    m_sugViewer1 = new QGLView(this);
    m_sugViewer2 = new QGLView(this);
    mainLayout.addWidget(m_mainViewer, 0, 0, 1, 3);
    mainLayout.addWidget(m_sugViewer0, 1, 0, 1, 1);
    mainLayout.addWidget(m_sugViewer0, 1, 1, 1, 1);
    mainLayout.addWidget(m_sugViewer0, 1, 2, 1, 1);
    setLayout(&mainLayout);
}