#include "DataDrawGL.h"
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include<limits> 
#include<algorithm>
#include<gl/glew.h>
using namespace std;
DataDrawGL::DataDrawGL(const std::string& filename) {
	this->Open(filename);
}

void DataDrawGL::Open(const std::string& filename) {
	this->_data_max = (numeric_limits<GLdouble>::min)();
	this->_data_min = (numeric_limits<GLdouble>::max)();

	this->load_data(filename);
	this->create_vec();
	this->create_vbo_curve();
}

void DataDrawGL::load_data(const std::string& filename) {
	ifstream ifs;
	stringstream ss;
	string str_line;
	Line data_line;
	double val;
	_mtx.clear();
	_mtx.reserve(20000);
	ifs.open(filename);
	double max_val = (numeric_limits<double>::min)();
	double min_val = (numeric_limits<double>::max)();
	int inc = 0;
	int len = 0;
	if (ifs.good()) {
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
			data_line[i] = val;
			max_val = max(max_val, val);
			min_val = min(min_val, val);
		}
		_mtx.push_back(data_line);

	}
	fclose(c_ifs);

	this->_data_max = max_val;
	this->_data_min = min_val;
	this->mat_transpose(this->_mtx, this->_mtx);
	this->get_all_curve_maxmin();
}

void DataDrawGL::mat_transpose(const  DataDrawGL::Mat&src, DataDrawGL::Mat&dst) {
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

void DataDrawGL::get_all_curve_maxmin(){
	this->_curve_max_min_ary.clear();
	for (const auto&line : this->_mtx) {
		GLdouble max_val = (numeric_limits<GLdouble>::min)();
		GLdouble min_val = (numeric_limits<GLdouble>::max)();
		for (const auto&val : line) {
			max_val = max(max_val, val);
			min_val = min(min_val, val);
		}
		this->_curve_max_min_ary.push_back(DataDrawGL::Point2d(min_val, max_val));
	}
}

void DataDrawGL::create_vec() {
	//导入点
	GLuint inc=0;
	for (int y = 0;y < this->_mtx.size();++y) {
		double max_val = _curve_max_min_ary[y].x;
		double min_val = _curve_max_min_ary[y].y;
		for (int x = 0;x < this->_mtx[y].size();++x) {
			this->_vec.push_back(Point2d(x, _mtx[y][x]));
			this->_indexs_curve.push_back(inc++);
			double color = (_mtx[y][x] - min_val) / (max_val - min_val);

			//在伪彩色图中对应的点越趋向于 红色；亮度越低，则对应的伪彩色越趋向于 蓝色；总体上按照灰度值高低，由红渐变至蓝，中间色为绿色。
			double red =  color;
			double green = color < 0.5 ? color : 1 - color;
			green = std::min(green * 2, 1.0);
			double blue = 1 - color;
			_colour.push_back(Vec3d(red, green, blue));
			//_colour.push_back(Vec3d(color, color, color));	//灰度图

			_vec_color.push_back(Point2d(x,_data_max-(_data_max-_data_min)/this->_mtx.size()*y));
		}
	}
	this->_curve_colour = Vec_Vec3d(inc, Vec3d(1, 0, 0));


	for (auto i = 0;i < this->_mtx.size() - 1;i++) {
		int _w = this->_mtx[i].size();
		for (auto j = 0;j < _w - 1;j++) {
			_indexs_color.push_back(i*_w + j);
			_indexs_color.push_back(i*_w + j + 1);
			_indexs_color.push_back((i + 1)*_w + j + 1);
			_indexs_color.push_back((i + 1)*_w + j);
		}
	}
}

void DataDrawGL::create_vbo_curve() {
	GLint err=glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW初始化失败！\n");
		exit(EXIT_FAILURE);
	}

	glGenVertexArrays(2, vao);

	
	//---------------			创建曲线
	glBindVertexArray(vao[0]);
	glGenBuffers(3, vbo+0);
	//顶点
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _vec.size()*sizeof(Point2d),_vec.data(), GL_DYNAMIC_DRAW);
	glVertexPointer(2, GL_DOUBLE, 0, nullptr);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, _curve_colour.size()*sizeof(Vec3d), _curve_colour.data(), GL_DYNAMIC_DRAW);
	glColorPointer(3, GL_DOUBLE, 0, nullptr);
	glEnableClientState(GL_COLOR_ARRAY);

	// GL_ELEMENT_ARRAY_BUFFER（表示索引数据），用索引数据初始化缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexs_curve.size()*sizeof(GLuint), _indexs_curve.data(), GL_STATIC_DRAW);
	

	//--------------			创建灰度块
	
	glBindVertexArray(vao[1]);
	glGenBuffers(3, vbo + 3);
	//顶点
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, _vec_color.size()*sizeof(Point2d), _vec_color.data(), GL_DYNAMIC_DRAW);
	glVertexPointer(2, GL_DOUBLE, 0, nullptr);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, _colour.size()*sizeof(Vec3d), _colour.data(), GL_DYNAMIC_DRAW);
	glColorPointer(3, GL_DOUBLE, 0, nullptr);
	glEnableClientState(GL_COLOR_ARRAY);

	// GL_ELEMENT_ARRAY_BUFFER（表示索引数据），用索引数据初始化缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexs_color.size()*sizeof(GLuint), _indexs_color.data(), GL_STATIC_DRAW);
	
}

void DataDrawGL::DrawCurve(int index) {
	static int last = -1;
	//if (index != last) {
		glBindVertexArray(index);
		last = index;
	//}
	if (index == 1) {
		int bias = 0;
		for (int i = 0;i < _mtx.size();i++) {
			glDrawElements(GL_LINE_STRIP, _mtx[i].size(), GL_UNSIGNED_INT, ((GLuint*)NULL + (bias)));
			bias += _mtx[i].size();
		}
	}
	else if(index==2){
		glDrawElements(GL_QUADS, _indexs_color.size(), GL_UNSIGNED_INT, ((GLuint*)NULL + (0)));
	}
}