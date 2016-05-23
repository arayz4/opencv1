#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QtGui>
#include <QImage>

#include <QTimer>

//OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

//Kinect
#include <Kinect.h>

//GUI
#include <QSpinBox>
#include <QVBoxLayout>

using namespace std;
using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    QString opdlg();
    void readImgWithCV();
    void readImgWithCV_showCV();
    void webcampaint();
    QImage* IplImage2QImage(const IplImage *iplImg);

    Mat detectFaceInImage(Mat &image,string &cascade_file);

    void stereoma();

    bool showimg;

    bool bla;

    //webcam
    CvCapture* capture;

    IplImage* iplimg;

    //Kinect
    IKinectSensor* kinect;
    void kinect1();
    void kinect2();

    void kine01();
    void kine02();
    HRESULT hResult;

    bool kineReady;

    bool eyetrack;

    //GUI
    QSpinBox *sld1;
    QSpinBox *sld2;
    QSpinBox *sld3;

    QWidget *mainwid;
    QVBoxLayout *vlay;

    //Disparity
    Mat imgLeft;
    Mat imgRight;
    Mat imgDisparity16S;
    Mat imgDisparity8U;

    bool yomi;

public slots:
    void titi();
    void slid();

private:
    QImage* qtimage;

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent* ev);
};

#endif // MAINWINDOW_H
