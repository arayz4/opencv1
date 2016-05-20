#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QtGui>
#include <QImage>

//OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

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
    QImage* IplImage2QImage(const IplImage *iplImg);

    Mat detectFaceInImage(Mat &image,string &cascade_file);

    bool showimg;

    bool bla;

private:
    QImage* qtimage;

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent* ev);
};

#endif // MAINWINDOW_H
