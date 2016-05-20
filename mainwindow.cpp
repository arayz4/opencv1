#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    showimg = false;
    bla = true;
}

//OPENCV FACE DETECTOR//
Mat MainWindow::detectFaceInImage(Mat &image,string &cascade_file){
    CascadeClassifier cascade;
    cascade.load(cascade_file);

    vector<Rect> faces;
    cascade.detectMultiScale(image, faces, 1.1,3,0,Size(20,20));

    for (int i = 0; i < faces.size(); i++){
        rectangle(image, Point(faces[i].x,faces[i].y),Point(faces[i].x + faces[i].width,faces[i].y + faces[i].height),Scalar(0,200,0),3,CV_AA);
    }
    return image;
}


void MainWindow::readImgWithCV()
{
    QString fileName;
    fileName = opdlg();

    //IplImage で画像をひらく
    if(!fileName.isEmpty()) {
        const char *fpath = fileName.toStdString().c_str();
        IplImage *img = cvLoadImage(fpath);

        Mat mat_img = cvarrToMat(img);

        string bundo = "E:/rnd/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";

        Mat detectFaceImage = detectFaceInImage(mat_img, bundo);

        IplImage dstImg = detectFaceImage;

        //IplImageをQImageにする
        qtimage = IplImage2QImage(&dstImg);

        cvReleaseImage(&img);

        showimg = true;

    }
}

void MainWindow::readImgWithCV_showCV()
{
    char* szWndName;
    CvCapture* capture;
    IplImage* iplimg;
    capture=cvCaptureFromCAM(0);
    if(capture==NULL){
        qDebug() << "Cant detect camera";
    }

    //IplImage *img = cvLoadImage(fpath);

    cvNamedWindow(szWndName, CV_WINDOW_AUTOSIZE);

    while(bla)
    {
        iplimg = cvQueryFrame(capture);

        Mat mat_img = cvarrToMat(iplimg);

        //string bundo = "E:/rnd/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
        string bundo = "E:/rnd/opencv/sources/data/haarcascades/haarcascade_eye.xml";

        Mat detectFaceImage = detectFaceInImage(mat_img, bundo);

        IplImage dstImg = detectFaceImage;

        cvShowImage(szWndName, &dstImg);

        if(cvWaitKey(1)>=0)
            break;
    }

    //cvShowImage(windowName,img);
    cvWaitKey(0);

    //cvReleaseImage(&img);

    cvDestroyWindow(szWndName);
    cvReleaseCapture(&capture);
}

// IplImage から QImage に変換
QImage* MainWindow::IplImage2QImage(const IplImage *iplImg)
{
    int h = iplImg->height;
    int w = iplImg->width;
    int channels = iplImg->nChannels;
    QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
    char *data = iplImg->imageData;

    for (int y = 0; y < h; y++, data += iplImg->widthStep)
    {
        for (int x = 0; x < w; x++)
        {
            char r, g, b, a = 0;
            if (channels == 1)
            {
                r = data[x * channels];
                g = data[x * channels];
                b = data[x * channels];
            }
            else if (channels == 3 || channels == 4)
            {
                r = data[x * channels + 2];
                g = data[x * channels + 1];
                b = data[x * channels];
            }

            if (channels == 4)
            {
                a = data[x * channels + 3];
                qimg->setPixel(x, y, qRgba(r, g, b, a));
            }
            else
            {
                qimg->setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qimg;
}

QString MainWindow::opdlg()
{
    QString fileName =
        QFileDialog::getOpenFileName
            (this, tr("Open Image"), ".",
             tr("JPEG (*.jpg *.jpeg)\n"
                "PNG (*.png)\n"
                "BMP (*.bmp)"));

    return fileName;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter paint;
    paint.begin(this);
    if(showimg){
        paint.drawImage(0,0,*qtimage);
    }
    paint.end();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt:: Key_O:
        {
            readImgWithCV();
            break;
        }

        case Qt::Key_P:
        {
            showimg = true;
            break;
        }

        case Qt::Key_B:
        {
            readImgWithCV_showCV();
            break;
        }

        case Qt::Key_Q:
        {
            bla = false;
            this->close();
            break;
        }
    }
}
