#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

namespace Ui {
class VideoRender;
}

class VideoRender : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    explicit VideoRender(QWidget *parent = nullptr);
    ~VideoRender();

    void save2File();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    GLuint shaderProgram;
};

#endif // VIDEORENDER_H
