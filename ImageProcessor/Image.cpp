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

/* --- modify image --- */
void Image::modifyRGB(signedRGBDelta delta_) {

    unsigned int imageHeight = image.height();
    for (unsigned int y = 0; y < imageHeight; y++) {
        QRgb *scanLine = (QRgb *) image.scanLine(y);
        for (unsigned int x = 0; x < image.width(); x++) {
            // line[x] has an individual pixel
            QColor color = scanLine[x];

            int newRed = (color.red() + delta_.red);
            if (newRed > 255) {
                newRed = 255;
            }
            if (newRed < 0 ) {
                newRed = 0;
            }
            color.setRed(newRed);

            int newGreen = (color.green() + delta_.green);
            if (newGreen > 255) {
                newGreen = 255;
            }
            if (newGreen < 0 ) {
                newGreen = 0;
            }
            color.setGreen(newGreen);

            int newBlue = (color.blue() + delta_.blue);
            if (newBlue > 255) {
                newBlue = 255;
            }
            if (newBlue < 0 ) {
                newBlue = 0;
            }
            color.setBlue(newBlue);

            scanLine[x] = color.rgb();
        }
        unsigned int progress = round(100.0 / double(imageHeight)*y);
        setProgressBar(progress);
    }
    setProgressBar(100);
    setAndReScalePixMapAfterModification(image);


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
    QFileDialog dialog;
    QString path = dialog.getSaveFileName();
    if (image.format() == QImage::Format_Indexed8) {
        grayScaleSignal.calcHistogram(path.toStdString());
    } else if (image.format() == QImage::Format_ARGB32) {
        //TODO
    }
}

QColor Image::getMaxRGB() {
    int maxRed = 0;
    int maxGreen = 0;
    int maxBlue = 0;
    unsigned int imageHeight = image.height();
    for (unsigned int y = 0; y < imageHeight; y++) {
        QRgb *scanLine = (QRgb *) image.scanLine(y);
        for (unsigned int x = 0; x < image.width(); x++) {
            QColor color = scanLine[x];
            if (color.red() > maxRed) {
                maxRed = color.red();
            }
            if (color.green() > maxGreen) {
                maxGreen = color.green();
            }
            if (color.blue() > maxBlue) {
                maxBlue = color.blue();
            }
        }
        unsigned int progress = round(100.0 / double(imageHeight)*y);
        setProgressBar(progress);
    }
    QColor maxColors(maxRed,maxGreen,maxBlue);
    setProgressBar(100);
    return maxColors;
}

void Image::setProgressBar(unsigned int percentage_) {
    progressBar->setValue(percentage_);
}

void Image::todo() {
    unsigned int imageHeight = image.height();
    for (unsigned int y = 0; y < imageHeight; y++) {
        QRgb *scanLine = (QRgb *) image.scanLine(y);
        uchar* ar = image.scanLine(y);
        vector<int> array;
        int width = image.width();
        cout << "[";
        for (unsigned int x = 0; x < width*4; x++) {
            cout << int(*ar) << ",";
            array.push_back(*ar);
            ar++;
        }
        cout << "]" << endl;
    }
}

void Image::smooth(int horizontal_,int vertical_) {
    qImageToSignalProcessors(image);
    int imageHeight = image.height();
    for (int y = 0; y < imageHeight; y++) {
        SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
        tempSignalProcessor->modifySignalProcessor(ModificationType::MOVING_AVERAGE,{horizontal_});
        unsigned int progress = round(100.0 / double(imageHeight)*y);
        setProgressBar(progress);
    }
    setProgressBar(100);
    // grayScaleSignal = grayScaleSignal + 50;
    signalProcessorsToQImage(image,true);
    setAndReScalePixMapAfterModification(image);
}

void Image::qImageToSignalProcessors(QImage imageToLoadFrom_) {
    if (imageToLoadFrom_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToLoadFrom_.height();
        unsigned int imageWidth = imageToLoadFrom_.width();
        grayScaleSignal = SignalProcessor(imageHeight*imageWidth,true,0,255);
        grayScaleSignalVector.clear();
        for (unsigned int y = 0; y < imageHeight; y++) {
            uchar* ar = imageToLoadFrom_.scanLine(y);
            grayScaleSignalVector.push_back(SignalProcessor(imageHeight*imageWidth,true,0,255));
            for (unsigned int x = 0; x < imageWidth; x++) {
                int index = imageWidth * y + x;
                grayScaleSignal[index] = *ar;
                SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
                (*tempSignalProcessor)[index] = *ar;
                ar++;
            }
        }
    } else if (imageToLoadFrom_.format() == QImage::Format::Format_ARGB32) {
        // todo
    }
};

void Image::signalProcessorsToQImage(QImage& imageToWriteTo_,bool loadFromSignalProcessorVector_) {
    if (imageToWriteTo_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToWriteTo_.height();
        unsigned int imageWidth = imageToWriteTo_.width();
        if (grayScaleSignal.size() == imageHeight * imageWidth) {
            for (unsigned int y = 0; y < imageHeight; y++) {
                uchar* ar = imageToWriteTo_.scanLine(y);
                for (unsigned int x = 0; x < imageWidth; x++) {
                    int index = imageWidth * y + x;
                    if (loadFromSignalProcessorVector_) {
                        SignalProcessor* tempSignalProcessor = &grayScaleSignalVector[y];
                        *ar = (*tempSignalProcessor)[index];
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
