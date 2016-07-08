#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <QPixmap>
#include <iostream>

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


    // Load initial image
    QPixmap pixMap("no-image.png");
    int widthLabel  = ui->label->width();
    int heightLabel = ui->label->height();



    ui->label->setPixmap(pixMap);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label->setScaledContents(true);
    int widthPicture = pixMap.width();
    int heightPicture = pixMap.height();
    stringstream sstr;
    sstr << "Width : " << widthLabel << ", Heigth : " << heightLabel;
    QString qstr = QString::fromStdString(sstr.str());
    ui->statusBar->showMessage(qstr);

    // initial gui - settings
    setWindowFlags(Qt::FramelessWindowHint);

    ui->tabWidget->tabBar()->hide();
    showMaximized();




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
    } else if (data == "  Save File") {
        cout << "Save File" << endl;
    } else if (data == "  Method1") {
        cout << "Method1" << endl;
        ui->tabWidget->setCurrentIndex(0);
    } else if (data == "  Method2") {
        cout << "Method1" << endl;
        ui->tabWidget->setCurrentIndex(1);
    }
}

