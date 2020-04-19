#include "Window.h"

Window::Window(QWidget *parent): QMainWindow(parent), example("testdata.bin") {
	setFixedSize(400, 400);
	m_button = new QPushButton("Open File", this);
	m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
	connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));


	slider = new QSlider(Qt::Horizontal, this);
	int max_count_of_layers = example.test.Z;
	slider->setRange(0, max_count_of_layers);
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slider_function()));
	
}


Window::~Window(){
}


void Window::handleButton(){
	example.resize(1000, 500);
	example.show();
}

void Window::slider_function() {
	example.resize(1000, 500);
	example.show();
}