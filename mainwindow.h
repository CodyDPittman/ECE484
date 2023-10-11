#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QGraphicsEffect>
#include <QShowEvent>
#include <QtNetwork/QUdpSocket>
#include <QByteArrayList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QUdpSocket * socket;
    QByteArray Data = QByteArray("0000",4);;

    QImage changeBrightness(QImage &image, int factor);
    QImage changeContrast(QImage &image, int factor);
    void initSocket();

public slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    int min = 0;
    int max = 255;
};
#endif // MAINWINDOW_H
