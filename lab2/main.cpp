#include <QApplication>
#include <QGLWidget>
#include <iostream>
#include <string>
#include <stdio.h>
#include <qimage.h>
#include <QtOpenGL/QGLWidget>
#include "Bin.h"
#include "View.h"
using namespace std;

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	View example("testdata.bin");

	example.resize(1000, 500);
	example.show();

	return a.exec();
}
