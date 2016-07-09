#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <string>

using namespace std;

class ImageProcessor {
public:
    ImageProcessor() {};
    ImageProcessor(QLabel* label_, QScrollArea* scrollArea_)
     : label(label_), scrollArea(scrollArea_)
    { };

    /* --- getter / setter --- */
    double getScalingFactor()            {return scalingFactor;};
    void   setScalingFactor(double val_) {scalingFactor = val_;};

    /* --- load / save --- */
    bool loadImage(const string& path_); // without file-open-dialog
    bool loadImage();                    // with    file-open-dialog
    bool saveImage(const string& path_); // without file-save-dialog
    bool saveImage();                    // with    file-save-dialog

    /* --- scaling / zooming / stretching --- */
    void stretchImageToLabel(bool stretchImageToLabel_);
    void scaleImage(double factor_, QLabel& label_, QScrollArea& scrollArea_);

private:
    QLabel* label;
    QScrollArea* scrollArea;

    double scalingFactor = 1.0;
    void scaleScrollBar(QScrollBar& scrollBar_, double factor_);
};

#endif // IMAGEPROCESSOR_H
