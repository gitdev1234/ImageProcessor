/**
 * SignalProcessor.cpp
 * Purpose: implements class SignalProcessor
 *
 * @author Wall.Of.Death
 * @version 1.0 20160713
 */

#include "SignalProcessor.h"

/* --- constructors / destructors --- */

/**
 * SignalProcessor::SignalProcessor
 * @brief standard constructor of class SignalProcessor
 * @param size_             size of vector
 * @param useCutOffToRange_ true if data is cut to minimum and maximum value during a modifySignalProcessor
 * @param minValue_         minimum value, needed for cutting data during modifySignalProcessor
 * @param maxValue_         maximum value, needed for cutting data during modifySignalProcessor
 *
 * creates vector of size 'size_' and initializes attributes
 *
 */
SignalProcessor::SignalProcessor(int size_     , bool useCutOffToRange_ ,
                                 int minValue_ , int maxValue_           )
    : vector<int>      ( size_             ),
      useCutOffToRange ( useCutOffToRange_ ),
      minValue         ( minValue_         ),
      maxValue         ( maxValue_         )
{ }

/**
 * SignalProcessor::SignalProcessor
 * @brief copy constructor of class SignalProcessor
 * @param other_ SignalProcessor object to copy from
 * @param copyVectorData_ if true constructor only copies attributes, if false constructor also copies content of vector
 */
SignalProcessor::SignalProcessor(const SignalProcessor &other_, bool copyVectorData_)
    : vector<int>      ( other_.getSize()        ),
      useCutOffToRange ( other_.getUseCutOffToRange() ),
      minValue         ( other_.getMinValue()         ),
      maxValue         ( other_.getMaxValue()         )
{
    if (copyVectorData_) {
        int size_l = other_.getSize();
        for (int i = 0; i < size_l; i++) {
            (*this)[i] = other_[i];
        }
    }
}

/* --- getters and setters --- */

/**
 * SignalProcessor::setUseCutOffToRange
 * @brief setter of useCutOffToRange
 * @param val_ true if data shall be cut to range of minValue and maxValue during modifySignalProcessor, otherwise false
 */
void SignalProcessor::setUseCutOffToRange(bool val_) {
    useCutOffToRange = val_;
}

/**
 * SignalProcessor::setMinMaxValue
 * @brief setter of minValue and maxValue
 * @param min_ new value for minValue
 * @param max_ new value for maxValue
 * @return returns true if it is allowed to set the range min_ -> max_
 * NOTE : min has to be smaller than max
 */
bool SignalProcessor::setMinMaxValue(int min_, int max_) {
    if (min_ < max_) {
        minValue = min_;
        maxValue = max_;
        return true;
    } else {
        return false;
    }
}

/* --- miscellaneous --- */
/**
 * SignalProcessor::getSize
 * @brief returns the number of fields within the vector
 * @return returns the number of fields within the vector
 * NOTE : this is needed, because vector::size() returns only unsigned int
 */
int SignalProcessor::getSize() const {
    return int( vector<int>::size() );
}

/**
 * SignalProcessor::getValueAt
 * @brief returns value at the index 'index_' within the vector-data
 * @param index_ index of the data which is requested
 * @return returns data at index 'index_'
 * NOTE : returns the data of index 0 if index is smaller than 0
 * NOTE : returns the data of index getSize() - 1 if index is bigger than getSize() - 1
 * NOTE : returns 0 if vector has no elements
 */
int SignalProcessor::getValueAt(int index_) const {
    int size_l = getSize();
    if( size_l  < 1 ) {
        return 0;
    }

    if( index_ < 0 ) {
        return (*this)[ 0 ];
    }
    if( index_ > size_l-1 ) {
        return (*this)[ size_l - 1 ];
    }
    return (*this)[ index_ ];

}


/**
 * SignalProcessor::modifySignalProcessor
 * @brief modifySignalProcessor iterates through all vector-data and modifies it depending on modificationType_
 * @param modificationType_ defines how SignalProcessor shall be modified
 * @param val_ value which shall be added, subtacted, multiplied ... with *this, if val_ is scalar use a vector with one element
 * @return returns the new SignalProcessor-object after modification
 * NOTE : This function encapsulates all logic for multiple channels within the data
 *        and for cutting data to the range of minValue and maxValue
 */
SignalProcessor SignalProcessor::modifySignalProcessor(ModificationType modificationType_, vector<int> val_ ) {
    // check dimensions of val_
    if ( (modificationType_ == ModificationType::ADD_SIGNALPROCESSOR     ) ||
         (modificationType_ == ModificationType::SUBTRACT_SIGNALPROCESSOR) ||
         (modificationType_ == ModificationType::MULTIPLY_SIGNALPROCESSOR) ||
         (modificationType_ == ModificationType::DIVIDE_SIGNALPROCESSOR  ) ){
        if (int(val_.size()) != getSize()) {
            return (*this);
        }
    } else {
        if (int(val_.size()) > 1) {
            return (*this);
        }
    }

    SignalProcessor copyBeforeModification;
    if ( (modificationType_ == ModificationType::MOVING_AVERAGE)  ||
         (modificationType_ == ModificationType::GRADIENT_NORMAL) ||
         (modificationType_ == ModificationType::GRADIENT_ABS)    ){
        copyBeforeModification = (*this);
    }

    int  size_l             = size();
    bool useCutOffToRange_l = getUseCutOffToRange();

    // iterate through all data
    for( int i = 0; i < size_l; i++ ) {
        // modify

        switch (modificationType_) {
            case ModificationType::ADD             :
                (*this)[ i ] = (*this)[ i ] + val_[0]; break;
            case ModificationType::SUBTRACT        :
                (*this)[ i ] = (*this)[ i ] - val_[0]; break;
            case ModificationType::MULTIPLY        :
                (*this)[ i ] = (*this)[ i ] * val_[0]; break;
            case ModificationType::DIVIDE          :
                if (val_[0] == 0) {
                   (*this)[ i ] = 0;
                } else {
                   (*this)[ i ] = (*this)[ i ] / val_[0]; break;
                }; break;
            case ModificationType::ADD_SIGNALPROCESSOR      :
                (*this)[ i ] = (*this)[ i ] + val_[i]; break;
            case ModificationType::SUBTRACT_SIGNALPROCESSOR :
                (*this)[ i ] = (*this)[ i ] - val_[i]; break;
            case ModificationType::MULTIPLY_SIGNALPROCESSOR :
                (*this)[ i ] = (*this)[ i ] * val_[i]; break;
            case ModificationType::DIVIDE_SIGNALPROCESSOR   :
                if (val_[i] == 0) {
                   (*this)[ i ] = 0;
                } else {
                   (*this)[ i ] = (*this)[ i ] / val_[i]; break;
                }; break;
            case ModificationType::THRESHOLD :
                if ((*this)[ i ] < val_[0]) {
                   (*this)[ i ] = 0 ;
                } break;
            case ModificationType::BINARY_THRESHOLD :
                if ((*this)[ i ] < val_[0]) {
                   (*this)[ i ] = 0;
                } else {
                   (*this)[ i ] = 1 ;
                }break;
            case ModificationType::MOVING_AVERAGE : {
                    int sum = 0;
                    for( int j = 0; j < val_[0]; j++ ) {
                        sum += copyBeforeModification.getValueAt( i+j-val_[0]/2 );
                    }

                    (*this)[i] = round(double(sum) / double( val_[0] ) );
                } break;
            case ModificationType::GRADIENT_NORMAL : {
                int sum = copyBeforeModification.getValueAt( i + 1) - copyBeforeModification.getValueAt( i - 1);
                (*this)[i] = round(double(sum) / double( 2 ) );
            } break;
            case ModificationType::GRADIENT_ABS : {
                int sum = copyBeforeModification.getValueAt( i + 1) - copyBeforeModification.getValueAt( i - 1);
                (*this)[i] = abs(round(double(sum) / double( 2 )));
            } break;
            case ModificationType::INVERT : {
                (*this)[i] = getMaxValue() - (*this)[i];
            } break;

            default : (*this)[i] = 0;
        }

        if (useCutOffToRange_l) {
            (*this)[ i ] = cutOffToRange((*this)[ i ]);
        }
    }

    return (*this);

}

int SignalProcessor::cutOffToRange(int val_) {
    int min = getMinValue();
    int max = getMaxValue();
    if (val_ < min) {
        val_ = min;
    } else if (val_ > max) {
        val_ = max;
    }
    return val_;
}

double SignalProcessor::analyzeSignalProcessor(AnalyzationType analyzationType_) {
    double result_l(1);
    double average_l;
    int size_l = getSize();

    switch (analyzationType_) {
        case AnalyzationType::MINIMUM       : result_l = INT_MAX; break;
        case AnalyzationType::MAXIMUM       : result_l = INT_MIN; break;
        case AnalyzationType::AVERAGE       : result_l =       0; break;
        case AnalyzationType::STD_DEVIATION : {   average_l = analyzeSignalProcessor(AnalyzationType::AVERAGE);
                                                result_l = 0;};   break;
        default : ; // TODO
    }

    // iterate through all data
    for( int i = 0; i < size_l; i++ ) {
        switch (analyzationType_) {
            case AnalyzationType::MINIMUM       : if ( (*this)[ i ] < result_l ) {
                                                    result_l = (*this)[ i ];
                                                  }; break;
            case AnalyzationType::MAXIMUM       : if ( (*this)[ i ] > result_l ) {
                                                    result_l = (*this)[ i ];
                                                  }; break;
            case AnalyzationType::AVERAGE       : result_l += (*this)[ i ]; break;
            case AnalyzationType::STD_DEVIATION : result_l += pow((double((*this)[ i ]) - double(average_l)),2); break;
            default : ; // TODO
        }
    }


    switch (analyzationType_) {
        case AnalyzationType::AVERAGE       : result_l = double(result_l) / double(size_l); break;
        case AnalyzationType::STD_DEVIATION : result_l = sqrt(double(result_l) / double(size_l)); break;
        default : ; // TODO
    }


    return result_l;
}

map<int, int> SignalProcessor::calcHistogram(const string &path_, const string &delimiter_) {
    int size_l = getSize();
    map<int, int> res;
    for( int i = 0; i < size_l; i++ ) {
        int value = (*this)[i];
        if(res.find(value) == res.end()) {
            res[value] = 1;
        } else {
            res[value] = res[value] + 1;
        }
    }
    if (path_ != "") {
        ofstream file;
        file.open(path_);
        typedef map<int, int>::iterator it_type;
        for(it_type iterator = res.begin(); iterator != res.end(); iterator++) {
            file << iterator->first;
            file << delimiter_;
            file << iterator->second << endl;
        }
        file.close();
    }
    return res;
}


/* --- operators --- */

/**
 * SignalProcessor::operator =
 * @brief assignment-operator, copies all content of other_
 * @param other_ SignalProcessor object to copy from
 * @return returns a new SignalProcessor instance which is copied from other
 * creates a new instance of SignalProcessor and copies the attributes
 * and vector-data of other_ to it
 */
SignalProcessor& SignalProcessor::operator=( const SignalProcessor& other_) {
    // proof for identical address
    if (this == &other_) {
        return (*this);
    }

    // copy attributes
    resize(other_.getSize());
    setUseCutOffToRange ( other_.getUseCutOffToRange()              );
    setMinMaxValue      ( other_.getMinValue(),other_.getMaxValue() );

    // copy vector-data
    int size_l = other_.getSize();
    for (int i = 0; i < size_l; i++) {
        (*this)[i] = other_[i];
    }

    return *this;

}

/**
 * SignalProcessor::operator ==
 * @brief equality-operator, checks if all content is equal
 * @param other_ SignalProcessor-object to compare with
 * @return returns true if all attributes and vector-data are equal, otherwise false
 * checks if all attributes and vector-data of *this and other_ are equal
 */
bool SignalProcessor::operator==( const SignalProcessor& other_) const {
    // check if attributes are identical
    bool isIdentical = ( getSize             () == other_.getSize             () &&
                         getUseCutOffToRange () == other_.getUseCutOffToRange () &&
                         getMinValue         () == other_.getMinValue         () &&
                         getMaxValue         () == other_.getMaxValue         () );

    // check if vector-data is identical
    if (isIdentical) {
        int size_l = getSize();
        for (int i = 0; i < size_l; i++) {
            if ((*this)[i] != other_[i]) {
                isIdentical = false;
            }
        }
    }

    return isIdentical;
}

/**
 * SignalProcessor::operator !=
 * @brief inequality-operator, checks if anything is different
 * @param other_ SignalProcessor object to compare with
 * @return returns true if any attribute or any content of the vector is different, otherwise false
 * checks if any attribute or any vector-data of *this and other_ is not equal
 */
bool SignalProcessor::operator!=( const SignalProcessor& other_) const {
    // check if identical and return the invers
    return (!((*this == other_)));
}

/**
 * SignalProcessor::operator +
 * @brief addition-operator, adds a SignalProcessor-object and an int value
 * @param val_ value to add with
 * @return returns a new SignalProcessor-object
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator+( int val_ ) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::ADD,temp);
}

/**
 * SignalProcessor::operator -
 * @brief substraction-operator, substracts an int value from a SignalProcessor-object
 * @param val_ value to subtract from SignalProcessor object
 * @return returns a new SignalProcessor-object
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator-( int val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::SUBTRACT,temp);
}

/**
 * SignalProcessor::operator *
 * @brief multiplication-operator, multiplies a SignalProcessor-object and an int value
 * @param val_ value to multiply with
 * @return returns a new SignalProcessor-object
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator*( int val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::MULTIPLY,temp);
}

/**
 * SignalProcessor::operator /
 * @brief division-operator, divides a SignalProcessor-object by an int value
 * @param val_ value to divide by
 * @return returns a new SignalProcessor-object
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator/( int val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::DIVIDE,temp);
}

/**
 * SignalProcessor::operator +
 * @brief addition-operator, adds two SignalProcessor-objects, element by element
 * @param val_ SignalProcessor-object to add with
 * @return returns a new SignalProcessor-object
 * NOTE : can only get used for two SignalProcessor-objects with same size
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator+( const SignalProcessor& val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::ADD_SIGNALPROCESSOR,temp);
}

/**
 * SignalProcessor::operator -
 * @brief substraction-operator, substracts two SignalProcessor-objects, element by element
 * @param val_ SignalProcessor-object which is subtracted from *this
 * @return returns a new SignalProcessor-object
 * NOTE : can only get used for two SignalProcessor-objects with same size
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator-( const SignalProcessor& val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::SUBTRACT_SIGNALPROCESSOR,temp);
}

/**
 * SignalProcessor::operator *
 * @brief multiplication-operator, multiplies two SignalProcessor-objects, element by element
 * @param val_ SignalProcessor-object to multiply with
 * @return returns a new SignalProcessor-object
 * NOTE : can only get used for two SignalProcessor-objects with same size
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator*( const SignalProcessor& val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    vector<int> temp = {val_};
    return resultSignalProcessor.modifySignalProcessor(ModificationType::MULTIPLY_SIGNALPROCESSOR,temp);
}

/**
 * SignalProcessor::operator /
 * @brief division-operator, divides two SignalProcessor-objects, element by element
 * @param val_ SignalProcessor-object by which *this is divided by
 * @return returns a new SignalProcessor-object
 * NOTE : can only get used for two SignalProcessor-objects with same size
 * NOTE : uses SignalProcessor::cutOffToRange if SignalProcessor::useCutOffToRange == true
 * NOTE : only modifies data of a specific channel if SignalProcessor::useMultiChannel == true
 * NOTE : modifies all data if SignalProcessor::useMultiChannel == false
 */
SignalProcessor SignalProcessor::operator/( const SignalProcessor& val_) const {
    // copy SignalProcessor because operator is const
    SignalProcessor resultSignalProcessor(*this,true);
    return resultSignalProcessor.modifySignalProcessor(ModificationType::DIVIDE_SIGNALPROCESSOR,val_);
}

/**
 * operator <<
 * @brief shift-out operator for print SignalProcessor-objects to console
 * @param ostream_ stream-object before printing SignalProcessor-object to it
 * @param SignalProcessor_ SignalProcessor object which shall be printed
 * @return returns stream object after printing SignalProcessor-object to it
 * prints out vector-data of SignalProcessor object to console
 * printign a SignalProcessor with the elements {1,2,4,5,100} looks like this:
 * [1,2,4,5,100]
 */
ostream& operator<<(ostream& ostream_, const SignalProcessor signalProcessor_) {
    int size_l = signalProcessor_.getSize();

    ostream_ << "[";
    for (int i = 0; i < size_l -1; i++) {
        ostream_ << signalProcessor_.getValueAt( i ) << ",";

    }
    ostream_ << signalProcessor_.getValueAt((size_l-1)) << "]" << endl;
    return ostream_;
}

ostream& operator<<(ostream& ostream_, vector<double> val_) {
    ostream_ << "[";
    for (unsigned int i = 0; i < val_.size() -1; i++) {
        ostream_ << val_[i] << ",";
    }
    ostream_ << val_[val_.size()-1] << "]" << endl;
    return ostream_;
}
