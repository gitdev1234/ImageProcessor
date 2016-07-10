#include "imageprocessor.h"


/* --- load / save --- */
bool ImageProcessor::loadImage(const string& path_) {
    QPixmap *p=new QPixmap(QString(path_.c_str()));
    QPixmap p1(p->scaled ( p->size().width(),p->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ));
    label->setPixmap(p1);
    return true;
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

