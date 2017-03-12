#include"WindowControl.h"
#include<gl/glew.h>


WindowControl::WindowControl(GLdouble width, GLdouble height,
	GLdouble x_min , GLdouble x_max , GLdouble y_min , GLdouble y_max)
{
	this->_z_max = 1.0;
	this->_z_min = -1.0;

	this->_width = width;
	this->_height = height;
	this->_x_min = x_min;
	this->_x_max = x_max;
	this->_y_min = y_min;
	this->_y_max = y_max;

	//updata
	this->_UpdataOrtho();
}

void WindowControl::Ortho(GLdouble x_min, GLdouble x_max, 
	GLdouble y_min, GLdouble y_max, GLdouble z_min , GLdouble z_max ) {

	this->_z_max = z_max;
	this->_z_min = z_min;
	this->_x_min = x_min;
	this->_x_max = x_max;
	this->_y_min = y_min;
	this->_y_max = y_max;

	//updata
	this->_UpdataOrtho();
}

void WindowControl::Reshape(GLdouble width, GLdouble height) {
	this->_width = width;
	this->_height = height;

	//updata
	this->_UpdataOrtho();
}

void WindowControl::Move(GLdouble r_x, GLdouble r_y) {
	this->_x_min += r_x;
	this->_x_max += r_x;
	this->_y_min += r_y;
	this->_y_max += r_y;

	this->_UpdataOrtho();
}

void WindowControl::Move(GLint x0, GLint y0, GLint x1, GLint y1) {
	double _x = -(x1 - x0);// 这里与你的窗口的坐标系有关,如果有问题尝试修改正负号
	double _y = +(y1 - y0);// 

	double c_x = (_x_max - _x_min) / _width;
	double c_y = (_y_max - _y_min) / _height;

	_x *= c_x;
	_y *= c_y;
	this->Move(_x, _y);
}

void WindowControl::Scale(int x, int y, double s_x, double s_y) {
	double c_x = (_x_max - _x_min) *s_x;
	double c_y = (_y_max - _y_min) *s_y;
	auto p = this->GetWorldPoint2d(x, y);

	this->_x_min = p.x - (p.x - this->_x_min) / s_x;
	this->_y_min = p.y - (p.y - this->_y_min) / s_y;

	this->_x_max = p.x + (this->_x_max - p.x) / s_x;
	this->_y_max = p.y + (this->_y_max - p.y) / s_y;

	this->_UpdataOrtho();
}

WindowControl::Point2d WindowControl::GetWorldPoint2d(GLdouble w_x, GLdouble w_y) {
	double _x = w_x;
	double _y = _height - w_y;
	double c_x = (_x_max - _x_min) / _width;
	double c_y = (_y_max - _y_min) / _height;

	_x *= c_x;
	_y *= c_y;

	_x += _x_min;
	_y += _y_min;
	return WindowControl::Point2d(_x,_y);
}

void WindowControl::_UpdataOrtho() {
	glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_x_min, _x_max, _y_min, _y_max, _z_min, _z_max);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}