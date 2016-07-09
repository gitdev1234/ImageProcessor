#include "imageprocessor.h"


/* --- load / save --- */
bool ImageProcessor::loadImage(const string& path_) {
    (*label).setPixmap(QPixmap(QString(path_.c_str())));
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
}

void ImageProcessor::scaleImage(double factor_, QLabel &label_, QScrollArea& scrollArea_) {
    setScalingFactor(getScalingFactor()*factor_);
    double newScalingFactor = getScalingFactor();
    label_.resize(newScalingFactor * label_.size());
    scaleScrollBar(*(scrollArea_.verticalScrollBar()),factor_);
    scaleScrollBar(*(scrollArea_.horizontalScrollBar()),factor_);

}

void ImageProcessor::scaleScrollBar(QScrollBar& scrollBar_, double factor_) {
    int max = scrollBar_.maximum();
    int min = scrollBar_.minimum();
    int sizeScrollBar = max-min;
    scrollBar_.setMaximum(sizeScrollBar*factor_ + min);
    scrollBar_.setValue(0);
}

