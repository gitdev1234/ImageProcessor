#ifndef COMPONENT_H
#define COMPONENT_H

#include <QImage>

class ImageData {
    public:
        ImageData() {};
        virtual QPixmap getImageData() = 0;
};

#endif // COMPONENT_H
