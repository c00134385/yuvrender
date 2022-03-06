#include "camerarender.h"

#include <QPalette>
#include <QDebug>
#include <QOpenGLPixelTransferOptions>

CameraRender::CameraRender(QWidget *parent) : QOpenGLWidget(parent)
{
    //    setStyleSheet("background-color:#ffff00;");

    //    QPalette pal = palette();
    //    pal.setColor(QPalette::Background, Qt::green);
    //    setAutoFillBackground(true);
    //    setPalette(pal);

    //    resize(100, 100);
    //    init();
}

void CameraRender::render(const QByteArray &data, int w, int h) {
//    qDebug() << data <<endl;
    qDebug() << data.size() <<endl;
    YUVData m_yuvData;
    QSize imgSize(w, h);
    m_yuvData.Y.resize(w * h);
    memcpy(m_yuvData.Y.data(), data.data(), static_cast<size_t>(m_yuvData.Y.size()));
    m_yuvData.U.resize(imgSize.width() * imgSize.height() / 4);
    memset(m_yuvData.U.data(), 0, static_cast<size_t>(m_yuvData.U.size()));
    m_yuvData.V.resize(imgSize.width() * imgSize.height() / 4);
    memset(m_yuvData.V.data(), 0, static_cast<size_t>(m_yuvData.V.size()));
    m_yuvData.yLineSize = imgSize.width();
    m_yuvData.uLineSize = imgSize.width() / 2;
    m_yuvData.vLineSize = imgSize.width() / 2;
    m_yuvData.height = imgSize.height();


    updateTextureInfo(w, h, 0);
    updateTextureData(m_yuvData);
}

void CameraRender::updateTextureInfo(int width, int height, int format) {
    mTexY->setSize(width, height);
    mTexY->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    mTexU->setSize(width / 2, height / 2);
    mTexU->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    mTexV->setSize(width / 2, height / 2);
    mTexV->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
}

void CameraRender::updateTextureData(const YUVData& data) {
    QOpenGLPixelTransferOptions options;
    options.setImageHeight(data.height);
    options.setRowLength(data.yLineSize);
    mTexY->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.Y.data(), &options);
    options.setRowLength(data.uLineSize);
    mTexU->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.U.data(), &options);
    options.setRowLength(data.vLineSize);
    mTexV->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.V.data(), &options);

    mTextureAlloced = true;
    update();
}

void CameraRender::initTexture() {
    // yuv420p
    mTexY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexY->setFormat(QOpenGLTexture::LuminanceFormat);
    mTexY->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexY->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexY->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexU->setFormat(QOpenGLTexture::LuminanceFormat);
    mTexU->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexU->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexU->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexV->setFormat(QOpenGLTexture::LuminanceFormat);
    mTexV->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexV->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexV->setWrapMode(QOpenGLTexture::ClampToEdge);
}
void CameraRender::initShader() {
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.vsh"))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.fsh"))
    {
        qWarning() << " add fragment shader file failed.";
        return;
    }
    mProgram.bindAttributeLocation("qt_Vertex", 0);
    mProgram.bindAttributeLocation("texCoord", 1);
    mProgram.link();
    mProgram.bind();
}
void CameraRender::initGeometry() {
    mVertices << QVector3D(-1, 1, 0.0f) << QVector3D(1, 1, 0.0f) << QVector3D(1, -1, 0.0f) << QVector3D(-1, -1, 0.0f);
    mTexcoords << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
}

void CameraRender::initializeGL() {
    qDebug() << "initializeGL" << this << endl;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();
    initTexture();
    initGeometry();
}

void CameraRender::paintGL() {
    qDebug() << "paintGL" << this << endl;
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!mTextureAlloced)
    {
        return;
    }

    qDebug() << "paintGL:" << mTextureAlloced << endl;

    mProgram.bind();

    int mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    int mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    int mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    int mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    int mTexCoordHandle = mProgram.attributeLocation("texCoord");
    //顶点
    mProgram.enableAttributeArray(mVerticesHandle);
    mProgram.setAttributeArray(mVerticesHandle, mVertices.constData());

    //纹理坐标
    mProgram.enableAttributeArray(mTexCoordHandle);
    mProgram.setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    mProgram.setUniformValue(mModelMatHandle, mModelMatrix);
    mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    mProgram.setUniformValue(mProjectMatHandle, mProjectionMatrix);

    // pixFmt
    mProgram.setUniformValue("pixFmt", 0);

    //纹理
    // Y
    glActiveTexture(GL_TEXTURE0);
    mTexY->bind();

    // U
    glActiveTexture(GL_TEXTURE1);
    mTexU->bind();

    // V
    glActiveTexture(GL_TEXTURE2);
    mTexV->bind();

    mProgram.setUniformValue("tex_y", 0);
    mProgram.setUniformValue("tex_u", 1);
    mProgram.setUniformValue("tex_v", 2);

    glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size());

    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();

//    mTextureAlloced = false;
}
