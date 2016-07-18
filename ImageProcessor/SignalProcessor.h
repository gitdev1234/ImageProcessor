/**
 * SignalProcessor.h
 * Purpose: defines class SignalProcessor and enum ModificationType
 *
 * @author Wall.Of.Death
 * @version 1.0 20160713
 */

#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <vector>
#include <iostream>
#include <climits>
#include <cmath>

using namespace std;

/**
 * @brief The ModificationType enum
 * defines types of basis mathematical operations
 * this is needed for the implementation of SignalProcessor::modifySignalProcessor
 * and the implementation of mathematical operators
 */
enum ModificationType {ADD, SUBTRACT, MULTIPLY, DIVIDE, ADD_SIGNALPROCESSOR,
                       SUBTRACT_SIGNALPROCESSOR, MULTIPLY_SIGNALPROCESSOR,
                       DIVIDE_SIGNALPROCESSOR, THRESHOLD, BINARY_THRESHOLD,
                       MOVING_AVERAGE, GRADIENT};

/**
 * @brief The AnalyzationType enum
 * TODO
 */
enum AnalyzationType {MINIMUM, MAXIMUM, AVERAGE, STD_DEVIATION};

/**
 * class SignalProcessor
 * @brief class inherited from std::vector which is for basic calculations on a array of int values
 * The class SignalProcessor can make basic calculations of like :
 *  --> addition         of scalar values
 *  --> substraction     of scalar values
 *  --> multiplication with scalar values
 *  --> division         by scalar values
 *
 *  --> addition         of two Signals
 *  --> substraction     of two Signals
 *  --> multiplication with two Signals
 *  --> division       with two Signals
 *
 *  --> TODO
 *
 * The class SignalProcessor supports data with multiple channels!
 * For example if one has stored data with different content in the same signal,
 * (like in pictures for every pixel one value for red, one for green, one for blue
 * and one for alpha-channel (transparency),) the SignalProcessor class can work on either
 *
 *  --> all channels (R,G,B,A) at once   --> useMultiChannel == false
 *  --> or on only one channel per time  --> useMultiChannel == true
 *
 * The multi-channel-mode can be activated by setting first the number of channels
 *  --> signalProcessor_object.setChannelsCount(4);
 *
 * and then setting the useMultiChannel-flag
 *  --> signalProcessor_object.setUseMultiChannel(true);
 *
 * To define on which channel the SignalProcessor class has to work, set the selectedChannel - attribute
 * Please note that channels are indexed beginning at index 0 to index channelsCount - 1
 *  --> signalProcessor_object.setSelectedChannel(2)
 *
 * If one has done these three steps, all mathematical operators and the functions
 *   - modifySignalProcessor
 *   - analyzeSignalProcessor
 * work on only the data of the selected channel
 *
 */
class SignalProcessor : public vector<int>{
    public:
        /* --- constructors / destructors --- */

        // standard - constructor
        SignalProcessor(int  size_             =       0, bool useCutOffToRange_ =   false,
                        int  minValue_         = INT_MIN,  int  maxValue_        = INT_MAX);

        // copy - constructor
        SignalProcessor(const SignalProcessor& other_, bool copyVectorData_ = true); // copy-constructor

        /* --- miscellaneous --- */
        int getSize   ()           const;
        int getValueAt(int index_) const;

        double analyzeSignalProcessor(AnalyzationType analyzationType_) ;
        SignalProcessor modifySignalProcessor(ModificationType modificationType_, vector<int> val_ = {0} );
        int cutOffToRange(int val_);

        /* --- getter / setter --- */
        bool getUseCutOffToRange () const    { return useCutOffToRange;};
        int  getMinValue         () const    { return minValue;        };
        int  getMaxValue         () const    { return maxValue;        };
        void setUseCutOffToRange (bool val_) ;
        bool setMinMaxValue      (int  min_, int max_);

        /* --- operators --- */
        SignalProcessor& operator=(const SignalProcessor& other_);
        bool   operator==(const SignalProcessor& other_) const;
        bool   operator!=(const SignalProcessor& other_) const;
        SignalProcessor operator+(int val_) const;
        SignalProcessor operator-(int val_) const;
        SignalProcessor operator*(int val_) const;
        SignalProcessor operator/(int val_) const;
        SignalProcessor operator+(const SignalProcessor& val_) const;
        SignalProcessor operator-(const SignalProcessor& val_) const;
        SignalProcessor operator*(const SignalProcessor& val_) const;
        SignalProcessor operator/(const SignalProcessor& val_) const;
        friend ostream& operator<<(ostream& ostream_, const SignalProcessor signalProcessor_);
        friend ostream& operator<<(ostream& ostream_, vector<double> val_);
    private:
        bool useCutOffToRange = false;
        int  minValue         = INT_MIN;
        int  maxValue         = INT_MAX;
};

#endif // SIGNALPROCESSOR_H
