#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"
#include "halfedge.h"
#include "display.h"

#include <QListWidgetItem>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QTimer>


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    QTimer timer;
    float currTime;

    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_camera;
    // A variable used to track the mouse's previous position when
    // clicking and dragging on the GL viewport. Used to move the camera
    // in the scene.
    glm::vec2 m_mousePosPrev;
    bool meshLoaded = false;


public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();
    VertexDisplay m_vertDisplay;
    EdgeDisplay m_edgeDisplay;
    FaceDisplay m_faceDisplay;
    Mesh m_geomMesh;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void generateMesh(QString filename);
    void createList();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

signals:
    void sig_sendVertex(QListWidgetItem*);
    void sig_sendHalfEdge(QListWidgetItem*);
    void sig_sendFace(QListWidgetItem*);

public slots:
    void tick();
};


#endif // MYGL_H
