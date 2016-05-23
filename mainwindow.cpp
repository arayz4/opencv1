#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    showimg = false;
    bla = true;

    kinect = nullptr;

    hResult = S_OK;

    kineReady = false;
    eyetrack = false;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(titi()));
    timer->start(10);  // 100ms
}

void MainWindow::titi()
{
    if(kineReady){
        kine02();
    }
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

void MainWindow::webcampaint()
{
    capture=cvCaptureFromCAM(0);
    if(capture==NULL){
        qDebug() << "Cant detect camera";
    }else{
    }
}

void MainWindow::stereoma()
{
    IplImage *img_l = cvLoadImage("C:/Users/arayz/Pictures/stereo/l.png");
    IplImage *img_r = cvLoadImage("C:/Users/arayz/Pictures/stereo/r.png");

    Mat mat_img_l = cvarrToMat(img_l);
    Mat mat_img_r = cvarrToMat(img_r);

    _OutputArray *dess;
    dess = new _OutputArray;

    qDebug() << "temae";

    StereoBM *stebm;
    stebm->compute(mat_img_l,mat_img_r,*dess);

    //StereoBM::compute(mat_img_l,mat_img_r,*dess);

    qDebug() << "computed";

    //Mat mat_result = dess.getMat();



}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter paint;
    paint.begin(this);
    if(showimg){
        iplimg = cvQueryFrame(capture);
        qtimage = IplImage2QImage(iplimg);
        paint.drawImage(0,0,*qtimage);
    }
    paint.end();
}

void MainWindow::kinect1()
{
    GetDefaultKinectSensor(&kinect);

    HRESULT hResult = S_OK;

    if ( hResult != kinect->Open()){qDebug() << "not open"; return;}

    IColorFrameSource* pColorSource;  //……1
    if ( hResult != kinect->get_ColorFrameSource(&pColorSource)){qDebug() << "color frame source error"; return;}

    IColorFrameReader* pColorReader;  //……2
    if ( hResult != pColorSource->OpenReader( &pColorReader )){qDebug() << "open reader error"; return;}

    // Frame
    IColorFrame* pColorFrame=nullptr;  //……4
    if (hResult != pColorReader->AcquireLatestFrame( &pColorFrame )){qDebug() << "frame get error"; return;}

    int width, height;
    IFrameDescription *finfo;
    if (hResult != pColorSource->get_FrameDescription(&finfo)){qDebug() << "Info get error"; return;}
    qDebug() << "1";
    finfo->get_Width(&width);
    qDebug() << "2";
    finfo->get_Height(&height);

    if(width < 1 || height < 1)
    {return;}

    qDebug() << width;
    qDebug() << height;


    unsigned int bufferSize = width * height * 4 * sizeof( unsigned char );  //……2
    cv::Mat bufferMat( height, width, CV_8UC4 );  //……3

    cv::namedWindow( "Color" );

    pColorFrame->CopyConvertedFrameDataToArray( bufferSize, reinterpret_cast<BYTE*>( bufferMat.data ), ColorImageFormat_Bgra );

    cv::Mat colorMat( height / 2, width / 2, CV_8UC4 );  //……3
    cv::resize( bufferMat, colorMat, cv::Size(), 0.5, 0.5 );

    // Show Window
    cv::imshow( "Color", colorMat );
    qDebug() << "4";

}

void MainWindow::kinect2()
{
    if ( kinect->get_IsOpen(false)){
        kinect->Close();
        kinect->Release();
    }
}

void MainWindow::kine01()// once
{
    if ( hResult != GetDefaultKinectSensor(&kinect)){qDebug() << "first fail";return;}
    if ( hResult != kinect->Open()){qDebug() << "not open"; return;}
    kineReady = true;
}

void MainWindow::kine02()
{
    IColorFrameSource* pColorSource;  //……1
    if ( hResult != kinect->get_ColorFrameSource(&pColorSource)){return;}

    IColorFrameReader* pColorReader;  //……2
    if ( hResult != pColorSource->OpenReader( &pColorReader )){return;}

    // Frame
    IColorFrame* pColorFrame=nullptr;  //……4
    if (hResult != pColorReader->AcquireLatestFrame( &pColorFrame )){return;}

    int width, height;
    IFrameDescription *finfo;
    if (hResult != pColorSource->get_FrameDescription(&finfo)){return;}
    finfo->get_Width(&width);
    finfo->get_Height(&height);

    if(width < 1 || height < 1)
    {return;}

    unsigned int bufferSize = width * height * 4 * sizeof( unsigned char );  //……2
    cv::Mat bufferMat( height, width, CV_8UC4 );  //……3

    cv::namedWindow( "Color" );

    pColorFrame->CopyConvertedFrameDataToArray( bufferSize, reinterpret_cast<BYTE*>( bufferMat.data ), ColorImageFormat_Bgra );

    cv::Mat colorMat( height / 2, width / 2, CV_8UC4 );  //……3
    cv::resize( bufferMat, colorMat, cv::Size(), 0.5, 0.5 );

    if (eyetrack){
        string bundo = "E:/rnd/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";

        Mat detectFaceImage = detectFaceInImage(colorMat, bundo);

        pColorSource->Release();
        pColorReader->Release();
        pColorFrame->Release();

        cv::imshow( "Color", detectFaceImage);

        return;
    }

    pColorSource->Release();
    pColorReader->Release();
    pColorFrame->Release();

    // Show Window

    cv::imshow( "Color", colorMat );

    //qDebug() << "renew";
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

        case Qt::Key_W:
        {
            webcampaint();
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

        case Qt::Key_S:
        {
            stereoma();
            break;
        }

        case Qt::Key_K:
        {
            kinect1();
            break;
        }
        case Qt::Key_L:
        {
            kinect2();
            break;
        }

        case Qt::Key_Z:
        {
            kine01();
            break;
        }
        case Qt::Key_X:
        {
            kineReady = false;
            kinect->Close();
            kinect->Release();
            break;
        }
        case Qt::Key_C:
        {
            if(eyetrack){
                eyetrack = false;
            }else{
                eyetrack = true;
            }
            break;
        }
    }
}
