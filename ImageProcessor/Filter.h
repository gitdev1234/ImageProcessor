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
        virtual void getSignalProcessors(SignalProcessor* gray_) {
            imageData->getSignalProcessors(grayScaleSignal);
            executeFilter();
            gray_ = grayScaleSignal;
        };
        void setActive(bool val_ = true) {
            active = val_;
        }
        bool getActive() {return active;};

        virtual void executeFilter() = 0;


    private:
        ImageData* imageData;
        bool active;




};

#endif // DECORATOR_H
