#include "camerarender.h"
#include "glvideowidget.h"
#include "home.h"

Home::Home(QWidget *parent) : QWidget(parent)
{
    GLVideoWidget *render = new GLVideoWidget;
    CameraRender *cameraRender = new CameraRender;
//    TaoRenderer *taoRender = new TaoRenderer;
    QPushButton *openFileButton = new QPushButton("open file");
    QTextEdit *widthInput = new QTextEdit;
    QTextEdit *heightInput = new QTextEdit;


    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(render);
    topLayout->addWidget(cameraRender);
//    topLayout->addWidget(taoRender);

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
        render->setYUV420pParameters(1280, 720);
        render->setFrameData(data);

//        cameraRender->render(data, 1280, 720);

        int width = 1280;
        int height = 720;
        YUVData m_yuvData;
        m_yuvData.Y.resize(width * height);
        memcpy(m_yuvData.Y.data(), data.data(), static_cast<size_t>(m_yuvData.Y.size()));

        m_yuvData.U.resize(width / 2 * height / 2);
//        memset(m_yuvData.U.data(), 0, static_cast<size_t>(m_yuvData.U.size()));
        memcpy(m_yuvData.U.data(), data.data() + m_yuvData.Y.size(), static_cast<size_t>(m_yuvData.U.size()));

        m_yuvData.V.resize(width / 2 * height / 2);
//        memset(m_yuvData.V.data(), 0, static_cast<size_t>(m_yuvData.V.size()));
        memcpy(m_yuvData.V.data(), data.data() + m_yuvData.Y.size() + m_yuvData.U.size(), static_cast<size_t>(m_yuvData.V.size()));

        m_yuvData.yLineSize = width;
        m_yuvData.uLineSize = width/2;
        m_yuvData.vLineSize = width/2;
        m_yuvData.height = height;

        cameraRender->updateTextureInfo(width, height, 2);
        cameraRender->updateTextureData(m_yuvData);
        cameraRender->render();
    });
}
