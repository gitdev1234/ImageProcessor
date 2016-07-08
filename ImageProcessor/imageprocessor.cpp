#include "imageprocessor.h"




QPoint ImageProcessor::loadImageToLabel(const string& path_, QLabel& label_, bool scaled_) {
    QPixmap pixMap(path_.c_str());
    label_.setPixmap((pixMap));
    if (scaled_) {
        label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        label_.setScaledContents(true);
    }
    int widthPicture = pixMap.width();
    int heightPicture = pixMap.height();
    return QPoint(heightPicture,widthPicture);
}

void ImageProcessor::saveImageFromLabel(const string &path_, QLabel &label_) {

}
