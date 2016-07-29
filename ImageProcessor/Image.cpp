#include "Image.h"

/* --- load / save --- */
QSize Image::loadImage(QWidget& widget_,const string& path_) {
    image = QImage(QString(path_.c_str()));
    setPath(path_);

    switch(image.format()) {
        case QImage::Format_RGB32 : image.convertToFormat(QImage::Format_ARGB32); break;
        case QImage::Format_ARGB32 : ; break;
        case QImage::Format_Indexed8 : if (!image.allGray()) image = convertToGrayScale(image); break;
        default : {
                    QMessageBox::information(&widget_,QString("Error"),QString("Error : Image-Type not supported"));
                    image = QImage();
                    label->setPixmap(QPixmap(0,0));
                  } break;
    }
    qImageToSignalProcessors(image); // load image into signal processors
    label->setPixmap(QPixmap::fromImage(image));
    return label->pixmap()->size();
}

QSize Image::loadImage(QWidget& widget_) {
    QFileDialog dialog(&widget_, QString("Open File"));
    if(dialog.exec() == QDialog::Accepted ) {
        QString path = dialog.selectedFiles().first();
        return loadImage(widget_,path.toStdString());
    } else {
        return QSize(0,0);
    }
}

QSize Image::loadImage(const Image& image_) {
    image = QImage(image_.image);
    qImageToSignalProcessors(image); // load image into signal processors
    label->setPixmap(QPixmap::fromImage(image));
    return label->pixmap()->size();
}

bool Image::saveImage() {
    QFileDialog dialog;
    QString path = dialog.getSaveFileName();
    return saveImage(path.toStdString());
}

bool Image::saveImage(const string &path_) {
    QFile file(QString(path_.c_str()));
    file.open(QIODevice::WriteOnly);
    return label->pixmap()->save(&file, "PNG");
}

/* --- converting / grayScale / Formats --- */
QImage Image::convertToGrayScale(const QImage& image_) {
    // create image of same size
    QImage image_gray(image_.width(), image_.height(), QImage::Format_Indexed8);

    // create color table for gray scale
    QVector<QRgb> grayscales;
    for (int i=0; i<256; ++i) {
        grayscales.push_back(qRgb(i,i,i));
    }

    // assign color table to new image
    image_gray.setColorTable(grayscales);

    // convert old colors
    for (int y=0; y<image_.height(); ++y) {
        for (int x=0; x<image_.width(); ++x) {
               // get old color
               QRgb rgb = image_.pixel(x,y);
               // convert to grayscale value
               unsigned char gray = 0.299*qRed(rgb) + 0.587*qGreen(rgb) + 0.114*qBlue(rgb);
               // assign grayscale value to new image
               image_gray.setPixel(x,y, gray);
         }
    }
    return image_gray;
}

/* --- scaling / zooming / stretching --- */
void Image::stretchImageToLabel(bool stretchImageToLabel_) {
    if (stretchImageToLabel_) {
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    label->setScaledContents(true);

}

void Image::scaleImage(double factor_) {
    if (label->sizePolicy() == QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)) {
        QSize size = label->pixmap()->size();
        QPixmap temp = label->pixmap()->scaled(factor_*size,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(temp);
    }
}

void Image::swap(Image &image_) {
    // TODO
    //QPixmap& temp  = image_.label->pixmap();
    //label->pixmap()->swap(temp);
}


void Image::setAndReScalePixMapAfterModification(const QImage &image_) {
    QSize sizeBeforeModification = label->pixmap()->size();
    label->setPixmap(QPixmap().fromImage(image).scaled(sizeBeforeModification,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //label->setPixmap(label->pixmap()->scaled(tempSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

/* --- analyze image --- */

QColor Image::getMinMax(bool min_) {
    QColor res;
    double minMax;
    qImageToSignalProcessors(image);
    if (image.format() == QImage::Format_Indexed8) {
        if (min_ ) {
            minMax = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::MINIMUM);
        } else {
            minMax = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::MAXIMUM);
        }
        res.setAlpha (minMax);
        res.setRed   (minMax);
        res.setGreen (minMax);
        res.setBlue  (minMax);
    } else if (image.format() == QImage::Format_ARGB32) {
        //TODO
    }
    return res;
}

QColor Image::getAverage() {
    qImageToSignalProcessors(image);
    QColor res;
    double average;
    if (image.format() == QImage::Format_Indexed8) {
        average = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::AVERAGE);
        res.setAlpha (average);
        res.setRed   (average);
        res.setGreen (average);
        res.setBlue  (average);
    } else if (image.format() == QImage::Format_ARGB32) {
        //TODO
    }
    return res;
}

QColor Image::getStandardDeviation() {
    qImageToSignalProcessors(image);
    QColor res;
    double standardDeviation;
    if (image.format() == QImage::Format_Indexed8) {
        standardDeviation = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::STD_DEVIATION);
        res.setAlpha (standardDeviation);
        res.setRed   (standardDeviation);
        res.setGreen (standardDeviation);
        res.setBlue  (standardDeviation);
    } else if (image.format() == QImage::Format_ARGB32) {
        //TODO
    }
    return res;
}

void Image::calcHistogram() {
    qImageToSignalProcessors(image);
    QFileDialog dialog;
    QString path = dialog.getSaveFileName();
    if (image.format() == QImage::Format_Indexed8) {
        grayScaleSignal.calcHistogram(path.toStdString());
    } else if (image.format() == QImage::Format_ARGB32) {
        //TODO
    }
}

void Image::setProgressBar(unsigned int percentage_) {
    progressBar->setValue(percentage_);
}


void Image::smooth(int horizontal_,int vertical_,bool processVertical_) {
    int imageHeight = image.height();
    int imageWidth  = image.width();

    // load image to SignalProcessor-object 'grayScaleSignal'
    qImageToSignalProcessors(image,processVertical_);
    // algorithm will work on old data 'grayScaleSignal' and save
    // results in buffer-variable 'newGrayScaleSignal'
    SignalProcessor newGrayScaleSignal = grayScaleSignal;

    // iterate all lines
    for (int y = 0; y < imageHeight; y++) {
        // iterate every 'column'(=pixel) for current line
        for (int x = 0; x < imageWidth; x++) {
            SignalProcessor temp(0,true,0,255);

            // load all neighbouring pixels of current sliding window-position
            // into temporary SignalProcessor 'temp'
            for (int yOffset = -vertical_; yOffset <= vertical_; yOffset++) {
                for (int xOffset = -horizontal_; xOffset <= horizontal_; xOffset++) {
                    int yCoord = y + yOffset;
                    int xCoord = x + xOffset;

                    // if index is no valid index use the first value of
                    // current line / column
                    if (yCoord < 0) {
                        yCoord = 0;
                    }
                    if (xCoord < 0) {
                        xCoord = 0;
                    }

                    // calculate index of 1-dimensional signalprocessor by
                    // line and column index
                    int index = yCoord * imageWidth + xCoord;
                    temp.push_back(grayScaleSignal.getValueAt(index));
                }
            }

            // calculate average of 'temp' and save it as new value of current pixel
            int index = y * imageWidth + x;
            newGrayScaleSignal[index] = round(temp.analyzeSignalProcessor(AnalyzationType::AVERAGE));
        }

        // update progressbar
        unsigned int progress = round(100.0 / double(imageHeight)*y);
        setProgressBar(progress);
    }

    // overwrite old grayScaleSignal with new values
    grayScaleSignal = newGrayScaleSignal;

    // load signalProcessor back to image
    signalProcessorsToQImage(image,false);
    setAndReScalePixMapAfterModification(image);

    // set progressbar to 100 percent
    setProgressBar(100);
}

void Image::gradient(bool processVertical_, GradientType gradientType_, bool loadImageProcessorsBackToQImage_) {
    qImageToSignalProcessors(image,processVertical_);
    int imageHeight = image.height();
    int imageWidth  = image.width();

    if ( (gradientType_ == GR_NORMAL) || (gradientType_ == GR_ABS) ) {
        ModificationType modType;
        if (gradientType_ == GR_NORMAL) {
            modType = ModificationType::GRADIENT_NORMAL;
        } else {
            modType = ModificationType::GRADIENT_ABS;
        }
        if (processVertical_) {
            for (int x = 0; x < imageWidth; x++) {
                SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[x];
                tempSignalProcessor->modifySignalProcessor(modType);
                unsigned int progress = round(100.0 / double(imageWidth)*x);
                setProgressBar(progress);
            }
        } else {
            for (int y = 0; y < imageHeight; y++) {
                SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
                tempSignalProcessor->modifySignalProcessor(modType);
                unsigned int progress = round(100.0 / double(imageHeight)*y);
                setProgressBar(progress);
            }
        }

        setProgressBar(100);
        if (loadImageProcessorsBackToQImage_) {
            signalProcessorsToQImage(image,true,processVertical_);
            setAndReScalePixMapAfterModification(image);
        }
    } else if (gradientType_ == GR_MAGNITUDE) {
        gradient(false, GradientType::GR_ABS,false);
        vector<SignalProcessor>  tempHorizontalSignalProcessorVector = grayScaleSignalVector;
        gradient(true, GradientType::GR_ABS,false);
        vector<SignalProcessor>  tempVerticalSignalProcessorVector = grayScaleSignalVector;
        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth; x++) {
                SignalProcessor* tempHorizontalSignalProcessor = &tempHorizontalSignalProcessorVector[y];
                SignalProcessor* tempVerticalSignalProcessor   = &tempVerticalSignalProcessorVector  [x];
                int horizontalGradient = (*tempHorizontalSignalProcessor)[x];
                int verticalGradient   = (*tempVerticalSignalProcessor  )[y];
                int magnitudeGradient  = sqrt(horizontalGradient*horizontalGradient + verticalGradient*verticalGradient);
                grayScaleSignal[( imageWidth * y ) - 1 + x] = abs(magnitudeGradient);
            }
            unsigned int progress = round(100.0 / double(imageHeight) * y);
            setProgressBar(progress);
        }
        setProgressBar(100);
        if (loadImageProcessorsBackToQImage_) {
            signalProcessorsToQImage(image,false,false);
            setAndReScalePixMapAfterModification(image);
        }
    }

}

void Image::invert() {
    qImageToSignalProcessors(image);
    grayScaleSignal.modifySignalProcessor(ModificationType::INVERT);
    setProgressBar(100);
    signalProcessorsToQImage(image);
    setAndReScalePixMapAfterModification(image);
}

void Image::threshold(ThresholdType threshholdType_, int thresholdValue_,
                      int smallerThanThresholdValue_, int biggerThanTrashholdValue_) {
    ModificationType modType;
    if (threshholdType_ == ThresholdType::TH_NORMAL) {
        modType = ModificationType::THRESHOLD;
    } else {
        modType = ModificationType::BINARY_THRESHOLD;
    }

    qImageToSignalProcessors(image);
    grayScaleSignal.modifySignalProcessor(modType,{thresholdValue_,smallerThanThresholdValue_,biggerThanTrashholdValue_});
    setProgressBar(100);
    signalProcessorsToQImage(image);
    setAndReScalePixMapAfterModification(image);
}

void Image::qImageToSignalProcessors(QImage imageToLoadFrom_, bool loadVerticalToVector_) {
    if (imageToLoadFrom_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToLoadFrom_.height();
        unsigned int imageWidth = imageToLoadFrom_.width();
        grayScaleSignal = SignalProcessor(imageHeight*imageWidth,true,0,255);
        grayScaleSignalVector.clear();

        // create signals for every horizontal / vertical line
        if (loadVerticalToVector_) {
            for (unsigned int x = 0; x < imageWidth; x++) {
                grayScaleSignalVector.push_back(SignalProcessor(imageHeight,true,0,255));
            }
        } else {
            for (unsigned int y = 0; y < imageHeight; y++) {
                grayScaleSignalVector.push_back(SignalProcessor(imageWidth,true,0,255));
            }
        }

        for (unsigned int y = 0; y < imageHeight; y++) {
            uchar* ar = imageToLoadFrom_.scanLine(y);
            for (unsigned int x = 0; x < imageWidth; x++) {
                int index = imageWidth * y + x;

                // push values into signal of whole picture
                grayScaleSignal[index] = *ar;

                // push values into signals per horizontal / vertical lines
                if (loadVerticalToVector_) {
                    SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[x];
                    (*tempSignalProcessor)[y] = *ar;
                } else {
                    SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
                    (*tempSignalProcessor)[x] = *ar;
                }

                // increment pointer to imageData
                ar++;
            }
        }
    } else if (imageToLoadFrom_.format() == QImage::Format::Format_ARGB32) {
        // todo
    }
};

void Image::signalProcessorsToQImage(QImage& imageToWriteTo_, bool loadFromSignalProcessorVector_, bool loadVerticalFromVector_) {
    if (imageToWriteTo_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToWriteTo_.height();
        unsigned int imageWidth = imageToWriteTo_.width();
        if (grayScaleSignal.size() == imageHeight * imageWidth) {
            for (unsigned int y = 0; y < imageHeight; y++) {
                uchar* ar = imageToWriteTo_.scanLine(y);
                for (unsigned int x = 0; x < imageWidth; x++) {
                    int index = imageWidth * y + x;
                    if (loadFromSignalProcessorVector_) {
                        if (loadVerticalFromVector_) {
                            SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[x];
                            *ar = (*tempSignalProcessor)[y];
                        } else {
                            SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
                            *ar = (*tempSignalProcessor)[x];
                        }
                    } else {
                        *ar = grayScaleSignal[index];
                    }
                    ar++;
                }
            }
        } else {
            // error
        }
    } else if (imageToWriteTo_.format() == QImage::Format::Format_ARGB32) {
        // todo
    }
}

void Image::staticIncrease(int delta_) {
    qImageToSignalProcessors(image);
    grayScaleSignal.modifySignalProcessor(ModificationType::ADD,{delta_});
    setProgressBar(100);
    signalProcessorsToQImage(image);
    setAndReScalePixMapAfterModification(image);
}

/* --- auto-detect --- */
int    Image::autoDetect() {
    int imageHeight = image.height();
    int imageWidth  = image.width();

    int estimatedSize = 0;
    int average = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::AVERAGE);
    int stdDev  = grayScaleSignal.analyzeSignalProcessor(AnalyzationType::STD_DEVIATION);
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            if (checkPixelRequirements(x,y,average,stdDev)) {
                grayScaleSignal[y * image.width() + x - 1] = 0;
                estimatedSize++;
            } else {
                grayScaleSignal[y * image.width() + x - 1] = 255;
            }
        }
        unsigned int progress = round(100.0 / double(imageHeight) * y);
        setProgressBar(progress);
        }
    setProgressBar(100);
    signalProcessorsToQImage(image,false);
    setAndReScalePixMapAfterModification(image);
    return estimatedSize;
}

bool   Image::checkPixelRequirements(int x_, int y_, int average_, int standardDeviation_) {
    if (grayScaleSignal[y_ * image.width() + x_ - 1] <  average_ - standardDeviation_) {
        return true;
    }
}
