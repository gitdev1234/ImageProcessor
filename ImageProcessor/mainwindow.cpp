#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <QPixmap>
#include <iostream>
#include "imageprocessor.h"

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

    /* --- bind gui-elements to ImageProcessor-objects --- */
    originalImage = ImageProcessor(ui->label);
    modifiedImage = ImageProcessor(ui->label_2);

    /* --- load initial images --- */
    QSize originalSize = originalImage.loadImage("no-image.png");
    modifiedImage.loadImage("no-image.png");

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
        //modifiedImage.loadImage(originalImage);
        // output size at statusbar
        stringstream sstr;
        sstr << "width : " << originalSize.width() << ", height : " << originalSize.height() << endl;
        ui->statusBar->showMessage(QString(sstr.str().c_str()));

    } else if (data == "  Save File") {
        originalImage.saveImage();
    } else if (data == "  General") {
        cout << "General" << endl;
        ui->tabWidget->setCurrentIndex(0);
    } else if (data == "  Method2") {
        cout << "Method2" << endl;
        ui->tabWidget->setCurrentIndex(1);
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
