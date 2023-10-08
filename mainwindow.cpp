#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Reading and loading original image
void MainWindow::on_pushButton_clicked()
{
    // Choosing the image
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath(), tr("*.bmp"));

    // Loading image
    QImage image(fileName);
    QPixmap p = QPixmap::fromImage(image);

    // Obtaing images dimensions
    int w = ui->ogPic->width();
    int h = ui->ogPic->height();

    // Placing images onto the UI
    ui->ogPic->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));
    ui->label_2->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
    ui->label_3->setPixmap(p.scaled(w,h, Qt::KeepAspectRatio));
}

// Initializing the UDP socket
void MainWindow::initSocket()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress("100.69.238.186"), 80);
}


// Calculating the image brightness change
QImage MainWindow::changeBrightness(QImage &image, int factor)
{
    double r,g,b;
    int pixels;
    unsigned int *data;

    // Obtaining the image dimensions
    pixels = image.width() * image.height();

    // Creating a byte array
    data = (unsigned int *)image.bits();

    // Changing each bits brightness
    for(int i = 0; i < pixels; i++)
    {
        // Manipulating each bits RGB color components
        r = qRed(data[i])+factor;

        // makes sure values are in the range of 0 - 255
        if(r < 0)
            r = 0;
        else if(r > 255)
            r =  255;

        g = qGreen(data[i])+factor;
        if( g < 0)
            g = 0;
        else if(g > 255)
            g =  255;

        b = qBlue(data[i])+factor;
        if(b < 0)
            b = 0;
        else if(b > 255)
            b =  255;

        data[i] = qRgb(r,g,b);

    }
    return image;
}

// Calculating the image contrast change
QImage MainWindow::changeContrast(QImage &image, int factor)
{
    double r,g,b, contrast;
    int pixels;
    unsigned int *data;

    contrast = (100.0+factor)/100;

    // Obtaining the image dimensions
    pixels = image.width() * image.height();

    // Creating a byte array
    data = (unsigned int *)image.bits();

    // Changing each bits brightness
    for(int i = 0; i < pixels; i++)
    {
        // Manipulating each bits RGB color components
        r = (contrast*(qRed(data[i]))) + factor;

        // makes sure values are in the range of 0 - 255
        if(r < 0)
            r = 0;
        else if(r > 255)
            r =  255;

        g = (contrast*(qGreen(data[i]))) + factor;
        if(g < 0)
            g = 0;
        else if(g > 255)
            g =  255;

        b = (contrast*(qBlue(data[i]))) + factor;
        if(b < 0)
            b = 0;
        else if(b > 255)
            b =  255;

        data[i] = qRgba(r,g,b, qAlpha(data[i]));

    }
    return image;
}

// Brightness slider
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QImage image;
    QPixmap pix;
    QByteArray Data;

    image = ui->ogPic->pixmap()->toImage();
    image = changeBrightness(image, value);
    pix = QPixmap::fromImage(image);
    ui->ogPic->setPixmap(pix);
    ui->label_3->setPixmap(pix);

    // Writing the slider value  as a datagram to the board
    Data.append(QString::number(value));
    //qDebug() << value;
    socket->writeDatagram(Data, QHostAddress("100.69.238.186"), 80);

}


// Contrast slider
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    QImage image;
    QPixmap pix;
    QByteArray Data;

    image = ui->label_2->pixmap()->toImage();
    image = changeContrast(image, value);
    pix = QPixmap::fromImage(image);
    ui->label_2->setPixmap(pix);
    ui->label_3->setPixmap(pix);

    // Writing the slider value  as a datagram to the board
    Data.append(QString::number(value));
    //qDebug() << value;
    socket->writeDatagram(Data, QHostAddress("100.69.238.186"), 80);

}

// Saving the modified image
void MainWindow::on_pushButton_2_clicked()
{

    QString fileName;

    // Saving the modified image under a pre-described name
    fileName = QFileDialog::getSaveFileName(this, "Image", "modifiedImage.bmp", "*.bmp");
    ui->label_3->pixmap()->save(fileName);
}

