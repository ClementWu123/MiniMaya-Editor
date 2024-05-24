#include "halfedge.h"

int Face::idLast = 1;

Face::Face() : halfEdge(),
               color(glm::vec3(glm::linearRand(0.f, 1.f),
                     glm::linearRand(0.f, 1.f),
                     glm::linearRand(0.f, 1.f))), id(idLast++) {
    QListWidgetItem::setText("Face " + QString::number(id));
}
