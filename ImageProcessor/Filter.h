#ifndef DECORATOR_H
#define DECORATOR_H

#include <QPixmap>
#include <QImage>
#include "ImageData.h"
#include "SignalProcessor.h"

class Filter : public ImageData {
    public:
        Filter() {};
        Filter(ImageData* imageData_) : imageData(imageData_) {};
        virtual QPixmap getImageData() {
            return imageData->getImageData();
        };

        void setFormat(QImage::Format val_) {format = val_;};


    private:
        ImageData* imageData;
        QImage::Format format;
        SignalProcessor grayScaleSignal;
        SignalProcessor redSignal;
        SignalProcessor greenSignal;
        SignalProcessor blueSignal;
        SignalProcessor alphaChannelSignal;

        void loadImageDataToSignalProcessors();


};

#endif // DECORATOR_H
