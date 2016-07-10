#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <string>
#include <iostream>

using namespace std;

class ImageProcessor {
public:
    /* --- constructors / destructors --- */
    ImageProcessor() {};
    ImageProcessor(QLabel* label_) : label(label_) { };

    /* --- operators --- */
    ImageProcessor& operator=( const ImageProcessor& other_ );

    /* --- getter / setter --- */

    /* --- load / save --- */
    QSize loadImage(const string& path_); // without file-open-dialog
    QSize loadImage(QWidget& widget_);    // with    file-open-dialog
    QSize loadImage(const ImageProcessor& image_);    // copy from image
    bool saveImage(const string& path_);  // without file-save-dialog
    bool saveImage();                     // with    file-save-dialog

    /* --- scaling / zooming / stretching --- */
    void stretchImageToLabel(bool stretchImageToLabel_);
    void scaleImage(double factor_);
protected:
    QLabel* label;
};

#endif // IMAGEPROCESSOR_H
