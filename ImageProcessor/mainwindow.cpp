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

    /* --- bind gui-elements to Image-objects --- */
    originalImage = Image(ui->label   ,ui->progressBar);
    modifiedImage = Image(ui->label_2 ,ui->progressBar);
    resultImage   = Image(ui->label_18,ui->progressBar);

    /* --- load initial images --- */
    QSize originalSize = originalImage.loadImage(*this,"Sample03.png");
    modifiedImage.loadImage(*this,"Sample03.png");

    /* --- output size at statusbar --- */
    stringstream sstr;
    sstr << "Filename : " << originalImage.getPath();
    sstr << " | Size : width = " << originalSize.width() << ", height = " << originalSize.height() << endl;
    ui->statusBar->showMessage(QString(sstr.str().c_str()));

    /* --- initial gui - settings ---*/
    setWindowFlags(Qt::FramelessWindowHint); //needed for custom titleBar (deactivates normal statusbar)
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget_2->tabBar()->hide();
    showMaximized();
    ui->toolButton->  setDefaultAction(ui->actionZoom_out);
    ui->toolButton_2->setDefaultAction(ui->actionZoom_in);
    ui->toolButton_3->setDefaultAction(ui->actionZoom_Fit);
    ui->toolButton_4->setDefaultAction(ui->actionLoad_from_original);
    ui->toolButton_5->setDefaultAction(ui->actionLoad_from_modified);
    ui->toolButton_6->setDefaultAction(ui->actionSwap);
    ui->listWidget->item(0)->setSelected(true);  // General
    ui->listWidget->item(3)->setSelected(true);  // Analyze
    ui->listWidget->item(4)->setSelected(true);  // Filter
    ui->listWidget->item(9)->setSelected(true);  // AutoDetect
    ui->listWidget->item(10)->setSelected(true); // Modify
    ui->horizontalSlider_4->setMaximum(originalSize.width());
    ui->horizontalSlider_5->setMaximum(originalSize.height());
    ui->label_10->setText(QString::number(originalSize.width()));
    ui->label_11->setText(QString::number(originalSize.height()));
    ui->horizontalSlider->setMaximum(originalImage.getMinMax(false).alpha());
    ui->horizontalSlider->setValue(originalImage.getMinMax(false).alpha()/2);
    ui->label_4->setText(QString::number(originalImage.getMinMax(false).alpha()));
    ui->horizontalSlider_2->setMaximum(originalImage.getMinMax(false).alpha());
    ui->label_13->setText(QString::number(originalImage.getMinMax(false).alpha()));
    ui->horizontalSlider_3->setMaximum(originalImage.getMinMax(false).alpha());
    ui->label_16->setText(QString::number(originalImage.getMinMax(false).alpha()));

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
        // output filename and size at statusbar
        stringstream sstr;
        sstr << "Filename : " << originalImage.getPath();
        sstr << " | Size : width = " << originalSize.width() << ", height = " << originalSize.height() << endl;
        ui->statusBar->showMessage(QString(sstr.str().c_str()));
        // set slider for smooth
        ui->horizontalSlider_4->setMaximum(originalSize.width());
        ui->horizontalSlider_5->setMaximum(originalSize.height());
        ui->label_10->setText(QString::number(originalSize.width()));
        ui->label_11->setText(QString::number(originalSize.height()));
    } else if (data == "  Save File") {
        originalImage.saveImage();
    } else if (data == "  --- Analyze ---") {
        ui->tabWidget->setCurrentIndex(0);
        ui->tabWidget_2->setCurrentIndex(0);
    } else if (data == "  Smooth") {
        ui->tabWidget->setCurrentIndex(1);
    } else if (data == "  Gradient") {
        ui->tabWidget->setCurrentIndex(2);
    } else if (data == "  Threshold") {
        ui->tabWidget->setCurrentIndex(3);
    } else if (data == "  Invert") {
        originalImage.invert();
    } else if (data == "  --- Auto-Detect ---") {
        resultImage.loadImage(originalImage);
        resultImage.stretchImageToLabel(true);
        int estimatedSize = resultImage.autoDetect();
        stringstream sstr;
        sstr << "Estimated Size : " << estimatedSize << " Pixels";
        ui->label_19->setText(QString(sstr.str().c_str()));
        ui->tabWidget_2->setCurrentIndex(1);
    }   else if (data == "  Static Inc / Dec") {
        ui->tabWidget->setCurrentIndex(4);
    }

    ui->listWidget->item(0)->setSelected(true );  // General
    ui->listWidget->item(1)->setSelected(false);  // Load
    ui->listWidget->item(2)->setSelected(false);  // Save
    ui->listWidget->item(3)->setSelected(true );  // Analyze
    ui->listWidget->item(4)->setSelected(true );  // Filter
    ui->listWidget->item(5)->setSelected(false);  // Smooth
    ui->listWidget->item(6)->setSelected(false);  // Gradient
    ui->listWidget->item(7)->setSelected(false);  // Threshold
    ui->listWidget->item(8)->setSelected(false);  // Invert
    ui->listWidget->item(9)->setSelected(true );  // Auto-Detect
    ui->listWidget->item(10)->setSelected(true);  // Modify Colors
    ui->listWidget->item(11)->setSelected(false); // Increase Decrease
}

void MainWindow::on_listWidget_3_itemClicked(QListWidgetItem *item) {
    QVariant variant = item->data(Qt::UserRole);
    string data = item->text().toStdString();
    QColor result;
    stringstream sstr;

    ui->listWidget_2->clear();
    if (data == "  Minimum") {
        result = originalImage.getMinMax(true);
    } else if (data == "  Maximum") {
        result = originalImage.getMinMax(false);
    } else if (data == "  Average") {
        result = originalImage.getAverage();
    } else if (data == "  Standard Deviation") {
        result = originalImage.getStandardDeviation();
    } else if (data == "  Histogram") {
        originalImage.calcHistogram();
    }

    sstr << data << " : " << result.red();
    ui->listWidget_2->addItem(QString(sstr.str().c_str()));
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
    sstr << "Filename : " << originalImage.getPath();
    sstr << " | Size : width = " << originalSize.width() << ", height = " << originalSize.height() << endl;
    ui->statusBar->showMessage(QString(sstr.str().c_str()));
    // set sliders for smooth
    ui->horizontalSlider_4->setMaximum(originalSize.width());
    ui->horizontalSlider_5->setMaximum(originalSize.height());
    ui->label_10->setText(QString::number(originalSize.width()));
    ui->label_11->setText(QString::number(originalSize.height()));
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


void MainWindow::on_pushButton_clicked()
{
    bool processVertical = (ui->comboBox_2->currentIndex() == 1);
    GradientType gradientType;
    switch (ui->comboBox_3->currentIndex()) {
        case 0 : gradientType = GradientType::GR_NORMAL; break;
        case 1 : gradientType = GradientType::GR_ABS; break;
        case 2 : gradientType = GradientType::GR_MAGNITUDE; break;
        case 3 : gradientType = GradientType::GR_DIRECTION; break;
    }

    originalImage.gradient(processVertical,gradientType);
}

void MainWindow::on_pushButton_4_clicked()
{
    originalImage.smooth(ui->horizontalSlider_4->value(),ui->horizontalSlider_5->value());
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_8->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    ui->label_9->setText(QString::number(value));
}

void MainWindow::on_progressBar_valueChanged(int value)
{
}

void MainWindow::on_toolButton_4_clicked()
{

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
     ui->label_3->setText(QString::number(value));
}

void MainWindow::on_pushButton_2_clicked()
{
    ThresholdType thresholdType;
    if (ui->comboBox_4->currentIndex() == 0) {
        thresholdType = ThresholdType::TH_BINARY;
    } else {
        thresholdType = ThresholdType::TH_NORMAL;
    }

    originalImage.threshold(thresholdType, ui->horizontalSlider->value(),
                            ui->horizontalSlider_2->value(),ui->horizontalSlider_3->value());
}

void MainWindow::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
    bool hideControls = (arg1.toStdString() != "Threshold Type : Normal Threshold");
    ui->label_15->setVisible(hideControls);
    ui->label_16->setVisible(hideControls);
    ui->label_17->setVisible(hideControls);
    ui->horizontalSlider_3->setVisible(hideControls);

}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_14->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_17->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    ui->label_21->setText(QString::number(value));
}

void MainWindow::on_pushButton_3_clicked()
{
    originalImage.staticIncrease(ui->horizontalSlider_6->value());
    ui->horizontalSlider_6->setValue(0);

}
