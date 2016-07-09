#include "imageprocessor.h"



/* --- scaling / zooming --- */
void ImageProcessor::setScalingOfLabel(bool activateScaling, QLabel &label_) {
    if (activateScaling) {
        label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label_.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }
    label_.setScaledContents(activateScaling);
}

void ImageProcessor::scaleImage(double factor_, QLabel &label_, QScrollArea& scrollArea_) {
    setScalingFactor(getScalingFactor()*factor_);
    double newScalingFactor = getScalingFactor();
    label_.resize(newScalingFactor * label_.pixmap()->size());


}

void ImageProcessor::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
