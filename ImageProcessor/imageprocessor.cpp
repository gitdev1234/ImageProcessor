#include "imageprocessor.h"



void ImageProcessor::loadImageToLabel(const string& path_, QLabel& label_) {
    QPixmap pixMap(path_.c_str());
    label_.setPixmap((pixMap));
    label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label_.setScaledContents(true);
    int widthPicture = pixMap.width();
    int heightPicture = pixMap.height();
}

void ImageProcessor::saveImageFromLabel(const string &path_, QLabel &label_) {

}
