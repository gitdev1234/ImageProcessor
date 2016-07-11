#include "imageprocessor.h"

/* --- load / save --- */
QSize ImageProcessor::loadImage(const string& path_) {
    QPixmap *p=new QPixmap(QString(path_.c_str()));
    QPixmap p1(p->scaled ( p->size().width(),p->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ));
    label->setPixmap(p1);
    return p1.size();
}

QSize ImageProcessor::loadImage(QWidget& widget_) {
    QFileDialog dialog(&widget_, QString("Open File"));
    if(dialog.exec() == QDialog::Accepted ) {
        QString path = dialog.selectedFiles().first();
        return loadImage(path.toStdString());
    } else {
        return QSize(0,0);
    }
}

QSize ImageProcessor::loadImage(const ImageProcessor& image_) {
    const QPixmap *pixMapToCopy = (image_.label->pixmap());
    QPixmap newPixMap(pixMapToCopy->scaled(pixMapToCopy->size().width(),pixMapToCopy->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ));
    label->setPixmap(newPixMap);
    return newPixMap.size();
}

bool ImageProcessor::saveImage() {
    QFileDialog dialog;
    QString path = dialog.getSaveFileName();
    return saveImage(path.toStdString());
}

bool ImageProcessor::saveImage(const string &path_) {
    QFile file(QString(path_.c_str()));
    file.open(QIODevice::WriteOnly);
    return label->pixmap()->save(&file, "PNG");
}

/* --- scaling / zooming / stretching --- */
void ImageProcessor::stretchImageToLabel(bool stretchImageToLabel_) {
    if (stretchImageToLabel_) {
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    label->setScaledContents(true);

}

void ImageProcessor::scaleImage(double factor_) {
    if (label->sizePolicy() == QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)) {
        QSize size = label->pixmap()->size();
        QPixmap temp = label->pixmap()->scaled(factor_*size,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(temp);
    }
}

void ImageProcessor::swap(ImageProcessor &image_) {
    // TODO
    //QPixmap& temp  = image_.label->pixmap();
    //label->pixmap()->swap(temp);
}

