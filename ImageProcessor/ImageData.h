#ifndef COMPONENT_H
#define COMPONENT_H

#include <QImage>

class ImageData {
    public:
        ImageData(QPixmap& pixMap_) {};
        virtual QPixmap getImageData() = 0;
    private:
        QPixmap imageData_;

};

#endif // COMPONENT_H
