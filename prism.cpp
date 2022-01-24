#include "prism.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <QTime>

int NumVert, k;

float x_c = 1.0;
float y_c = 1.0;

QVector<QVector3D> points;
QVector<QVector3D> cyl_vertices;

float GetKoef(int n)
{
    float koef;
    switch (n) {
        case 0: koef = 1; NumVert = 4; break;
        case 1: koef = 1.2; NumVert = 10; break;
        case 2: koef = 1.5; NumVert = 20; break;
        case 3: koef = 1.8; NumVert = 35; break;
        case 4: koef = 2; NumVert = 50; break;
        case 5: koef = 2.5; NumVert = 60; break;
    }
    return koef;
}
// Создание вершин усеченного конуса
void GenVertices(int n, float x_, float y_)
{
    float koef = GetKoef(n);
    if(cyl_vertices.size()!=0) {
        cyl_vertices.clear();
        points.clear();
    }
    cyl_vertices.resize(NumVert*2);
    float angle = 0;
    float inc = 360/NumVert;

    for(int i = 0; i < NumVert; i++){
        float x = cos(angle*M_PI/180);
        float y = sin(angle*M_PI/180);
        cyl_vertices[i] = QVector3D(1/koef*x+x_, 1/koef*y+y_, 1.0f);
        cyl_vertices[i+NumVert] = QVector3D(x+x_, y+y_, -1.0f);
        angle += inc;
    }
}
// Создание треугольника для отрисовки
void GenTriangle(int a, int b, int c)
{
    points.push_back(cyl_vertices[a]);
    points.push_back(cyl_vertices[b]);
    points.push_back(cyl_vertices[c]);
}
// Создание массива треугольников для отрисовки
void GenTriangles()
{
    GenTriangle(0, NumVert-1,NumVert*2-1);
    GenTriangle(NumVert, 0, NumVert*2-1);
    for(int i = 0; i < NumVert-1; i++)
        GenTriangle(0, i+1, i);
    for(int i = 0; i < NumVert-1; i++)
        GenTriangle(NumVert, NumVert+i+1, NumVert+i);
    for(int i = 0; i < NumVert-1; i++){
        GenTriangle(i,NumVert+i,i+1);
        GenTriangle(i+1,NumVert+i,i+1+NumVert);
    }
}
// Получение нормали плоскости
QVector3D GetNormal(QVector3D x0, QVector3D x1, QVector3D x2)
{
    QVector3D v0 = x0 - x2;
    QVector3D v1 = x1 - x2;

    QVector3D a = QVector3D::crossProduct(v0,v1);
    a.normalize();
    return a;
}
// конструктор

Prism::Prism(QWidget* prt)
    : QGLWidget(prt), n2_(0), scale_(0.25), x_c(1.0), y_c(1.0)
{
    QGLFormat frmt; // создать формат по умолчанию
    frmt.setDoubleBuffer(false); // задать простую буферизацию
    setFormat(frmt);
    // установка таймера
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    QTime time = QTime::currentTime();
    lastFrame_ = static_cast<float>(QTime(0, 0, 0).msecsTo(time));
    // установка начальных параметров
    for(int i = 0; i < 3; i++) params_.push_back(0.0); // углы поворота
    params_.push_back(2.0); params_.push_back(1.0); params_.push_back(0.0); // координаты источника
    params_.push_back(1.0); params_.push_back(0.5); params_.push_back(0.31); // фоновый свет
    params_.push_back(1.0); params_.push_back(0.5); params_.push_back(0.31); // расс. свет
    params_.push_back(0.5); params_.push_back(0.5); params_.push_back(0.5); // отраж. свет
    params_.push_back(1.0);
    params_.push_back(2.0);
}

void Prism::initializeGL()
{
    initializeOpenGLFunctions();
    k = 0;
    GenVertices(0,x_c,y_c);
    GenTriangles();

    float points_[points.size()*3]; // вершины
    float normals[points.size()*3]; // нормали
    int j = 0;
    for(int i = 0; i < points.size()*3; i+=3){
            points_[i] = points[j].x(); points_[i+1] = points[j].y();
            points_[i+2] = points[j].z();
            if(j%3==2 && j != 0){
                QVector3D n = GetNormal(points[j-2], points[j-1], points[j]);
                for(int k = i-6; k < (i+2); k+=3){
                    normals[k] = n.x(); normals[k+1] = n.y(); normals[k+2] = n.z();
                }
            }
           j++;
    }
    // шейд. программа
    shaderProgram_.addShaderFromSourceFile(QGLShader::Vertex, "/home/elena/Рабочий стол/LR6/light_shader.vs");
    shaderProgram_.addShaderFromSourceFile(QGLShader::Fragment, "/home/elena/Рабочий стол/LR6/light_shader.fs");
    shaderProgram_.bindAttributeLocation("inPosition", vertexLocationInShader_);
    shaderProgram_.link();
    // создание буферов
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points_), points_, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &normalVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

// отрисовка
void Prism::paintGL()
{
   QTime time = QTime::currentTime();
   float currentFrame = static_cast<float>(QTime(0, 0, 0).msecsTo(time));
   float t = currentFrame - lastFrame_;
   t *= 0.001;

   glEnable(GL_DOUBLEBUFFER);
   glEnable(GL_DEPTH_TEST);
   glCullFace(GL_FRONT_AND_BACK);
   glFrontFace(GL_CW);
   glDepthFunc(GL_LEQUAL);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   QMatrix4x4 Projection;
   Projection.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
   QMatrix4x4 View;

   View.lookAt(QVector3D(2,0,0),QVector3D(0,0,0),QVector3D(0,0,1));

   QMatrix4x4 Model;
   Model.setToIdentity();
   Model.rotate(params_[0], QVector3D(1,0,0));
   Model.rotate(params_[1], QVector3D(0,1,0));
   Model.rotate(params_[2], QVector3D(0,0,1));
   Model.scale(QVector3D(scale_, scale_, scale_));

   // при изменении степени аппроксимации
   if(n2_!=k)
   {
       glBindVertexArray(VAO_);
       k = n2_;
       GenVertices(n2_,cos(t),sin(t+x_c));
       GenTriangles();
       float points_[points.size()*3];
       float normals[points.size()*3];
       int j = 0;
       for(int i = 0; i < points.size()*3; i+=3){
           points_[i] = points[j].x(); points_[i+1] = points[j].y();
           points_[i+2] = points[j].z();
           if(j%3==2 && j != 0){
               QVector3D n = GetNormal(points[j-2], points[j-1], points[j]);
               for(int k = i-6; k < (i+2); k+=3){
                   normals[k] = n.x(); normals[k+1] = n.y(); normals[k+2] = n.z();
               }
            }
            j++;
       }
       // очистка буферов вершин и нормалей и создание новых
       glDeleteBuffers(1, &VBO_);
       glDeleteVertexArrays(1, &normalVBO_);

       glGenBuffers(1, &VBO_);
       glBindBuffer(GL_ARRAY_BUFFER, VBO_);
       glBufferData(GL_ARRAY_BUFFER, sizeof(points_), points_, GL_DYNAMIC_DRAW);

       glGenBuffers(1, &normalVBO_);
       glBindBuffer(GL_ARRAY_BUFFER, normalVBO_);
       glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);

       glGenVertexArrays(1, &VAO_);
       glBindVertexArray(VAO_);

       glBindBuffer(GL_ARRAY_BUFFER, VBO_);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
       glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO

       glBindBuffer(GL_ARRAY_BUFFER, normalVBO_);
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
       glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO

       glBindVertexArray(0);
   }

   shaderProgram_.bind();
   shaderProgram_.setUniformValue("light.position", params_[3], params_[4], params_[5]);
   shaderProgram_.setUniformValue("viewPos", 2.0f, 0.0f, 0.0f);
   shaderProgram_.setUniformValue("light.ambient",  0.2f, 0.2f, 0.2f);
   shaderProgram_.setUniformValue("light.diffuse", 0.5f, 0.5f, 0.5f);
   shaderProgram_.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);
   shaderProgram_.setUniformValue("material.ambient", params_[6], params_[7], params_[8]);
   shaderProgram_.setUniformValue("material.diffuse", params_[9], params_[10], params_[11]);
   shaderProgram_.setUniformValue("material.specular", params_[12], params_[13], params_[14]);
   shaderProgram_.setUniformValue("material.shininess", 0.5f);
   shaderProgram_.setUniformValue("projection", Projection);
   shaderProgram_.setUniformValue("view", View);
   shaderProgram_.setUniformValue("model", Model);
   shaderProgram_.setUniformValue("time", t);
   shaderProgram_.setUniformValue("x_c", x_c);
   shaderProgram_.setUniformValue("y_c", y_c);

   glBindVertexArray(VAO_);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glDrawArrays(GL_TRIANGLES, 0, NumVert*2*3*+(NumVert-2)*2*3);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);
   shaderProgram_.release();
}
// изменение размера окна
void Prism::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

Prism::~Prism()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &normalVBO_);
}
// установка значений параметров
void Prism::setF(QVector<float> params)
{
   for(int i = 0; i < params.size(); i++) params_[i] = params[i];
   this->paintGL();
}
// установка масштаба
void Prism::setScale(float scl)
{
   scale_ = scl;
   this->paintGL();
}
// установка степени аппроксимации
void Prism::setAccuracy(int value)
{
    std::cout << value << std::endl;
    n2_ = value;
    this->paintGL();
}


