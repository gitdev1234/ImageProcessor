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


    // Load initial image
    ImageProcessor imgP;
    QPoint size = imgP.loadImageToLabel("no-image.png",*(ui->label),true);

    // output size of image on statusbar
    stringstream sstr;
    sstr << "Width : " << size.x() << ", Heigth : " << size.y();
    QString qstr = QString::fromStdString(sstr.str());
    ui->statusBar->showMessage(qstr);

    ui->label_2->setPixmap(*(ui->label->pixmap()));
    const QPixmap *pi;
    pi = ui->label->pixmap();
    const QPixmap *pi2;
    pi2 = ui->label_2->pixmap();



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
    ImageProcessor imageProcessor;
    if (data == "  Load File") {
        cout << "Load File" << endl;
        QFileDialog dialog(this, QString("Open File"));
        if(dialog.exec() == QDialog::Accepted ) {
            ui->label->setPixmap(QPixmap(dialog.selectedFiles().first()));
        }
    } else if (data == "  Save File") {
        cout << "Save File" << endl;
        QFileDialog dialog;
        QString s = dialog.getSaveFileName();


        //if(dialog.exec() == QDialog::Accepted ) {
            const QPixmap *pixMap = ui->label->pixmap();
            QFile file(s);
            file.open(QIODevice::WriteOnly);
            pixMap->save(&file, "PNG");
        //}
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
 ImageProcessor imageProcessor;
 imageProcessor.setScalingOfLabel(checked,*(ui->label));
 imageProcessor.setScalingOfLabel(checked,*(ui->label_2));
}
