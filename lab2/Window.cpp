#include "Window.h"


Window::Window(QWidget *parent): QMainWindow(parent){
	example = new View("testdata.bin", this);
	example->setGeometry(200, 200, 200, 200);

	setFixedSize(1200, 1200);
	m_button = new QPushButton("Open File", this);
	m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
	connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));


	sliderZ = new QSlider(Qt::Horizontal, this);
	int max_count_of_layers = example->test.Z;
	sliderZ->setRange(0, max_count_of_layers);
	connect(sliderZ, &QSlider::valueChanged, this, &Window::sliderZ_function);
	
	bool ok;
	int imin = QInputDialog::getInt(this, tr("input min transferfunction"), tr("Your min value:"), 0, 0, 5000, 1, &ok);
	if (ok)//если человек нажал окей
		example->min = imin;
	int imax = QInputDialog::getInt(this, tr("input max transferfunction"), tr("Your max value:"), 0, 0, 5000, 1, &ok);
	if (ok)//если человек нажал окей
		example->max = imax;
}

Window::~Window(){

}

void Window::handleButton(){
	example->setmode(2);//указываю режим
	example->resize(500, 500);
	example->show();
}

void Window::sliderZ_function() {
	int value = sliderZ->value();
	example->input_layer(value - 1);
}