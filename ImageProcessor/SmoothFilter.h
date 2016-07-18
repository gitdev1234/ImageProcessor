#ifndef SMOOTHFILTER_H
#define SMOOTHFILTER_H

#include <iostream>
#include "Filter.h"

using namespace std;

class SmoothFilter : public Filter {
    public:
        SmoothFilter(ImageData* imageData_) : Filter(imageData_) {};
        virtual QPixmap getImageData() {
            return Filter::getImageData();
        };
    private:
        ImageData smoothFilter() {
            cout << "SmoothFilter : 50.3" << endl;
            return 50.3;
        }
};

#endif // SMOOTHFILTER_H
