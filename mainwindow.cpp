#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myudp.h"

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
    socket->bind(QHostAddress("100.69.238.184"), 80);

    //connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
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
    QString s, temp1, temp2;

    image = ui->ogPic->pixmap()->toImage();
    image = changeBrightness(image, value);
    pix = QPixmap::fromImage(image);
    ui->ogPic->setPixmap(pix);
    ui->label_3->setPixmap(pix);

    // Writing the slider value  as a datagram to the board

    // Converting the integer to string for the byte array
    s = QString::number(value);

    // Copying the last 2 bytes of the array so they are not lost when inserting new values
    temp1 = Data[2];
    temp2 = Data[3];

    // Insert string value into the first two bytes
    Data.insert(0, s);

    // Replacing the last 2 bytes with the original values
    Data.insert(2, temp1);
    Data.insert(3, temp2);


    // If the value is a single digit number x, format to be 0X
    if(value < 10)
    {
        Data.insert(0, "0");

        // Replacing the last 2 bytes with the original values
        Data.insert(2, temp1);
        Data.insert(3, temp2);
    }

    // Writing to the board
    socket->writeDatagram(Data, QHostAddress("100.69.238.184"), 80);
    Data.resize(4);
    qDebug() << Data;
}


// Contrast slider
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    QImage image;
    QPixmap pix;
    QString s;

    image = ui->label_2->pixmap()->toImage();
    image = changeContrast(image, value);
    pix = QPixmap::fromImage(image);
    ui->label_2->setPixmap(pix);
    ui->label_3->setPixmap(pix);

    // Converting the integer to string for byte array
    s = QString::number(value);

    // Insert string value into the last two bytes
    Data.insert(2,s);

    // If the value is a single digit number X, format to be 0X
    if(value < 10)
    {
        Data.insert(2, "0");
    }

    // Writing the slider value  as a datagram to the board
    socket->writeDatagram(Data, QHostAddress("100.69.238.184"), 80);
    Data.resize(4);
    qDebug() << Data;

}


// Saving the modified image
void MainWindow::on_pushButton_2_clicked()
{

    QString fileName;

    // Saving the modified image under a pre-described name
    fileName = QFileDialog::getSaveFileName(this, "Image", "modifiedImage.bmp", "*.bmp");
    ui->label_3->pixmap()->save(fileName);
}

