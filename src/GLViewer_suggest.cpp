#include "GLViewer_suggest.h"

GLViewer_suggest::GLViewer_suggest(QWidget *parent): QWidget(parent) {
    m_mainViewer = new QGLView(this);
    m_sugViewer0 = new QGLView(this);
    m_sugViewer1 = new QGLView(this);
    m_sugViewer2 = new QGLView(this);
    m_sugViewer0->set_viewer_id(0);
    m_sugViewer1->set_viewer_id(1);
    m_sugViewer2->set_viewer_id(2);
    QString css = "border: 20px solid #FF0000;";
    m_sugViewer0->setStyleSheet(css);
    // m_sugViewer0->setStyleSheet("border:5px solid #ff0000;");
    m_sugViewers.push_back(m_sugViewer0);
    m_sugViewers.push_back(m_sugViewer1);
    m_sugViewers.push_back(m_sugViewer2);
    mainLayout.addWidget(m_mainViewer, 0, 0, 2, 3);
    mainLayout.addWidget(m_sugViewer0, 2, 0, 1, 1);
    mainLayout.addWidget(m_sugViewer1, 2, 1, 1, 1);
    mainLayout.addWidget(m_sugViewer2, 2, 2, 1, 1);
    
    
    
    setLayout(&mainLayout);



}