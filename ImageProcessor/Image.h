#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QProgressBar>
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QFile>
#include <QFileDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <string>
#include <iostream>
#include <QMessageBox>
//---
#include "Types.h"

using namespace std;

class Image {
public:
    /* --- constructors / destructors --- */
    Image() {};
    Image(QLabel* label_, QProgressBar* progressBar_)
        : label(label_), progressBar(progressBar_) { };

    /* --- getter / setter --- */

    /* --- load / save --- */
    QSize loadImage(QWidget &widget_, const string& path_); // without file-open-dialog
    QSize loadImage(QWidget& widget_);    // with    file-open-dialog
    QSize loadImage(const Image& image_);    // copy from image
    bool saveImage(const string& path_);  // without file-save-dialog
    bool saveImage();                     // with    file-save-dialog

    /* --- scaling / zooming / stretching --- */
    void stretchImageToLabel(bool stretchImageToLabel_);
    void scaleImage(double factor_);
    void swap(Image& image_);

    /* --- modify image --- */
    void modifyRGB(signedRGBDelta delta_);
    void setAndReScalePixMapAfterModification(const QImage& image_);
    /* --- analyse image --- */
    QColor getMaxRGB();
    void todo();
private:
    QImage image;
    QLabel* label;
    QProgressBar* progressBar;

    void setProgressBar(unsigned int percentage_);
};

#endif // IMAGE_H
