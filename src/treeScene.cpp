#include "treeScene.h"

treeScene::treeScene(QObject* parent) : QGraphicsScene(parent) {
    m_selectRect = new QGraphicsRectItem(0,0,1,1);
    m_selectRect->setBrush(Qt::magenta);
    m_selectRect->setOpacity(0.2);
}
treeScene::~treeScene() {
    
}


void treeScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {   
}

void treeScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {   
}

void treeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {   
}