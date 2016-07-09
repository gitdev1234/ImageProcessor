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

    /* --- getter / setter --- */
    double getScalingFactor()            {return scalingFactor;};
    void   setScalingFactor(double val_) {scalingFactor = val_;};

    /* --- Scaling / Zooming --- */
    void setScalingOfLabel(bool activateScaling, QLabel& label_);
    //void scaleImage(double factor_, QLabel& label_, QScrollArea& scrollArea_);
private:
    double scalingFactor = 1.0;
    //void adjustScrollBar(QScrollBar *scrollBar, double factor);
};

#endif // IMAGEPROCESSOR_H
