#ifndef SMOOTHFILTER_H
#define SMOOTHFILTER_H

#include <iostream>
// ---
#include "Filter.h"
#include "ImageData.h"

using namespace std;

class SmoothFilter : public Filter {
    public:
        SmoothFilter() {};
        SmoothFilter(ImageData* imageData_) : Filter(imageData_) {};
        virtual void executeFilter() {
            if (getActive()) {
                vector<int> temp;
                temp.push_back(3);
                grayScaleSignal->modifySignalProcessor(ModificationType::MOVING_AVERAGE,temp);
            }
        }
};

#endif // SMOOTHFILTER_H
