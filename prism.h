// Иванова Елена М8О-301Б
// ЛР 6
// Вариант 7: 4-гранная правильная прямая призма
// Вариант 13. Усеченный прямой круговой цилиндр
// Вариант 5: Анимация. Координата X изменяется по закону X*cos(t), координата Y изменяется по закону Y = Ysin(X+t)

#ifndef PRISM_H
#define PRISM_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QtMath>
#include <QTimer>

class Prism: public QGLWidget, protected QOpenGLExtraFunctions{
    Q_OBJECT
private:
    const int vertexLocationInShader_ = 0;
    int n2_; // степень аппроксимации
    QVector<float> params_; // угол поворота, координаты источника света, фоновый, рассеянный и отраженный свет
    float scale_; // для регулирования масштаба
    GLuint VAO_, VBO_, normalVBO_; // массивы для вершин, их буферов и нормалей
    QGLShaderProgram shaderProgram_; // шейдерная программа
    float lastFrame_; // таймер
    float x_c, y_c; // центр фигуры
protected:
    void initializeGL(); // инициализация
    void resizeGL(int w, int h); // изменение размара
    void paintGL(); // отрисовка
 //   void updateGL();
public:
   Prism(QWidget *prt = 0);
    //Prism(const QGLFormat& fmt);
    ~Prism();
    void setF(QVector<float> params); // установка значения пааметров
    void setScale(float scl); // установка масштаба
    void setAccuracy(int value); // установка степени аппроксимации
};

#endif // PRISM_H
