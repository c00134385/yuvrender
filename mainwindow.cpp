#include "glvideowidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>


#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#define printf qDebug

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    QFile f(":/1280x720.yuv");
    f.open(QIODevice::ReadOnly);
    QByteArray data(f.readAll());
    qDebug("data size: %d", data.size());
    const int w = 1280, h = 720;
    ui->openGLWidget_2->setYUV420pParameters(w, h); //call once
    ui->openGLWidget_2->setFrameData(data);
    */


    //    QFile f(":/860x484.yuv");
    //    f.open(QIODevice::ReadOnly);
    //    QByteArray data(f.readAll());
    //    qDebug("data size: %d", data.size());
    //    const int w = 860, h = 484;
    //    ui->openGLWidget_2->setYUV420pParameters(w, h); //call once
    //    ui->openGLWidget_2->setFrameData(data);

//    on_openButton_clicked();

//    QImage img = QImage(":/test.png").convertToFormat(QImage::Format_RGB888);
//    ui->openGLWidget_2->setQImageParameters(img.format(), img.width(), img.height(), img.bytesPerLine()); //call once
//    ui->openGLWidget_2->setImage(img);


//    if (!a.arguments().contains("-img")) {
//        QFile f(":/1280x720.yuv");
//        f.open(QIODevice::ReadOnly);
//        QByteArray data(f.readAll());
//        qDebug("data size: %d", data.size());
//        const int w = 1280, h = 720;
//        glw.setYUV420pParameters(w, h); //call once
//        glw.setFrameData(data);
//    } else {
//        QImage img = QImage(":/test.png").convertToFormat(QImage::Format_RGB888);
//        glw.setQImageParameters(img.format(), img.width(), img.height(), img.bytesPerLine()); //call once
//        glw.setImage(img);
//    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void saveImage1(QImage& image) {
//    QString fileName = QDir::currentPath().append(QDateTime::currentDateTime().toString("/yyyy-MM-dd hh-mm-ss-zzz") + ".png");
//    printf("fileName:%s", fileName.toStdString().c_str());
    QString picPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString fileName = picPath + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz") + ".png";
    image.save(fileName, "png");
}

void savePixmap(QPixmap& pixmap) {
//    QString fileName = QDir::currentPath().append(QDateTime::currentDateTime().toString("/yyyy-MM-dd hh-mm-ss-zzz") + ".png");
//    printf("fileName:%s", fileName.toStdString().c_str());
    QString picPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString fileName = picPath + QDir::separator() + QDateTime::currentDateTime().toString("Pixmap_yyyy-MM-dd hh-mm-ss-zzz") + ".png";
    pixmap.save(fileName, "png");
}

void MainWindow::on_saveButton_clicked()
{
//    ui->openGLWidget->save2File();
//    QImage img = ui->openGLWidget_2->grabFrameBuffer();
//    saveImage1(img);

//    QPixmap p = ui->openGLWidget_2->renderPixmap();
//    savePixmap(p);
}

void MainWindow::on_openButton_clicked()
{
    const int w = 858, h = 482;
    const int size = w*h*3/2;

    QFile f("D:/vs/yuvrender/test.yuv");
    f.open(QIODevice::ReadOnly);

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int offset = qrand() % 400;    //产生5以内的随机数
    f.seek(227*size);
    printf("offset:%d", offset);

    QByteArray data = f.read(size);
    qDebug("data size: %d", data.size());
//    ui->openGLWidget_2->setYUV420pParameters(w, h); //call once
//    ui->openGLWidget_2->setFrameData(data);
}

QByteArray scaleYuv(QByteArray& input, int w1, int h1, int w2, int h2) {
    char* buff1 = input.data();

    const int size2 = w2*h2*3/2;
    QByteArray newYuv(size2, Qt::Initialization::Uninitialized);
    char* buff2 = newYuv.data();
    memset(buff2, 0, size2);

    int stride = (w1 > w2)? w2 : w1;
    int height = (h1 > h2)? h2 : h1;
    // Y
    char* src = buff1;
    char* dst = buff2;
    for(int i = 0; i < height; i++) {
        memcpy(dst + i * w2, src + i * w1, stride);
    }

    // U
    src = buff1 + w1 * h1;
    dst = buff2 + w2 * h2;
    for(int i = 0; i < height/2; i++) {
        memcpy(dst + i * w2/2, src + i * w1/2, stride/2);
    }

    // V
    src = buff1 + w1 * h1 + w1 * h1 / 4;
    dst = buff2 + w2 * h2 + w2 * h2 / 4;

    for(int i = 0; i < height/2; i++) {
        memcpy(dst + i * w2/2, src + i * w1/2, stride/2);
    }
    return newYuv;
}

void saveYuv(QByteArray& data) {
    QString picPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString fileName = picPath + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz") + ".yuv";
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    f.write(data.data(), data.size());
}

QByteArray readYuv() {
    const int w = 858, h = 482;
    const int size = w*h*3/2;

    QFile f("D:/vs/yuvrender/test.yuv");
    f.open(QIODevice::ReadOnly);

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int offset = qrand() % 400;    //产生5以内的随机数
    f.seek(227*size);
    printf("offset:%d", offset);

    char buff[size];
    int readSize = f.read(buff, size);
    printf("readSize:%d", readSize);

//    const int w1 = 860, h1 = 484;
//    const int size1 = w1*h1*3/2;
//    QByteArray newYuv(size1, Qt::Initialization::Uninitialized);
//    printf("newYuv Size:%d", newYuv.size());

//    char* buff1 = newYuv.data();
//    memset(buff1, 0, size1);

//    // Y
//    char* dst = buff1;
//    char* src = buff;
////    memcpy(dst, src, size);
//    for(int i = 0; i < h; i++) {
//        memcpy(dst + i * w1, src + i * w, w);
//    }

//    // U
//    dst = buff1 + w1 * h1;
//    src = buff + w * h;
//    for(int i = 0; i < h/2; i++) {
//        memcpy(dst + i * w1/2, src + i * w/2, w/2);
//    }

//    // V
//    dst = buff1 + w1 * h1 + w1 * h1 / 4;
//    src = buff + w * h + w * h / 4;
//    for(int i = 0; i < h/2; i++) {
//        memcpy(dst + i * w1/2, src + i * w/2, w/2);
//    }

    f.close();
    return QByteArray(buff);
}


char* tempData = NULL;

void renderYuv(GLVideoWidget* opengl, QByteArray& data, int w, int h) {
    if(tempData == NULL) {
        tempData = new char[data.size()];
    }

    memcpy(tempData, data.data(), data.size());

    opengl->setYUV420pParameters(w, h); //call once
    opengl->setFrameData(data);
}


int count = 0;

void MainWindow::on_saveYuv_clicked()
{
//    const int w = 860, h = 484;
//    QByteArray data = readYuv();
//    QByteArray x = scaleYuv(data, 858, 482, w, h);

//    renderYuv(ui->openGLWidget_2, x, w, h);
//    ui->openGLWidget_2->setYUV420pParameters(w, h); //call once
//    ui->openGLWidget_2->setFrameData(data);

//    Sleep(500);
//    saveYuv(x);


    QFile f("D:/vs/yuvrender/test.yuv");
    f.open(QIODevice::ReadOnly);

    const int w = 858, h = 482;

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int offset = qrand() % 400;    //产生5以内的随机数
    f.seek(858*482*3/2 * (count));
    count += 5;
    printf("offset:%d", offset);

    QByteArray data(858*482*3/2, Qt::Uninitialized);
    int readSize = f.read(data.data(), 858*482*3/2);
    while(readSize <= 0) {
        count = 0;
        f.seek(858*482*3/2 * (count));
        readSize = f.read(data.data(), 858*482*3/2);
    }




    printf("count:%d readSize:%d", count, readSize);

    int ww = (w + 64) / 4 * 4;
    int hh = (h + 64) / 4 * 4;

    QByteArray newData = scaleYuv(data, w, h, ww, hh);
//    ui->openGLWidget_2->setYUV420pParameters(ww, hh); //call once
//    ui->openGLWidget_2->setFrameData(newData);


//    ui->openGLWidget_2->setYUV420pParameters(w, h); //call once
//    ui->openGLWidget_2->setFrameData(data);
    f.close();
}







