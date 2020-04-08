#pragma once
#include "Bin.h"

class View : public QGLWidget {
public:
	Bin test;
	View();
	View(string path) {
		test.readBIN(path);
	}
	~View();
	virtual void initializeGL();
	virtual void resizeGL(int, int);
	virtual void paintGL(int);
};
