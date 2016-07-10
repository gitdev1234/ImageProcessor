#include "imageprocessor.h"


/* --- load / save --- */
bool ImageProcessor::loadImage(const string& path_) {
   /* // load image into pixmap of label
    QImage image(QString(path_.c_str()));
    label->setPixmap(QPixmap::fromImage(image));

    // save initial Sizes before scaling
    setImageSizeBeforeScaling(label->pixmap()->size());
    setHorizontalScrollBarSizeBeforeScaling(QSize(scrollArea->horizontalScrollBar()->minimum(),scrollArea->horizontalScrollBar()->maximum()));
    setVerticalScrollBarSizeBeforeScaling  (QSize(scrollArea->verticalScrollBar()->minimum()  ,scrollArea->verticalScrollBar()->maximum()));
*/
    QPixmap *p=new QPixmap(QString(path_.c_str()));
    QPixmap p1(p->scaled ( p->size().width(),p->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ));
    label->setPixmap(p1);
    return true;
}

/*bool loadImage();                    // with    file-open-dialog
bool saveImage(const string& path_); // without file-save-dialog
bool saveImage();                    // with    file-save-dialog
*/

/* --- scaling / zooming / stretching --- */
void ImageProcessor::stretchImageToLabel(bool stretchImageToLabel_) {
    if (stretchImageToLabel_) {
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    label->setScaledContents(true);
    /* --- reset scaling --- */
    // reset scaling factor
    setScalingFactor(1.0);

    // save initial Sizes before scaling
    setImageSizeBeforeScaling(label->pixmap()->size());
    setHorizontalScrollBarSizeBeforeScaling(QSize(scrollArea->horizontalScrollBar()->minimum(),scrollArea->horizontalScrollBar()->maximum()));
    setVerticalScrollBarSizeBeforeScaling  (QSize(scrollArea->verticalScrollBar()->minimum()  ,scrollArea->verticalScrollBar()->maximum()));
}

void ImageProcessor::scaleImage(double factor_) {
   /* setScalingFactor(getScalingFactor()*factor_);
    double newScalingFactor = getScalingFactor();
    QSize temp = label->size();
    label->resize(newScalingFactor * getImageSizeBeforeScaling());
    QSize temp2 = label->size();
    scaleScrollBar(*(scrollArea->verticalScrollBar()),factor_);
    scaleScrollBar(*(scrollArea->horizontalScrollBar()),factor_);
*/
   /* Q_ASSERT(label->pixmap());
    scalingFactor *= factor_;
    if ((scalingFactor < 3.0) && scalingFactor > 0.03) {
        cout << "scaling-factor : " << scalingFactor << endl;
        label->resize(scalingFactor * label->pixmap()->size());
        cout << "sizeLabel : " << label->size().width() << "," << label->size().height() << endl;
        cout << "sizePixmap : " << label->pixmap()->size().width() << "," <<  label->pixmap()->size().height() << endl;
        adjustScrollBar(scrollArea->horizontalScrollBar(), factor_);
        adjustScrollBar(scrollArea->verticalScrollBar(), factor_);
        scrollArea->horizontalScrollBar()->setMaximum(scrollArea->horizontalScrollBar()->maximum()*5);
        scrollArea->verticalScrollBar()->setMaximum(scrollArea->verticalScrollBar()->maximum()*5);
    }*/


    //zoomInAct->setEnabled(scaleFactor < 3.0);
    //zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageProcessor::adjustScrollBar(QScrollBar *scrollBar, double factor_) {
    scrollBar->setValue(int(factor_ * scrollBar->value()
                        + ((factor_ - 1) * scrollBar->pageStep()/2)));
}

void ImageProcessor::scaleScrollBar(QScrollBar& scrollBar_, double factor_) {
    /*// vertical scrollbar
    QSize verticalSize = getVerticalScrollBarSizeBeforeScaling();
    int min = verticalSize.width();
    int max = verticalSize.height();
    int sizeScrollBar = max-min;
    scrollArea->verticalScrollBar()->setMaximum(sizeScrollBar*factor_ + min);
    scrollArea->verticalScrollBar()->setValue(0);
    //horizontal scrollbar
    QSize horizontalSize = getHorizontalScrollBarSizeBeforeScaling();
    min = horizontalSize.width();
    max = horizontalSize.height();
    sizeScrollBar = max-min;
    scrollArea->horizontalScrollBar()->setMaximum(sizeScrollBar*factor_ + min);
    scrollArea->horizontalScrollBar()->setValue(0);
*/
    scrollArea->verticalScrollBar()->setValue(int(factor_ * scrollArea->verticalScrollBar()->value()
                                + ((factor_ - 1) * scrollArea->verticalScrollBar()->pageStep()/2)));
    scrollArea->horizontalScrollBar()->setValue(int(factor_ * scrollArea->horizontalScrollBar()->value()
                                + ((factor_ - 1) * scrollArea->horizontalScrollBar()->pageStep()/2)));
}

