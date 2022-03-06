#include "videorender.h"
#include "ui_videorender.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QMatrix>
#include <QStandardPaths>

static GLuint VBO, VAO;
static const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";
static const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

VideoRender::VideoRender(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

VideoRender::~VideoRender()
{
//    delete ui;
}

void saveImage(QImage& image) {
//    QString fileName = QDir::currentPath().append(QDateTime::currentDateTime().toString("/yyyy-MM-dd hh-mm-ss-zzz") + ".png");
//    printf("fileName:%s", fileName.toStdString().c_str());
    QString picPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString fileName = picPath + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz") + ".png";
    image.save(fileName, "png");
}

void VideoRender::save2File() {
    GLint m_viewport[4];
    glGetIntegerv( GL_VIEWPORT, m_viewport );

    int width  = m_viewport[2];
    int height =  m_viewport[3];

    qDebug("chengjl %d %d %d %d", m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    qDebug("chengjl %d x %d", width, height);

//    GLuint texture;
//    glGenTextures( 1, &texture );

//    glBindTexture(GL_TEXTURE_2D, texture);

//    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_viewport[0],
//                     m_viewport[1], m_viewport[2], m_viewport[3],0);

    /// READ THE CONTENT FROM THE FBO
//       glReadBuffer(GL_COLOR_ATTACHMENT0);
//    glReadBuffer(GL_FRONT);
//       glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    unsigned char   *pixels = (unsigned char*)malloc(width*height*3);

    /// READ THE CONTENT FROM THE FBO
//    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);


    QImage img = grabFramebuffer();
    saveImage(img);

//    QPixmap p;
//    QImage img;
//    QImage qimg((uchar*)pixels, width, height, QImage::Format_BGR888);

//    saveImage(qimg);
//    QImage qimg((uchar*)ImgData, ViewPort[2], ViewPort[3], QImage::Format_BGR888);
//    qimg = qimg.mirrored();
//    image = qimg.copy();
    free(pixels);

    // now when framebuffer is copied, i need to link a
    //pointer to a raw texture data:
//    glPixelStorei(GL_PACK_ALIGNMENT, 1);
//    BYTE *raw_img = (BYTE*) malloc(sizeof(BYTE) * width * height * 3);
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_img);



//    // save using, for example libpng
//    free(raw_img);
}

void VideoRender::initializeGL() {
    //着色器部分
    this->initializeOpenGLFunctions();
    //core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //VAO，VBO数据部分
    GLfloat vertices[] = {
            0.0f, -0.5f, 0.0f, // left
             0.5f, -0.5f, 0.0f, // right
             0.0f,  0.5f, 0.0f  // top
    };


    glGenVertexArrays(1, &VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void VideoRender::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    qDebug("resizeGL chengjl %d x %d", w, h);
}
void VideoRender::paintGL() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}
