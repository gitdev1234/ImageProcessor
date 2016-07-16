#ifndef DECORATOR_H
#define DECORATOR_H

#include "ImageData.h"

class Filter : public ImageData {
    public:
        Filter(ImageData* imageData_) : imageData(imageData_) {};
        virtual QPixmap getImageData() {
            return imageData->getImageData();
        };

    private:
        ImageData* imageData;


};

#endif // DECORATOR_H
