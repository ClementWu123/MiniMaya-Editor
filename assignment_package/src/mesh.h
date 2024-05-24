#pragma once
#include "halfedge.h"
#include "drawable.h"
#include <la.h>
#include <vector>
#include "utils.h"
#include <unordered_set>

class Mesh: public Drawable {
public:
    std::vector<uPtr<Vertex>> vertices;
    std::vector<uPtr<Face>> faces;
    std::vector<uPtr<HalfEdge>> halfEdges;
    //std::vector<glm::vec3> normals;

    Mesh(OpenGLContext* context);

    void initializeAndBufferGeometryData() override;
    void loadObj(QString filename);
    std::map<Face*, glm::vec3> findCentroid();
    void quadrangulate(Face* face, std::map<Face*, glm::vec3> centroids);
    uPtr<Vertex> computeMid(HalfEdge* he, glm::vec3 face1,  glm::vec3 face2);
    void subdivision();

    GLenum drawMode() override;
};
