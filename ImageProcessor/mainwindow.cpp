#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <QPixmap>
#include <iostream>
#include "Image.h"

using namespace std;

/* --- CONSTRUCTORS / DESTRUCTORS --- */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* --- initial gui - settings ---*/
    setWindowFlags(Qt::FramelessWindowHint); //needed for custom titleBar (deactivates normal statusbar)
    ui->tabWidget->tabBar()->hide();
    showMaximized();
    ui->toolButton->  setDefaultAction(ui->actionZoom_out);
    ui->toolButton_2->setDefaultAction(ui->actionZoom_in);
    ui->toolButton_3->setDefaultAction(ui->actionZoom_Fit);
    ui->toolButton_4->setDefaultAction(ui->actionLoad_from_original);
    ui->toolButton_5->setDefaultAction(ui->actionLoad_from_modified);
    ui->toolButton_6->setDefaultAction(ui->actionSwap);

    /* --- bind gui-elements to Image-objects --- */
    originalImage = Image(ui->label,ui->progressBar);
    modifiedImage = Image(ui->label_2,ui->progressBar);

    /* --- load initial images --- */
    QSize originalSize = originalImage.loadImage(*this,"Sample01.png");
    modifiedImage.loadImage(*this,"Sample01.png");

    /* --- output size at statusbar --- */
    stringstream sstr;
    sstr << "width : " << originalSize.width() << ", height : " << originalSize.height() << endl;
    ui->statusBar->showMessage(QString(sstr.str().c_str()));

    /* --- initially stretch images --- */
    originalImage.stretchImageToLabel(true);
    modifiedImage.stretchImageToLabel(true);
    ImagesStretchedToLabel = true;
    ui->toolButton->setEnabled(!ImagesStretchedToLabel);   // zoom-out button
    ui->toolButton_2->setEnabled(!ImagesStretchedToLabel); // zoom-in  button

    /* --- initial settings for enabling / disabling RGB channels --- */
    rgbDeltasBeforeDisabling.red   = 0;
    rgbDeltasBeforeDisabling.green = 0;
    rgbDeltasBeforeDisabling.blue  = 0;
}

MainWindow::~MainWindow() {
    delete ui;
}

/* --- SLOTS --- */

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item) {
    QVariant variant = item->data(Qt::UserRole);
    string data = item->text().toStdString();

    if (data == "  Load File") {
        // load images
        QSize originalSize = originalImage.loadImage(*this);
        modifiedImage.loadImage(originalImage);
        // output size at statusbar
        stringstream sstr;
        sstr << "width : " << originalSize.width() << ", height : " << originalSize.height() << endl;
        ui->statusBar->showMessage(QString(sstr.str().c_str()));

    } else if (data == "  Save File") {
        originalImage.saveImage();
    } else if (data == "  General") {
        ui->tabWidget->setCurrentIndex(0);
    } else if (data == "  RGB") {
        ui->tabWidget->setCurrentIndex(1);
    } else if (data == "  Analyze") {
        ui->tabWidget->setCurrentIndex(2);
    }
}

/* --- ACTIONS --- */

void MainWindow::on_actionZoom_in_triggered()
{
    originalImage.scaleImage(2.0);
    modifiedImage.scaleImage(2.0);
}

void MainWindow::on_actionZoom_out_triggered()
{
    originalImage.scaleImage(0.5);
    modifiedImage.scaleImage(0.5);
}

void MainWindow::on_actionZoom_Fit_triggered()
{
    ImagesStretchedToLabel = !ImagesStretchedToLabel; // toggle
    originalImage.stretchImageToLabel(ImagesStretchedToLabel);
    modifiedImage.stretchImageToLabel(ImagesStretchedToLabel);
    ui->toolButton->setEnabled(!ImagesStretchedToLabel);   // zoom-out button
    ui->toolButton_2->setEnabled(!ImagesStretchedToLabel); // zoom-in  button
}

void MainWindow::on_actionMaximize_triggered() {
    if(isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void MainWindow::on_actionClose_triggered() {
    close();
}

/* --- HANDLING OF CUSTOM TITLE-BAR --- */
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {

        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::on_actionLoad_from_modified_triggered()
{
    QSize originalSize = originalImage.loadImage(modifiedImage);
    // output size at statusbar
    stringstream sstr;
    sstr << "width : " << originalSize.width() << ", height : " << originalSize.height() << endl;
    ui->statusBar->showMessage(QString(sstr.str().c_str()));
}

void MainWindow::on_actionLoad_from_original_triggered()
{
    modifiedImage.loadImage(originalImage);
}

void MainWindow::on_actionSwap_triggered()
{
    bufferImage.loadImage(originalImage);
    originalImage.loadImage(modifiedImage);
    modifiedImage.loadImage(bufferImage);
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    signedRGBDelta delta;
    delta.red = ui->horizontalSlider->value();
    modifiedImage.modifyRGB(delta);
}

void MainWindow::on_horizontalSlider_2_sliderReleased()
{
    signedRGBDelta delta;
    delta.green = ui->horizontalSlider_2->value();
    modifiedImage.modifyRGB(delta);
}

void MainWindow::on_horizontalSlider_3_sliderReleased()
{
    signedRGBDelta delta;
    delta.blue = ui->horizontalSlider_3->value();
    modifiedImage.modifyRGB(delta);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if (checked) {
        ui->horizontalSlider->setValue(rgbDeltasBeforeDisabling.red);
    } else {
        rgbDeltasBeforeDisabling.red = ui->horizontalSlider->value();
        ui->horizontalSlider->setValue(-255);
    }
    on_horizontalSlider_sliderReleased();
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if (checked) {
        ui->horizontalSlider_2->setValue(rgbDeltasBeforeDisabling.green);
    } else {
        rgbDeltasBeforeDisabling.green = ui->horizontalSlider_2->value();
        ui->horizontalSlider_2->setValue(-255);
    }
    on_horizontalSlider_2_sliderReleased();
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    if (checked) {
        ui->horizontalSlider_3->setValue(rgbDeltasBeforeDisabling.blue);
    } else {
        rgbDeltasBeforeDisabling.blue = ui->horizontalSlider_3->value();
        ui->horizontalSlider_3->setValue(-255);
    }
    on_horizontalSlider_3_sliderReleased();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->horizontalSlider  ->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
    rgbDeltasBeforeDisabling.red   = 0;
    rgbDeltasBeforeDisabling.green = 0;
    rgbDeltasBeforeDisabling.blue  = 0;
    ui->checkBox  ->setChecked(true);
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setChecked(true);
    signedRGBDelta temp;
    temp.red   = 0;
    temp.green = 0;
    temp.blue  = 0;
    modifiedImage.modifyRGB(temp);
}

void MainWindow::on_pushButton_3_clicked()
{
    stringstream sstr;
    QColor maxColors = modifiedImage.getMaxRGB();

    ui->listWidget_2->clear();
    sstr << "Red Max : " << maxColors.red();
    ui->listWidget_2->addItem(QString(sstr.str().c_str()));
    sstr.str("");
    sstr << "Green Max : " << maxColors.green();
    ui->listWidget_2->addItem(QString(sstr.str().c_str()));
    sstr.str("");
    sstr << "Blue Max : " << maxColors.blue();
    ui->listWidget_2->addItem(QString(sstr.str().c_str()));

}

void MainWindow::on_pushButton_clicked()
{
    originalImage.todo();
}
