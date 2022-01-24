#include "widget.h"
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>

int t = 0;

//Создание окна
MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    scene = new Prism;

    button = new QPushButton("ok"); //  кнопка
    QLabel *line1 = new QLabel("Поворот");
    QLabel *line2 = new QLabel("Масштаб");
    QLabel *line3 = new QLabel("Точность");
    QLabel *line4 = new QLabel("Свет");
    QLabel *line5 = new QLabel("Фоновый");
    QLabel *line6 = new QLabel("Paccеянный");
    QLabel *line7 = new QLabel("Отраженный");

    lineEdits.resize(15);
    // поля ввода
    for(int i = 0; i < 15; i++){
        lineEdits[i] = new QLineEdit;
    }

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setSingleStep(1);
    slider->setValue(50);

    slider2 = new QSlider(Qt::Horizontal);
    slider2->setMinimum(0);
    slider2->setMaximum(5);
    slider2->setSingleStep(1);
    slider2->setValue(0);

    QGridLayout *grid = new QGridLayout(this); // слой, на который все будет доюавляться
    grid->addWidget(scene, 0, 0, 3, 4);

    grid->addWidget(line1, 3, 0);
    for(int i = 0; i < 3; i++) lineEdits[i]->setText("0.0");
    grid->addWidget(lineEdits[0], 3, 1);
    grid->addWidget(lineEdits[1], 3, 2);
    grid->addWidget(lineEdits[2], 3, 3);

    grid->addWidget(line4, 4, 0);
    lineEdits[3]->setText("2.0");
    lineEdits[4]->setText("1.0");
    lineEdits[5]->setText("0.0");
    grid->addWidget(lineEdits[3], 4, 1);
    grid->addWidget(lineEdits[4], 4, 2);
    grid->addWidget(lineEdits[5], 4, 3);

    grid->addWidget(line2, 5, 0);
    grid->addWidget(slider, 5, 1, 1, 3);
    grid->addWidget(line3, 6, 0);
    grid->addWidget(slider2, 6, 1, 1, 3);

    grid->addWidget(line5, 7, 0);
    lineEdits[6]->setText("1.0");
    lineEdits[7]->setText("0.5");
    lineEdits[8]->setText("0.31");
    grid->addWidget(lineEdits[6], 7, 1);
    grid->addWidget(lineEdits[7], 7, 2);
    grid->addWidget(lineEdits[8], 7, 3);

    grid->addWidget(line6, 8, 0);
    lineEdits[9]->setText("1.0");
    lineEdits[10]->setText("0.5");
    lineEdits[11]->setText("0.31");
    grid->addWidget(lineEdits[9], 8, 1);
    grid->addWidget(lineEdits[10], 8, 2);
    grid->addWidget(lineEdits[11], 8, 3);

    grid->addWidget(line7, 9, 0);
    for(int i = 12; i < 15; i++) lineEdits[i]->setText("0.5");
    grid->addWidget(lineEdits[12], 9, 1);
    grid->addWidget(lineEdits[13], 9, 2);
    grid->addWidget(lineEdits[14], 9, 3);

    grid->addWidget(button, 10, 0);
    // связывание кнопки и слайдеров со слотами
    QObject::connect(button, &QPushButton::clicked, this, &MainWidget::on_ok_clicked);
    QObject::connect(slider, &QSlider::valueChanged, this, &MainWidget::slider_changed);
    QObject::connect(slider2, &QSlider::valueChanged, this, &MainWidget::slider2_changed);

}
// отправка параметров

void MainWidget::on_ok_clicked()
{
    QVector<float> params;
    for(int i = 0; i < 15; i++)
        params.push_back(lineEdits[i]->text().toFloat());
    scene->setF(params);
}
// отправка масштаба
void MainWidget::slider_changed(int value)
{
    scene->setScale(static_cast<float>(value/100.f));
}
// отправка степени аппроксимации
void MainWidget::slider2_changed(int value)
{
    scene->setAccuracy(value);
}

MainWidget::~MainWidget() {}


