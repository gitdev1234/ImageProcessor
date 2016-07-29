#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QImageReader>
#include <QColor>
#include <QPainter>
// ----
#include "Image.h"
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

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_listWidget_3_itemClicked(QListWidgetItem *item);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_progressBar_valueChanged(int value);

    void on_toolButton_4_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_comboBox_4_currentIndexChanged(const QString &arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;

    bool ImagesStretchedToLabel;
    Image originalImage;
    Image modifiedImage;
    Image bufferImage;
    Image resultImage;
    QLabel bufferLabel;

    // enabling/disabling of RGB-channels
    signedRGBDelta rgbDeltasBeforeDisabling;

};

#endif // MAINWINDOW_H
