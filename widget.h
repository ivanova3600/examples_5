#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QTimer>
#include "prism.h"

// для создания окна
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
public slots: // слоты
    void on_ok_clicked();
    void slider_changed(int value);
    void slider2_changed(int value);
private:
    QPushButton *button; // кнопка
    QSlider *slider; // слайдер
    QSlider *slider2; // слайдер 2
    QVector<QLineEdit*> lineEdits; // поля ввода
    Prism *scene;
};

#endif // WIDGET_H
