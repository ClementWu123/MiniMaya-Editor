#pragma once
#include <la.h>
#include "halfedge.h"
#include "drawable.h"


class VertexDisplay: public Drawable {
public:
    Vertex *representedVertex;
    VertexDisplay(OpenGLContext* context);
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void initializeAndBufferGeometryData() override;
    // Change which Vertex representedVertex points to
    void updateVertex(Vertex*);

    GLenum drawMode() override;
};

class FaceDisplay: public Drawable {

public:
    Face *representedFace;
    FaceDisplay(OpenGLContext* context);
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void initializeAndBufferGeometryData() override;
    // Change which Vertex representedVertex points to
    void updateFace(Face*);

    GLenum drawMode() override;

};

class EdgeDisplay: public Drawable {

public:
    HalfEdge *representedEdge;
    EdgeDisplay(OpenGLContext* context);
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void initializeAndBufferGeometryData() override;
    // Change which Vertex representedVertex points to
    void updateEdge(HalfEdge*);

    GLenum drawMode() override;

};
