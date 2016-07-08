#include "imageprocessor.h"


QPoint ImageProcessor::loadImageToLabel(QLabel& label_, bool scaled_) {
    //QFileDialog dialog(label_, QString("Open File"));
    //while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
    return QPoint(1,1);
}

QPoint ImageProcessor::loadImageToLabel(const string& path_, QLabel& label_, bool scaled_) {
    QPixmap pixMap(path_.c_str());
    label_.setPixmap((pixMap));
    setScalingOfLabel(scaled_,label_);
    int widthPicture = pixMap.width();
    int heightPicture = pixMap.height();
    return QPoint(heightPicture,widthPicture);
}

void ImageProcessor::saveImageFromLabel(QLabel &label_) {

}

void ImageProcessor::saveImageFromLabel(const string &path_, QLabel &label_) {
   // QFile file("yourFile.png");
    //file.open(QIODevice::WriteOnly);
    //(label_.pixmap())->save(&file, "PNG");
}

void ImageProcessor::setScalingOfLabel(bool activateScaling, QLabel &label_) {
    if (activateScaling) {
        label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label_.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }
    label_.setScaledContents(activateScaling);
}
