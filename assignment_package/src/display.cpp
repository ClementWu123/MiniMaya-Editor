#include "display.h"

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), representedVertex(nullptr) {}

void VertexDisplay::initializeAndBufferGeometryData() {
    std::vector<glm::vec3> pos, color;
    std::vector<GLuint> idx = {0};

    pos.push_back(glm::vec3(representedVertex->pos));
    color.push_back(glm::vec3(1.f, 1.f, 1.f));

    this->indexBufferLength = idx.size();

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    // Setup for Position Buffer
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // Setup for Color Buffer
    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, color);

}

void VertexDisplay::updateVertex(Vertex* v) {
    representedVertex = v;
}

GLenum VertexDisplay::drawMode() {
    return GL_POINTS;
}

//Face
FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr) {}

void FaceDisplay::initializeAndBufferGeometryData() {
    std::vector<glm::vec3> pos, color;
    std::vector<GLuint> idx;

    HalfEdge *half = representedFace->halfEdge;
    HalfEdge *current = half;

    int vert_num = 0;

    do {
        glm::vec3 posHE = glm::vec3(current->vert->pos);
        pos.push_back(posHE);
        current = current->next;
        vert_num++;

    } while (current != half);

    glm::vec3 faceColor = representedFace->color;
    glm::vec3 newColor = glm::vec3(1.f - faceColor.r, 1.f - faceColor.g, 1.f - faceColor.b);

    for (int i = 0; i < vert_num - 1; i ++) {
        idx.push_back(i);
        idx.push_back(i + 1);
    }

    idx.push_back(vert_num - 1);
    idx.push_back(0);

    for (size_t i = 0; i < pos.size(); i ++) {
        color.push_back(newColor);
    }

    this->indexBufferLength = idx.size();
    // other stuff

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    // Setup for Position Buffer
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // Setup for Color Buffer
    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, color);

}

void FaceDisplay::updateFace(Face* f) {
    representedFace = f;
}

GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

// Edges

EdgeDisplay::EdgeDisplay(OpenGLContext* context)
    :Drawable(context), representedEdge(nullptr)
{
}

void EdgeDisplay::initializeAndBufferGeometryData() {

    std::vector<glm::vec3> pos, color;

    std::vector<GLuint> idx = {0,1};

    glm::vec3 pos1 = glm::vec3(representedEdge->vert->pos);
    glm::vec3 pos2 = glm::vec3(representedEdge->symPtr->vert->pos);

    pos.push_back(pos1);
    pos.push_back(pos2);

    color.push_back(glm::vec3(1.f, 0.f, 0.f));
    color.push_back(glm::vec3(1.f, 1.f, 0.f));

    this->indexBufferLength = idx.size();

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    // Setup for Position Buffer
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // Setup for Color Buffer
    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, color);

}

void EdgeDisplay::updateEdge(HalfEdge* e) {
    representedEdge = e;
}

GLenum EdgeDisplay::drawMode() {
    return GL_LINES;
}
