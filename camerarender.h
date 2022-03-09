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

const static int cBufferSize = 1280 * 720;

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
    ~CameraRender();

    void init();
    void paint();
    void resize(int width, int height);
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(const YUVData& data);


    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void render();

protected:
    void initTexture();
    void initShader();
    void initGeometry();

signals:

private:
    QOpenGLShaderProgram mProgram;
    QOpenGLTexture* mTexY = nullptr;
    QOpenGLTexture* mTexU = nullptr;
    QOpenGLTexture* mTexV = nullptr;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mTexCoordHandle;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;
};

#endif // CAMERARENDER_H
