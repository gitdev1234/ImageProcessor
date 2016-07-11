#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QImageReader>
// ----
#include "imageprocessor.h"
#include "Types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_actionMaximize_triggered();

    void on_actionClose_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionZoom_Fit_triggered();

    void on_actionLoad_from_modified_triggered();

    void on_actionLoad_from_original_triggered();

    void on_actionSwap_triggered();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_2_sliderReleased();

    void on_horizontalSlider_3_sliderReleased();

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;

    bool ImagesStretchedToLabel;
    ImageProcessor originalImage;
    ImageProcessor modifiedImage;
    ImageProcessor bufferImage;
    QLabel bufferLabel;

};

#endif // MAINWINDOW_H
