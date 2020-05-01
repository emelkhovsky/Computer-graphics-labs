#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QInputDialog>
#include <QGLWidget>
#include "Bin.h"
#include "View.h"


class Window: public QMainWindow{
	Q_OBJECT//������ Q_OBJECT, �.�. ��� ����������� ���� ����������� ������� � �����
public:
	explicit Window(QWidget *parent = 0);
	~Window();
	QPushButton *m_button;
	QSlider *sliderZ;
	View* example;
private slots:
	void handleButton();
	void sliderZ_function();
};

