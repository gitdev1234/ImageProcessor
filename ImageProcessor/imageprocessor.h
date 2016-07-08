#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QLabel>
#include <QPixmap>
#include <string>

using namespace std;

class ImageProcessor {
public:
    ImageProcessor() {};
    QPoint loadImageToLabel(const string& path_,QLabel& label_, bool scaled_ = true);
    void saveImageFromLabel(const string& path_, QLabel& label_);
};

#endif // IMAGEPROCESSOR_H
