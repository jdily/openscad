#include "GLViewer_group.h"

GLViewer_group::GLViewer_group(QWidget *parent): QWidget(parent) {
    mainLayout.addWidget(m_perpViewer, 0, 0, nullptr);
    mainLayout.addWidget(m_topViewer, 0, 1, nullptr);
    mainLayout.addWidget(m_sideViewer, 1, 0, nullptr);
    mainLayout.addWidget(m_frontViewer, 1, 1, nullptr);
    setLayout(mainLayout)
}