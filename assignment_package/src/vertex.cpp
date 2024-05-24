#include "halfedge.h"

int Vertex::idLast = 1;

Vertex::Vertex(glm::vec3 pos) :pos(pos), halfEdge(), id(idLast++) {

    QListWidgetItem::setText("Vertex " + QString::number(id));
}
