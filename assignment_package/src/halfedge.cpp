#include "halfedge.h"

int HalfEdge::idLast = 1;

HalfEdge::HalfEdge() : symPtr(), next(), face(), vert(), id(idLast++) {
    QListWidgetItem::setText("Edge " + QString::number(id));
}

void HalfEdge::setVertex(Vertex *v) {
    this->vert = v;
    v->halfEdge = this;
}

Vertex* HalfEdge::findPreVert() {

    HalfEdge* lastEdge = this->next;

    do {

        if (lastEdge->next == this) {
            return lastEdge->vert;
        } else {
            lastEdge = lastEdge->next;
        }

    } while (lastEdge != this);
    return lastEdge->vert;
}

int HalfEdge::findFaceVerts() {
    int vert_num = 1;
    HalfEdge* last = this->next;

    do {

        if (last->next == this) {
            return vert_num + 1;
        } else {
            vert_num +=1;
            last = last->next;
        }

    } while (last != this);
    return -1;
}

void HalfEdge::sym(HalfEdge *he) {
    this->symPtr = he;
    he->symPtr = this;
}
