#ifndef ORIGINALIMAGEDATA_H
#define ORIGINALIMAGEDATA_H

#include "ImageData.h"

class OriginalImageData : public ImageData {
   public:
        OriginalImageData() { };
        virtual void getSignalProcessors(SignalProcessor* gray_) {
            gray_ = grayScaleSignal;
        };
        OriginalImageData(QImage& image_) {
            qimageToSignalProcessors(image_);
        };
};


#endif // ORIGINALIMAGEDATA_H
