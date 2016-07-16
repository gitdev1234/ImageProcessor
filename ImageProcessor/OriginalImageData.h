#ifndef ORIGINALIMAGEDATA_H
#define ORIGINALIMAGEDATA_H

#include "ImageData.h"

class OriginalImageData : public ImageData {
   public:
        OriginalImageData() { };
        OriginalImageData(const QPixmap* pixMap_) {
            imageData = QPixmap(*pixMap_);
        };
        virtual QPixmap getImageData() {
            return imageData;
        };
    private:
        QPixmap imageData;
};


#endif // ORIGINALIMAGEDATA_H
