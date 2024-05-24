#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_loadObj_clicked();
    void slot_updateVertex(QListWidgetItem*);
    void slot_updateFace(QListWidgetItem*);
    void slot_updateHalfEdge(QListWidgetItem*);
    void on_vertsListWidget_itemClicked(QListWidgetItem *item);
    void on_halfEdgesListWidget_itemClicked(QListWidgetItem *item);
    void on_facesListWidget_itemClicked(QListWidgetItem *item);
    void on_vertPosXSpinBox_valueChanged(double value);
    void on_vertPosZSpinBox_valueChanged(double value);
    void on_vertPosYSpinBox_valueChanged(double value);
    void on_faceRedSpinBox_valueChanged(double value);
    void on_faceGreenSpinBox_valueChanged(double value);
    void on_faceBlueSpinBox_valueChanged(double value);
    void on_subButton_clicked();
    void on_vertButton_clicked();
    void on_triButton_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
