#pragma once
#include "Bin.h"

class View : public QGLWidget {
public:
	Bin test;
	int layerNumber;
	View();
	View(string path, QWidget* parent): QGLWidget(parent){
		test.readBIN(path);
		layerNumber = 1;
	}
	~View();
	void DrawQuads(int);
	virtual void initializeGL();
	virtual void resizeGL(int, int);
	virtual void paintGL();
	void input_layer(int);
	//void input_layersdsad(int);
};
	