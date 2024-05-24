#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl, SIGNAL(sig_sendVertex(QListWidgetItem*)), this, SLOT(slot_updateVertex(QListWidgetItem*)));

    connect(ui->mygl, SIGNAL(sig_sendFace(QListWidgetItem*)), this, SLOT(slot_updateFace(QListWidgetItem*)));

    connect(ui->mygl, SIGNAL(sig_sendHalfEdge(QListWidgetItem*)), this, SLOT(slot_updateHalfEdge(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_loadObj_clicked()
{
    QString filename = QFileDialog::getOpenFileName(0, QString(""), QDir::currentPath().append(QString("../..")), QString("*.obj"));

    if ( !filename.isNull() ) {
        qDebug() << "selected file path :" << filename.toUtf8();
        ui->mygl->generateMesh(filename);
    }
}

void MainWindow::slot_updateVertex(QListWidgetItem *item) {
    ui->vertsListWidget->addItem(item);
}

void MainWindow::slot_updateHalfEdge(QListWidgetItem *item) {
    ui->halfEdgesListWidget->addItem(item);
}

void MainWindow::slot_updateFace(QListWidgetItem *item) {
    ui->facesListWidget->addItem(item);
}

void MainWindow::on_vertsListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_vertDisplay.updateVertex((Vertex*) item);
    ui->mygl->m_vertDisplay.initializeAndBufferGeometryData();
    ui->mygl->update();
}


void MainWindow::on_halfEdgesListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_edgeDisplay.updateEdge((HalfEdge*) item);
    ui->mygl->m_edgeDisplay.initializeAndBufferGeometryData();
    ui->mygl->update();
}


void MainWindow::on_facesListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_faceDisplay.updateFace((Face*) item);
    ui->mygl->m_faceDisplay.initializeAndBufferGeometryData();
    ui->mygl->update();
}

void MainWindow::on_vertPosXSpinBox_valueChanged(double value) {
    if (ui->mygl->m_vertDisplay.representedVertex != nullptr) {
        ui->mygl->m_vertDisplay.representedVertex->pos[0] = value;
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_vertDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}

void MainWindow::on_vertPosYSpinBox_valueChanged(double value) {
    if (ui->mygl->m_vertDisplay.representedVertex != nullptr) {
        ui->mygl->m_vertDisplay.representedVertex->pos[1] = value;
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_vertDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}

void MainWindow::on_vertPosZSpinBox_valueChanged(double value) {
    if (ui->mygl->m_vertDisplay.representedVertex != nullptr) {
        ui->mygl->m_vertDisplay.representedVertex->pos[2] = value;
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_vertDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}

void MainWindow::on_faceRedSpinBox_valueChanged(double value) {
    if (ui->mygl->m_faceDisplay.representedFace != nullptr) {
        ui->mygl->m_faceDisplay.representedFace->color.r = value;
        ui-> mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_faceDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}


void MainWindow::on_faceGreenSpinBox_valueChanged(double value) {
    if (ui->mygl->m_faceDisplay.representedFace != nullptr) {
        ui->mygl->m_faceDisplay.representedFace->color.g = value;
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_faceDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}

void MainWindow::on_faceBlueSpinBox_valueChanged(double value) {
    if (ui->mygl->m_faceDisplay.representedFace != nullptr) {
        ui->mygl->m_faceDisplay.representedFace->color.b = value;
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->m_faceDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}

void MainWindow::on_subButton_clicked() {
    ui->mygl->m_geomMesh.subdivision();
    ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
    ui->mygl->createList();
    ui->mygl->update();
}

void MainWindow::on_vertButton_clicked() {
    if (ui->mygl->m_edgeDisplay.representedEdge != nullptr) {
        HalfEdge* old_he = ui->mygl->m_edgeDisplay.representedEdge;
        uPtr<HalfEdge> new_he = mkU<HalfEdge>();

        HalfEdge* old_sym = ui->mygl->m_edgeDisplay.representedEdge->symPtr;
        uPtr<HalfEdge> new_sym = mkU<HalfEdge>();

        // set new pointers
        new_he->sym(old_sym);
        new_sym->sym(old_he);
        new_he->face = old_he->face;
        new_sym->face = old_sym->face;
        new_he->next = old_he->next;
        new_sym->next = old_sym->next;
        new_sym->setVertex(old_sym->vert);
        new_he->setVertex(old_he->vert);

        // update old ones
        old_he->next = new_he.get();
        old_sym->next = new_sym.get();

        Vertex* v1 = old_he->findPreVert();
        Vertex* v2 = new_he->vert;

        uPtr<Vertex> mid = mkU<Vertex>(
            glm::vec3((v1->pos[0] + v2->pos[0])/2,
                      (v1->pos[1] + v2->pos[1])/2,
                      (v1->pos[2] + v2->pos[2])/2));

        old_sym->setVertex(mid.get());
        old_he->setVertex(mid.get());

        ui->mygl->m_geomMesh.halfEdges.push_back(std::move(new_he));
        ui->mygl->m_geomMesh.halfEdges.push_back(std::move(new_sym));
        ui->mygl->m_geomMesh.vertices.push_back(std::move(mid));
        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->createList();
        ui->mygl->m_edgeDisplay.updateEdge(old_he);
        ui->mygl->m_edgeDisplay.initializeAndBufferGeometryData();
        ui->mygl->update();
    }
}


void MainWindow::on_triButton_clicked() {
    if (ui->mygl->m_faceDisplay.representedFace != nullptr) {
        HalfEdge* he = ui->mygl->m_faceDisplay.representedFace->halfEdge;
        int n = he->findFaceVerts();
        for (int i = 0; i < n - 2; i ++) {
            uPtr<HalfEdge> he1 = mkU<HalfEdge>();
            uPtr<HalfEdge> he2 = mkU<HalfEdge>();
            he1->setVertex(he->vert);
            he2->setVertex(he->next->next->vert);
            he1->sym(he2.get());
            uPtr<Face> face = mkU<Face>();
            face->halfEdge = he1.get();
            he1->face = face.get();
            he->next->face = face.get();
            he->next->next->face = face.get();
            he2->face = ui->mygl->m_faceDisplay.representedFace;
            he2->next = he->next->next->next;
            he->next->next->next = he1.get();
            he1->next = he->next;
            he->next = he2.get();

            ui->mygl->m_geomMesh.halfEdges.push_back(std::move(he1));
            ui->mygl->m_geomMesh.halfEdges.push_back(std::move(he2));
            ui->mygl->m_geomMesh.faces.push_back(std::move(face));
        }

        ui->mygl->m_geomMesh.initializeAndBufferGeometryData();
        ui->mygl->createList();
        ui->mygl->update();
    }
}

