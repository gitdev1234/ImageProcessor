#ifndef COMPONENT_H
#define COMPONENT_H

#include <QImage>
// ---
#include "SignalProcessor.h"

class ImageData {
    public:
        ImageData() {};
        void qimageToSignalProcessors(QImage imageToLoadFrom_) {
            if (imageToLoadFrom_.format() == QImage::Format::Format_Indexed8) {
                unsigned int imageHeight = imageToLoadFrom_.height();
                for (unsigned int y = 0; y < imageHeight; y++) {
                    uchar* ar = imageToLoadFrom_.scanLine(y);
                    int imageWidth = imageToLoadFrom_.width();
                    grayScaleSignal = SignalProcessor(imageHeight*imageWidth,true,0,255);
                    for (unsigned int x = 0; x < imageWidth; x++) {
                        int index = imageWidth * y + x;
                        grayScaleSignal[index] = *ar;
                        ar++;
                    }
                }
            } else if (imageToLoadFrom_.format() == QImage::Format::Format_ARGB32) {
                    //todo
            }
        };

        virtual QImage getImageData() = 0;
        SignalProcessor grayScaleSignal;
        SignalProcessor redSignal;
        SignalProcessor greenSignal;
        SignalProcessor blueSignal;
        SignalProcessor alphaChannelSignal;
};

#endif // COMPONENT_H
