#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <string>

using namespace std;

class ImageProcessor {
public:
    ImageProcessor() {};
    QPoint loadImageToLabel(QLabel& label_, bool scaled_ = true); // with file-dialog
    QPoint loadImageToLabel(const string& path_,QLabel& label_, bool scaled_ = true); // without file-dialog
    void saveImageFromLabel(QLabel& label_);
    void saveImageFromLabel(const string& path_, QLabel& label_);
    void setScalingOfLabel(bool activateScaling, QLabel& label_);
};

#endif // IMAGEPROCESSOR_H
