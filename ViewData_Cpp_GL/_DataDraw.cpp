#include<gl/freeglut.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include<limits> 
#include<algorithm>

#include<Windows.h>
#include<gl/freeglut.h>

#include"_DataDraw.h"
using namespace std;
void DataDraw::create_init() {
	this->_data_max = 0;
	this->_data_min = 0;
	this->_height = 0;
	this->_width = 0;
	this->_mtx = Mat();
	this->_x_real_scale = 1;
	this->_y_real_scale = 1;
	this->_gl_list_id = reinterpret_cast<GLuint>(this);
}

bool DataDraw::load_data(Mat &mtx,const string& filename) {
	auto tt = GetTickCount();
	ifstream ifs;
	stringstream ss;
	string str_line;
	Line data_line;
	double val;
	mtx.clear();
	mtx.reserve(20000);
	ifs.open(filename);
	double max_val = (numeric_limits<double>::min)();
	double min_val = (numeric_limits<double>::max)();
	

	int inc = 0;
	int len = 0;
	if(ifs.good()) {
		ss.clear();
		getline(ifs, str_line);
		ss << str_line;
		while (ss >> val) {
			len++;
		}
	}
	ifs.close();
	FILE *c_ifs;
	c_ifs = fopen(filename.c_str(), "r");
	data_line.resize(len);
	while (!feof(c_ifs)) {
		//data_line.clear();
		for (int i = 0;i < len;i++) {
			fscanf(c_ifs, "%lf", &val);
			data_line[i]=val;
			max_val = max(max_val, val);
			min_val = min(min_val, val);
		}
		mtx.push_back(data_line);

	}
	fclose(c_ifs);

	this->_data_max = max_val;
	this->_data_min = min_val;
	this->mat_transpose(this->_mtx, this->_mtx);
	this->getAllCurveMaxMin();
	cout << "read data time(ms):" << (GetTickCount() - tt)<< endl;
	return true;
}

void DataDraw::mat_transpose(const Mat& src, Mat &dst) {
	//h 是 src 的高度 ,w 是 src 的宽度
	auto h = src.size();
	auto w = src[0].size();

	//dst刚好换过来  ,！！！注意 这个_dst 是临时的
	Mat _dst = Mat(w, Line(h));

	//由于数据是离散的不是连续的，这里cache的缓存机制效果体现不出来
	for (auto i = 0; i < h; ++i)
		for (auto j = 0; j < w; ++j)
			_dst[j][i] = src[i][j];

	dst.clear();
	dst = _dst;
}

void DataDraw::create_gl_list() {
	if (glIsList(this->_gl_list_id) == GL_TRUE) {	//被占用
		glDeleteLists(this->_gl_list_id, 1);
	}
	glNewList(this->getListId(), GL_COMPILE);
	double len = static_cast<double>(_mtx[0].size());
	int inc = 0;
	double y_unit= (_data_max - _data_min) / _mtx.size();
	for (auto iter = _mtx.begin();iter!=_mtx.end();++iter) {
		glBegin(GL_LINE_STRIP /*GL_QUADS*/); {
			double max_val = _curve_max_min_ary[inc].x;
			double min_val = _curve_max_min_ary[inc].y;
			for (auto i = 0;i < iter->size();++i) {
				/*	彩色绘图
				double _x = i;// i / len - 0.5;
				double _y = iter->at(i)+inc*0.05;//(iter->at(i) - _data_min) / (_data_max - _data_min) - 0.5;

				glVertex2d(_x, _y);
				*/
				
				double _x = i;
				double _y = _data_max-y_unit*inc;
				double color = (iter->at(i) - min_val) / (max_val - min_val);
				glColor3d(color, color, color);
				//glRectd(_x,_y,_x+1,_y-y_unit);
				glVertex2d(_x, _y);
				glVertex2d(_x + 1, _y);
				glVertex2d(_x + 1, _y - y_unit);
				glVertex2d(_x , _y - y_unit);
				
	
			}
			inc++;
		}glEnd();
	}
	glEndList();
	/*
	this->_x_min = -0.5;
	this->_x_max = 0.5;
	this->_y_min = -0.5;
	this->_y_max = 0.5;
	*/
	this->_x_min = 0;
	this->_x_max = len;
	this->_y_min = _data_min;
	this->_y_max = _data_max;
}

GLuint DataDraw::getListId() {
	return this->_gl_list_id;
}

DataDraw::DataDraw() {
	this->create_init();
}

DataDraw::DataDraw(int _w, int _h) {
	this->create_init();
	this->_height = _h;
	this->_width = _w;
}

DataDraw::DataDraw(const string& filename, int _w, int _h) {
	this->create_init();
	this->_height = _h;
	this->_width = _w;
	this->load_data(this->_mtx, filename);
}

DataDraw::DataDraw(const Mat& _in_mtx, int _w, int _h) {
	this->create_init();
	this->_height = _h;
	this->_width = _w;
	this->_mtx= _in_mtx;
}

bool DataDraw::open(const string& filename) {
	return this->load_data(this->_mtx, filename);
}

void DataDraw::reshape(int _w, int _h) {
	this->_height = _h;
	this->_width = _w;
	this->updataOrtho2D();
}

DataDraw::Point2d DataDraw::getWorldPoint(int x, int y) {
	double _x = x;
	double _y = _height - y;
	double c_x = (_x_max - _x_min) / static_cast<double>(_width);
	double c_y = (_y_max - _y_min) / static_cast<double>(_height);

	_x *= c_x;
	_y *= c_y;

	_x += _x_min;
	_y += _y_min;
	return{ _x,_y };
}

void DataDraw::move(double _x, double _y) {
	this->_x_min += _x;
	this->_x_max += _x;
	this->_y_min += _y;
	this->_y_max += _y;
	this->updataOrtho2D();
}

void DataDraw::move(int x0, int y0, int x1, int y1) {
	double _x = -(x1 - x0);// / static_cast<double>(_width);
	double _y = +(y1 - y0);// / static_cast<double>(_height);

	double c_x = (_x_max - _x_min)/ static_cast<double>(_width);
	double c_y = (_y_max - _y_min) / static_cast<double>(_height);

	_x *= c_x;
	_y *= c_y;
	this->move(_x, _y);
}

void DataDraw::scale(int x, int y, double s_x, double s_y) {
	double c_x = (_x_max - _x_min) *s_x;
	double c_y = (_y_max - _y_min) *s_y;
	auto p = this->getWorldPoint(x, y);
	
	this->_x_min = p.x-(p.x - this->_x_min)	/ s_x;
	this->_y_min = p.y-(p.y - this->_y_min) / s_y;

	this->_x_max = p.x+(this->_x_max - p.x) / s_x;
	this->_y_max = p.y+(this->_y_max - p.y) / s_y;

	this->updataOrtho2D();
}

void DataDraw::updataOrtho2D() {
	glLoadIdentity();
	glViewport(0, 0, _width, _height);
	gluOrtho2D(_x_min, _x_max, _y_min, _y_max);//left right,buttom,up (left,buttom),(right,up)
	glMatrixMode(GL_MODELVIEW);
}

void DataDraw::getAllCurveMaxMin() {
	this->_curve_max_min_ary.clear();
	for (const auto&line : this->_mtx) {
		double max_val = (numeric_limits<double>::min)();
		double min_val = (numeric_limits<double>::max)();
		for (const auto&val : line) {
			max_val = max(max_val, val);
			min_val = min(min_val, val);
		}
		this->_curve_max_min_ary.push_back(DataDraw::Point2d(max_val,min_val));
	}
}

