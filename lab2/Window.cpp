#include "Window.h"

Window::Window(QWidget *parent): QMainWindow(parent){
	example = new View("testdata.bin", this);
	example->setGeometry(200, 200, 200, 200);

	setFixedSize(1200, 1200);
	m_button = new QPushButton("Open File", this);
	m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
	connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));


	slider = new QSlider(Qt::Horizontal, this);
	int max_count_of_layers = example->test.Z;
	slider->setRange(0, max_count_of_layers);
	connect(slider, &QSlider::valueChanged, this, &Window::slider_function);
	
}

Window::~Window(){
}

void Window::handleButton(){
	example->setmode(2);//вызываю режим текстуры
	example->resize(500, 500);
	example->show();
}

void Window::slider_function() {
	int value = slider->value();
	example->input_layer(value - 1);
}