#include "camerarender.h"
#include "glvideowidget.h"
#include "home.h"

Home::Home(QWidget *parent) : QWidget(parent)
{
    GLVideoWidget *render = new GLVideoWidget;
    CameraRender *cameraRender = new CameraRender;
    QPushButton *openFileButton = new QPushButton("open file");
    QTextEdit *widthInput = new QTextEdit;
    QTextEdit *heightInput = new QTextEdit;


    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(render);
    topLayout->addWidget(cameraRender);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(openFileButton);
    bottomLayout->addWidget(new QLabel("width:"));
    bottomLayout->addWidget(widthInput);
    bottomLayout->addWidget(new QLabel("height:"));
    bottomLayout->addWidget(heightInput);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(bottomLayout, 1);
    setLayout(mainLayout);

    connect(openFileButton, &QPushButton::clicked, this, [=](){
        qDebug() << "openYUVButton clicked" << endl;
        QString filename = QFileDialog::getOpenFileName(this,tr("action"));
        if(filename.isEmpty()) {
            return;
        }
        qDebug() << filename << endl;
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
//        render->setYUV420pParameters(1280, 720);
//        render->setFrameData(data);

        cameraRender->render(data, 1280, 720);
    });
}
