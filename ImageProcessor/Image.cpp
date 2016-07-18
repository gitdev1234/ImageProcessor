#include "Image.h"

/* --- load / save --- */
QSize Image::loadImage(QWidget& widget_,const string& path_) {
    image = QImage(QString(path_.c_str()));

    switch(image.format()) {
        case QImage::Format_RGB32 : image.convertToFormat(QImage::Format_ARGB32); break;
        case QImage::Format_ARGB32 : ; break;
        case QImage::Format_Indexed8 : ; break;
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

void Image::smooth() {
    grayScaleSignal = grayScaleSignal + 50;
    signalProcessorsToQImage(image);
    setAndReScalePixMapAfterModification(image);
}


void Image::qImageToSignalProcessors(QImage imageToLoadFrom_) {
    if (imageToLoadFrom_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToLoadFrom_.height();
        unsigned int imageWidth = imageToLoadFrom_.width();
        grayScaleSignal = SignalProcessor(imageHeight*imageWidth,true,0,255);
        for (unsigned int y = 0; y < imageHeight; y++) {
            uchar* ar = imageToLoadFrom_.scanLine(y);
            for (unsigned int x = 0; x < imageWidth; x++) {
                int index = imageWidth * y + x;
                grayScaleSignal[index] = *ar;
                ar++;
            }
        }
    } else if (imageToLoadFrom_.format() == QImage::Format::Format_ARGB32) {
        // todo
    }
};

void Image::signalProcessorsToQImage(QImage& imageToWriteTo_) {
    if (imageToWriteTo_.format() == QImage::Format::Format_Indexed8) {
        unsigned int imageHeight = imageToWriteTo_.height();
        unsigned int imageWidth = imageToWriteTo_.width();
        if (grayScaleSignal.size() == imageHeight * imageWidth) {
            for (unsigned int y = 0; y < imageHeight; y++) {
                uchar* ar = imageToWriteTo_.scanLine(y);
                for (unsigned int x = 0; x < imageWidth; x++) {
                    int index = imageWidth * y + x;
                    *ar = grayScaleSignal[index];
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
