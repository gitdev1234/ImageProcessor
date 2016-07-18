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
                unsigned int imageWidth = imageToLoadFrom_.width();
                grayScaleSignal = new SignalProcessor(imageHeight*imageWidth,true,0,255);
                for (unsigned int y = 0; y < imageHeight; y++) {
                    uchar* ar = imageToLoadFrom_.scanLine(y);
                    for (unsigned int x = 0; x < imageWidth; x++) {
                        int index = imageWidth * y + x;
                        (*grayScaleSignal)[index] = *ar;
                        ar++;
                    }
                }
            } else if (imageToLoadFrom_.format() == QImage::Format::Format_ARGB32) {
                // todo
            }
        };

        void signalProcessorsToQImage(QImage& imageToWriteTo_) {
            if (imageToWriteTo_.format() == QImage::Format::Format_Indexed8) {
                unsigned int imageHeight = imageToWriteTo_.height();
                unsigned int imageWidth = imageToWriteTo_.width();
                if (grayScaleSignal->size() == imageHeight * imageWidth) {
                    for (unsigned int y = 0; y < imageHeight; y++) {
                        uchar* ar = imageToWriteTo_.scanLine(y);
                        for (unsigned int x = 0; x < imageWidth; x++) {
                            int index = imageWidth * y + x;
                            *ar = (*grayScaleSignal)[index];
                            ar++;
                        }
                    }
                } else {
                    // error
                }
            } else if (imageToWriteTo_.format() == QImage::Format::Format_ARGB32) {
                // todo
            }
        }

        void getImage(QImage& image_) {
            getSignalProcessors(grayScaleSignal);
            signalProcessorsToQImage(image_);
        };
        virtual void getSignalProcessors(SignalProcessor* gray_) = 0;
        SignalProcessor* grayScaleSignal;
        SignalProcessor* redSignal;
        SignalProcessor* greenSignal;
        SignalProcessor* blueSignal;
        SignalProcessor* alphaChannelSignal;
};

#endif // COMPONENT_H
