#ifndef CAMERARENDER_H
#define CAMERARENDER_H

#include <QObject>
#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

const static int cBufferSize = 1024 * 768;

struct YUVData
{
    YUVData()
    {
        Y.reserve(cBufferSize);
        U.reserve(cBufferSize);
        V.reserve(cBufferSize);
    }
    QByteArray Y;
    QByteArray U;
    QByteArray V;
    int yLineSize;
    int uLineSize;
    int vLineSize;
    int height;
};
Q_DECLARE_METATYPE(YUVData);

class CameraRender : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CameraRender(QWidget *parent = nullptr);

    void render(const QByteArray &data, int w, int h);
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(const YUVData& data);
protected:
    void initTexture();
    void initShader();
    void initGeometry();

protected:
//    void bind();
//    void bindPlane(int p);
//    void initializeShader();
//    void initTextures();
    virtual void initializeGL() override;
    virtual void paintGL() override;
//    virtual void resizeGL(int w, int h) override;
//    virtual bool event(QEvent* event) override;



signals:

private:
    QOpenGLShaderProgram mProgram;
    QOpenGLTexture* mTexY = nullptr;
    QOpenGLTexture* mTexU = nullptr;
    QOpenGLTexture* mTexV = nullptr;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    bool mTextureAlloced = false;
};

#endif // CAMERARENDER_H
