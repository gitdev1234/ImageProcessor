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
    originalImage = new ImageProcessor(ui->label,ui->scrollArea_3);
    modifiedImage = new ImageProcessor(ui->label_2,ui->scrollArea_4);

    originalImage.loadImage("no-image.png");
    originalImage.setScalingOfLabel(true);




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
 ImageProcessor imageProcessor((ui->label),(ui->scrollArea_3));
 imageProcessor.setScalingOfLabel(checked,*(ui->label));
 imageProcessor.setScalingOfLabel(checked,*(ui->label_2));
}

void MainWindow::on_toolButton_clicked()
{
    ImageProcessor imageProcessor((ui->label),(ui->scrollArea_3));
    //imageProcessor.scaleImage(2.0,*(ui->label),*(ui->scrollArea_3));
    //ui->label->resize(1.25 * ui->label->pixmap()->size());
    //ui->scrollArea_3->resize(1.25 * ui->label->pixmap()->size());
    double factor = 1.25;
    int i = ui->scrollArea_3->verticalScrollBar()->value();
    int max = ui->scrollArea_3->verticalScrollBar()->maximum();
    int min = ui->scrollArea_3->verticalScrollBar()->minimum();
    cout << "value : " <<  i << ", max : " << max << ", min : " << min << endl;
    ui->scrollArea_3->verticalScrollBar()->setValue(0);
    cout << "size --> w: " << ui->label->size().width() << ", h: " << ui->label->size().height() << endl;
    ui->label->resize(0.5*ui->label->size());
    int sizeScrollArea = max-min;

    ui->scrollArea_3->verticalScrollBar()->setMaximum(sizeScrollArea*0.5+min);

}

void MainWindow::on_toolButton_2_clicked()
{

    //imageProcessor.scaleImage(2.0,*(ui->label),*(ui->scrollArea_3));
    //ui->label->resize(1.25 * ui->label->pixmap()->size());
    //ui->scrollArea_3->resize(1.25 * ui->label->pixmap()->size());
    double factor = 1.25;
    int i = ui->scrollArea_3->verticalScrollBar()->value();
    int max = ui->scrollArea_3->verticalScrollBar()->maximum();
    int min = ui->scrollArea_3->verticalScrollBar()->minimum();
    cout << "value : " <<  i << ", max : " << max << ", min : " << min << endl;
    ui->scrollArea_3->verticalScrollBar()->setValue(0);
    cout << "size --> w: " << ui->label->size().width() << ", h: " << ui->label->size().height() << endl;
    ui->label->resize(1.5*ui->label->size());
    int sizeScrollArea = max-min;

    ui->scrollArea_3->verticalScrollBar()->setMaximum(sizeScrollArea*1.5+min);
}
