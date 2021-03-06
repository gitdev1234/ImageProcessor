#ifndef IMAGE_H
#define IMAGE_H

#define _USE_MATH_DEFINES
#include <cmath>
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
#include "SignalProcessor.h"

using namespace std;

enum GradientType  {GR_NORMAL, GR_ABS, GR_MAGNITUDE, GR_DIRECTION};
enum ThresholdType {TH_NORMAL, TH_BINARY};

class Image {
public:
    /* --- constructors / destructors --- */
    Image() {};
    Image(QLabel* label_, QProgressBar* progressBar_)
        : label(label_), progressBar(progressBar_) { };

    /* --- getter / setter --- */
    void setPath(const string& path_ ) {path = path_;};
    const string& getPath() {return path;};

    /* --- load / save --- */
    QSize loadImage(QWidget &widget_, const string& path_); // without file-open-dialog
    QSize loadImage(QWidget& widget_);    // with    file-open-dialog
    QSize loadImage(const Image& image_);    // copy from image
    bool saveImage(const string& path_);  // without file-save-dialog
    bool saveImage();                     // with    file-save-dialog

    /* --- converting / grayScale / Formats --- */
    QImage convertToGrayScale(const QImage& image_);

    /* --- scaling / zooming / stretching --- */
    void stretchImageToLabel(bool stretchImageToLabel_);
    void scaleImage(double factor_);
    void setAndReScalePixMapAfterModification(const QImage& image_);

    /* --- modify image --- */
    void smooth(int horizontal_, int vertical_);
    void gradient(bool processVertical_, GradientType gradientType_, bool loadImageProcessorsBackToQImage_ = true);
    void invert();
    void threshold(ThresholdType threshholdType_, int thresholdValue_,
                   int smallerThanThresholdValue_, int biggerThanTrashholdValue_);
    void staticIncrease(int delta_);

    /* --- analyze image --- */
    QColor getMinMax(bool min_);
    QColor getAverage();
    QColor getStandardDeviation();
    void   calcHistogram();

    /* --- auto-detect --- */
    int    autoDetect();
    bool   checkPixelRequirements(int x_, int y_, int average_, int standardDeviation_);

private:
    QImage image;
    string path;
    QLabel* label;
    QProgressBar* progressBar;
    void setProgressBar(unsigned int percentage_);

    // Converting QImage <---> SignalProcessor
    void qImageToSignalProcessors(QImage imageToLoadFrom_, bool loadVerticalToVector_ = false);
    void signalProcessorsToQImage(QImage& imageToWriteTo_, bool loadFromSignalProcessorVector_ = false, bool loadVerticalFromVector_ = false);

    // one SignalProcessor per whole image
    SignalProcessor grayScaleSignal;
    // vector of SignalProcessors, for one signalProcessor per scanline
    vector<SignalProcessor> grayScaleSignalVector;

};

#endif // IMAGE_H
