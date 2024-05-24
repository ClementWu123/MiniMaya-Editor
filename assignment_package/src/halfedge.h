#pragma once
#include <la.h>
#include <glm/gtc/random.hpp>
#include <QListWidgetItem>
#include "utils.h"

class HalfEdge;

class Face: public QListWidgetItem{
public:
    Face();
    HalfEdge* halfEdge; // pointer to half-edge
    glm::vec3 color;
    int id; // unique int
    static int idLast; // tracks the last ID
};

class Vertex: public QListWidgetItem{
public:
    Vertex(glm::vec3 pos);

    glm::vec3 pos;
    HalfEdge* halfEdge; // pointer to half-edge
    int id; // unique int
    static int idLast; // tracks the last ID
};

class HalfEdge: public QListWidgetItem {
public:
    HalfEdge* symPtr;
    HalfEdge* next; //pointer to the next HalfEdge
    Face* face; // pointer to the Face
    Vertex* vert; //pointer to the Vertex between this HalfEdge and its next HalfEdge
    int id; // unique int
    static int idLast; // tracks the last ID

    HalfEdge();
    void setVertex(Vertex *v);
    Vertex* findPreVert();
    int findFaceVerts();
    void sym(HalfEdge *he);
};
