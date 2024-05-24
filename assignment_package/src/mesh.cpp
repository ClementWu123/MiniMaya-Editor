#include "mesh.h"
#include <QFile>
#include <iostream>

Mesh::Mesh(OpenGLContext* context) : Drawable(context), vertices(), faces(), halfEdges() {}

// function to find centroids
std::map<Face*, glm::vec3> Mesh::findCentroid() {
    std::map<Face*, glm::vec3> cents;

    for (const uPtr<Face> &face : faces) {
        HalfEdge* start = face->halfEdge;
        HalfEdge* curr = start;
        int count = 0;
        glm::vec3 cent = glm::vec3(0.);

        do {
            cent += curr->vert->pos;
            count++;
            curr = curr -> next;
        } while (curr != start);

        cent /= count;
        cents[face.get()] = cent;
    }
    return cents;
}

// function to quadrangulate
void Mesh::quadrangulate(Face* face, std::map<Face*, glm::vec3> centroids) {
    HalfEdge *he = face->halfEdge;
    int sub_num = 0;
    do {
        sub_num++;
        he = he->next->next;
    } while (he != face->halfEdge);

    he = face->halfEdge->next;
    uPtr<Vertex> centroid = mkU<Vertex>(centroids.at(face));

    HalfEdge* prev = nullptr;
    HalfEdge* start = nullptr;
    for (int i = 0; i < sub_num; i ++) {
        uPtr<HalfEdge> towardCent = mkU<HalfEdge>();
        uPtr<HalfEdge> fromCent = mkU<HalfEdge>();

        uPtr<Face> newFace = mkU<Face>();
        towardCent.get()->face = newFace.get();
        fromCent.get()->face = newFace.get();
        he->face = newFace.get();
        he->next->face = newFace.get();

        newFace->halfEdge = he;

        if (start == nullptr) {
            start = fromCent.get();
        }

        towardCent->next = fromCent.get();
        fromCent->next = he;

        if (prev != nullptr) {
            fromCent.get()->sym(prev);
        }

        towardCent.get()->setVertex(centroid.get());
        fromCent.get()->setVertex(he->symPtr->vert);

        HalfEdge* currNext = he -> next;
        he = he->next->next;
        currNext->next = towardCent.get();
        prev = towardCent.get();
        faces.push_back(std::move(newFace));

        halfEdges.push_back(std::move(towardCent));
        halfEdges.push_back(std::move(fromCent));
    }
    prev->sym(start);
    vertices.push_back(std::move(centroid));
}

// fucntion to compute smoothed midpoint of each edge
uPtr<Vertex> Mesh::computeMid(HalfEdge* he, glm::vec3 face1,  glm::vec3 face2) {
    uPtr<HalfEdge> new_he = mkU<HalfEdge>();
    HalfEdge* old_sym = he->symPtr;
    uPtr<HalfEdge> new_sym = mkU<HalfEdge>();

    // set new pointers
    new_he->sym(old_sym);
    new_sym->sym(he);
    new_he->face = he->face;
    new_sym->face = old_sym->face;
    new_he->next = he->next;
    new_sym->next = old_sym->next;

    // update old ones
    he->next = new_he.get();
    old_sym->next = new_sym.get();
    Vertex* v1 = he->findPreVert();
    Vertex* v2 = he->vert;

    uPtr<Vertex> mid = mkU<Vertex>(glm::vec3((v1->pos + v2->pos + face1 + face2) / 4.f));

    new_sym->setVertex(old_sym->vert);
    new_he->setVertex(he->vert);
    he->setVertex(mid.get());
    old_sym->setVertex(mid.get());

    halfEdges.push_back(std::move(new_he));
    halfEdges.push_back(std::move(new_sym));

    return mid;
}

void Mesh::subdivision() {
    Face::idLast = 1;
    int f_n = faces.size();
    int v_n = vertices.size();
    int h_n = halfEdges.size();

    // find face's centroid
    std::map<Face*, glm::vec3> cents = findCentroid();

    // compute smoothed midpoint of each edge
    std::unordered_set<int> split;

    for (int i = 0; i < h_n; i ++) {
        HalfEdge* he = halfEdges[i].get();
        if (split.count(he->id) == 0) {
            split.insert(he->symPtr->id);
            glm::vec3 face1 = cents.at(he->face);
            glm::vec3 face2 = cents.at(he->symPtr->face);

            uPtr<Vertex> mid = computeMid(he, face1, face2);

            vertices.push_back(std::move(mid));
        }
    }

    // smooth original vertices
    std::cout << "Subdivision Created!" <<std::endl;
    for (int i = 0; i < v_n; i++)
    {
        glm::vec3 edge = glm::vec3(0,0,0);
        glm::vec3 face = glm::vec3(0,0,0);
        float n = 0.f;

        HalfEdge* start = vertices[i]->halfEdge;
        HalfEdge* curr = start;

        do {
            edge += curr->symPtr->vert->pos;
            face += cents[curr->face];
            n += 1.f;
            curr = curr->next->symPtr;
        }while(curr != start);

        vertices[i]->pos = (vertices[i]->pos * ((n - 2) / n)) + (edge * (1.f / (n * n))) + (face * (1.f / (n * n)));
    }

    // for each original face, split into N quadrangle faces
    for (int i = 0; i < f_n; i++) {
        quadrangulate(faces[0].get(), cents);
        faces.erase(faces.begin());
    }
    cents.clear();
}

void Mesh::loadObj(QString filename) {
    vertices.clear();
    faces.clear();
    halfEdges.clear();

    Vertex::idLast = 1;
    HalfEdge::idLast = 1;
    Face::idLast = 1;

    QFile file = QFile(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    std::map<std::pair<Vertex*, Vertex*>, HalfEdge*> symPtrs;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList chunk = line.split(" ");

        // load vertices
        if (line.startsWith("v ")) {
           glm::vec3 pos = glm::vec3(chunk.at(1).toFloat(), chunk.at(2).toFloat(), chunk.at(3).toFloat());
           uPtr<Vertex> v = mkU<Vertex>(pos);
           vertices.push_back(std::move(v));
        }

        Vertex* pre_vert;

        // load faces
        if (line.startsWith("f ")) {
            uPtr<Face> f = mkU<Face>();
            HalfEdge *first_he = nullptr;
            uPtr<HalfEdge> he;

            Vertex *first_vert = nullptr;

            // create half-edges
            for (int i = 1; i < chunk.size(); i ++) {
                QStringList num = chunk.at(i).split("/");
                Vertex *v = vertices.at(num.at(0).toInt() - 1).get();
                he = mkU<HalfEdge>();

                he->setVertex(v);
                he->face = f.get();

                if (i == 1) {
                    first_he = he.get();
                    pre_vert = v;
                    first_vert = v;
                }

                if (i != 1) {
                    halfEdges.back().get()->next = he.get();
                    symPtrs.insert(std::pair(std::pair(he->vert, pre_vert), he.get()));
                    symPtrs.insert(std::pair(std::pair(pre_vert, he->vert), he.get()));
                    pre_vert = he->vert;
                }

                if (i == chunk.size() - 1) {
                    symPtrs.insert(std::pair(std::pair(first_vert, pre_vert), first_he));
                    symPtrs.insert(std::pair(std::pair(pre_vert, first_vert), first_he));
                }

                halfEdges.push_back(std::move(he));
            }
            halfEdges.back().get()->next = first_he;


            f->halfEdge = halfEdges.back().get();
            faces.push_back(std::move(f));
        }
    }

    // prepare sym pointers
    for (uPtr<HalfEdge> & he: halfEdges) {
        HalfEdge *next_he = he->next;

        while (next_he->next != he.get()) {
            next_he = next_he -> next;
        }
        Vertex *pre_vert = next_he->vert;
        Vertex *curr_vert = he->vert;

        std::pair<Vertex*, Vertex*> v = std::pair(pre_vert, curr_vert);

        if (symPtrs.find(v) != symPtrs.end()) {
            HalfEdge *sym = symPtrs.at(v);

            if (sym->id != he->id) {
                he->sym(sym);
            }
        }
    }

    initializeAndBufferGeometryData();
}

void Mesh::initializeAndBufferGeometryData() {

    std::vector<glm::vec3> pos, nor, color;
    std::vector<GLuint> idx;

    int total = 0;

    for (const uPtr<Face> &f : faces) {
        int vertsOnFace = 0;
        HalfEdge* he = f->halfEdge;
        do {
            pos.push_back(glm::vec3(he->vert->pos));
            color.push_back(he->face->color);
            glm::vec3 one = he->vert->pos - he->symPtr->vert->pos;
            glm::vec3 two = he->next->vert->pos-he->vert->pos;

            if (glm::length(glm::cross(one,two)) != 0.f) {
                nor.push_back(glm::normalize(glm::cross(one,two)));
            }

            vertsOnFace++;
            he = he->next;
        } while (he != f->halfEdge);

        for (int i = 0; i < vertsOnFace - 2; i++) {
            idx.push_back(0 + total);
            idx.push_back(i + 1 + total);
            idx.push_back(i + 2 + total);
        }
        total += vertsOnFace;
    }

    this->indexBufferLength = idx.size();

    // OpenGL Stuff


    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    // Setup for Position Buffer
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    generateBuffer(NORMAL);
    bindBuffer(NORMAL);
    bufferData(NORMAL, nor);

    // Setup for Color Buffer
    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, color);

}

GLenum  Mesh::drawMode() {
    return GL_TRIANGLES;
}
