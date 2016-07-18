#ifndef DECORATOR_H
#define DECORATOR_H

#include <QPixmap>
#include "ImageData.h"
#include "SignalProcessor.h"

class Filter : public ImageData {
    public:
        Filter(ImageData* imageData_) : imageData(imageData_) {};
        virtual QPixmap getImageData() {
            return imageData->getImageData();
        };


    private:
        ImageData* imageData;
        SignalProcessor grayScaleSignal;
        SignalProcessor redSignal;
        SignalProcessor greenSignal;
        SignalProcessor blueSignal;
        SignalProcessor alphaChannelSignal;

        void loadImageDataToSignalProcessors();


};

#endif // DECORATOR_H
