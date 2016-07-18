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
        virtual QImage getImageData() {
            return imageData->getImageData();
        };


    private:
        ImageData* imageData;





};

#endif // DECORATOR_H
