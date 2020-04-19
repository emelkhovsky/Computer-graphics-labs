#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QGLWidget>
#include "Bin.h"
#include "View.h"


class Window: public QMainWindow{
	Q_OBJECT//макрос Q_OBJECT, т.к. тут реализуются свои собственные сигналы и слоты
public:
	explicit Window(QWidget *parent = 0);
	~Window();
	QPushButton *m_button;
	QSlider *slider;
	View example;
private slots:
	void handleButton();
	void slider_function();
};

