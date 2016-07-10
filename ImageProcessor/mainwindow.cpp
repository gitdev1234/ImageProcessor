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

    /* --- bind gui-elements to ImageProcessor-objects --- */
    originalImage = ImageProcessor(ui->label,ui->scrollArea_3);
    modifiedImage = ImageProcessor(ui->label_2,ui->scrollArea_4);

    originalImage.loadImage("no-image.png");
    originalImage.stretchImageToLabel(true);
    modifiedImage.loadImage("no-image.png");
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

void MainWindow::on_actionClose_triggered()
{
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


void MainWindow::on_checkBox_clicked(bool checked)
{
 ui->checkBox_2->setChecked(checked);
 originalImage.stretchImageToLabel(checked);
 modifiedImage.stretchImageToLabel(checked);
}

void MainWindow::on_toolButton_clicked()
{
    originalImage.scaleImage(0.5);
    modifiedImage.scaleImage(0.5);

}

void MainWindow::on_toolButton_2_clicked()
{
    originalImage.scaleImage(2.0);
    modifiedImage.scaleImage(2.0);

}

void MainWindow::on_toolButton_3_clicked()
{
    stringstream temp;
    temp << ui->scrollArea_3->verticalScrollBar()->maximum();
    ui->statusBar->showMessage(QString(temp.str().c_str()));
}
