#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <QPixmap>
#include <iostream>
#include "imageprocessor.h"

using namespace std;

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

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

    /* --- bind gui-elements to ImageProcessor-objects --- */
    originalImage = ImageProcessor(ui->label,ui->scrollArea_3);
    modifiedImage = ImageProcessor(ui->label_2,ui->scrollArea_4);

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




}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QVariant variant = item->data(Qt::UserRole);
    string data = item->text().toStdString();

    if (data == "  Load File") {
        cout << "Load File" << endl;
        QFileDialog dialog(this, QString("Open File"));
        if(dialog.exec() == QDialog::Accepted ) {
            QString path = dialog.selectedFiles().first();
            QImageReader reader(path);
            //reader.setAutoTransform(true);
            const QImage newImage = reader.read();
            /*if (newImage.isNull()) {
                QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                         tr("Cannot load %1: %2")
                                         .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
                return false;
            }*/


            //ui->label->setPixmap(QPixmap::fromImage(newImage));
            //ui->label->setPixmap(QPixmap(path));
            ImageProcessor imageProcessor((ui->label),(ui->scrollArea_3));
            imageProcessor.loadImage(path.toStdString());
        }
    } else if (data == "  Save File") {
        cout << "Save File" << endl;
        QFileDialog dialog;
        QString s = dialog.getSaveFileName();
        const QPixmap *pixMap = ui->label->pixmap();
        QFile file(s);
        file.open(QIODevice::WriteOnly);
        pixMap->save(&file, "PNG");

    } else if (data == "  General") {
        cout << "General" << endl;
        ui->tabWidget->setCurrentIndex(0);
    } else if (data == "  Method2") {
        cout << "Method2" << endl;
        ui->tabWidget->setCurrentIndex(1);
    }
}


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
    originalImage.stretchImageToLabel(!ImagesStretchedToLabel);
    modifiedImage.stretchImageToLabel(!ImagesStretchedToLabel);
    ui->toolButton->setEnabled(ImagesStretchedToLabel);   // zoom-out button
    ui->toolButton_2->setEnabled(ImagesStretchedToLabel); // zoom-in  button
}
