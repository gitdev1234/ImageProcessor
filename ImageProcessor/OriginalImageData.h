#ifndef ORIGINALIMAGEDATA_H
#define ORIGINALIMAGEDATA_H

#include "ImageData.h"

class OriginalImageData : public ImageData {
   public:
        OriginalImageData() { };
        OriginalImageData(QImage& image_) {
            qimageToSignalProcessors(image_);
        };
        virtual QImage getImageData() {
            return QImage("");//imageData;
        };
};


#endif // ORIGINALIMAGEDATA_H
