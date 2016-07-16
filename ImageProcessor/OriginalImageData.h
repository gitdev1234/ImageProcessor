#ifndef ORIGINALIMAGEDATA_H
#define ORIGINALIMAGEDATA_H

#include <iostream>
#include "ImageData.h"

using namespace std;

class OriginalImageData : public ImageData {
   public:
        OriginalImageData() {};
        virtual QPixmap getImageData() {
            return imageData_;
        };
};

#endif // ORIGINALIMAGEDATA_H
