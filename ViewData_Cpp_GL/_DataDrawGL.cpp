#include"_DataDrawGL.h"
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include<limits> 
#include<algorithm>
#include<gl/glew.h>
#include"Count.h"
using namespace std;
DataDrawGL::DataDrawGL(const std::string& filename) {
	this->Open(filename);
}

void DataDrawGL::Open(const std::string& filename) {


	auto t1 = CTime::GetMicroS();
	this->load_bin_data(filename);
	auto t2 = CTime::GetMicroS();
	cout << t2 - t1 << endl;
	this->create_vec();
	this->create_vbo_curve();
}
void DataDrawGL::mat_transpose(const  DataDrawGL::Mat&src, DataDrawGL::Mat&dst) {
	auto h = src.size();
	auto w = src[0].size();

	Mat _dst = Mat(w, Line(h));

	for (auto i = 0; i < h; ++i)
		for (auto j = 0; j < w; ++j)
			_dst[j][i] = src[i][j];
	dst.clear();
	dst = _dst;
}



void DataDrawGL::create_vec() {
	//导入点
	GLuint inc=0;

	int sum = this->_mtx.size()*this->_mtx[0].size()+100;
	this->_vec.reserve(sum);
	this->_indexs_curve.reserve(sum);
	this->_colour.reserve(sum);
	this->_vec_color.reserve(sum);

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
			green = min(green * 2, 1.0);
			double blue = 1 - color;
			_colour.push_back(Vec3d(red, green, blue));
			//_colour.push_back(Vec3d(color, color, color));	//灰度图

			_vec_color.push_back(Point2d(x,_data_max-(_data_max-_data_min)/this->_mtx.size()*y));
		}
	}
	this->_curve_colour = Vec_Vec3d(inc, Vec3d(1, 0, 0));


	_indexs_color.resize(4 * sum);
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

void DataDrawGL::NoramlData2BinData(const std::string &normal_filename, const std::string &bin_filename) {
	ifstream normal_ifs;
	stringstream ss;
	string str_line;
	Line data_line;
	double val;
	Mat tmp_mtx;
	normal_ifs.open(normal_filename);
	double max_val = (numeric_limits<double>::min)();
	double min_val = (numeric_limits<double>::max)();
	int32_t nums = 0;
	int32_t len = 0;
	if (normal_ifs.good()) {
		ss.clear();
		getline(normal_ifs, str_line);
		ss << str_line;
		while (ss >> val) {
			len++;
		}
	}
	normal_ifs.close();
	FILE *c_ifs;
	c_ifs = fopen(normal_filename.c_str(), "r");
	data_line.resize(len);
	while (!feof(c_ifs)) {
		for (int i = 0; i < len; i++) {
			fscanf(c_ifs, "%lf", &val);
			data_line[i] = val;
			max_val = max(max_val, val);
			min_val = min(min_val, val);
		}
		nums++;
		tmp_mtx.push_back(data_line);
	}
	fclose(c_ifs);
	mat_transpose(tmp_mtx, tmp_mtx);
	// 因为上面这条,矩阵转置,所以len 和 nums 交换
	std::swap(nums, len);

	Vec_Vec2d min_max_ary;
	for (const auto&line : tmp_mtx) {
		GLdouble curve_max_val = (numeric_limits<GLdouble>::min)();
		GLdouble curve_min_val = (numeric_limits<GLdouble>::max)();
		for (const auto&val : line) {
			curve_max_val = max(curve_max_val, val);
			curve_min_val = min(curve_min_val, val);
		}
		min_max_ary.push_back(DataDrawGL::Point2d(curve_min_val, curve_max_val));
	}


	//生成bin数据
	ofstream bin_ofs(bin_filename, std::ios::binary);
	
	/*	bias(b) 0---31  32---63	 64---127  128---191
	 *			nums	len		  max_val    min_val 
	 *			192~~~~192+nums*sizeof(double)*2-1			
	 *			<line_min,line_max>*nums
	 *			192+nums*sizeof(double)*2~~~~end
	 *			data(double)
	 *	@yd 数据储存格式
	 */


	bin_ofs.write(reinterpret_cast<const char*>(&nums), sizeof(int32_t));
	bin_ofs.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
	bin_ofs.write(reinterpret_cast<const char*>(&max_val), sizeof(double));
	bin_ofs.write(reinterpret_cast<const char*>(&min_val), sizeof(double));

	bin_ofs.write(reinterpret_cast<const char*>(min_max_ary.data()), min_max_ary.size() * sizeof(DataDrawGL::Point2d));

	for (auto&line : tmp_mtx) {
		bin_ofs.write(reinterpret_cast<const char*>(line.data()), len * sizeof(double));
	}
	bin_ofs.close();
}

void DataDrawGL::load_bin_data(const std::string&bin_filename) {
	int32_t len, nums;
	/*
	ifstream bin_ifs(bin_filename, std::ios::binary);
	bin_ifs.read(reinterpret_cast<char*>(&nums), sizeof(int32_t));
	bin_ifs.read(reinterpret_cast<char*>(&len), sizeof(int32_t));

	bin_ifs.read(reinterpret_cast<char*>(&(this->_data_max)), sizeof(double));
	bin_ifs.read(reinterpret_cast<char*>(&(this->_data_min)), sizeof(double));

	this->_curve_max_min_ary = Vec_Vec2d(nums);
	bin_ifs.read(reinterpret_cast<char*>(this->_curve_max_min_ary.data()), sizeof(DataDrawGL::Point2d)*nums);

	this->_mtx = Mat(nums, Line(len, 0.0));
	for (auto&line : this->_mtx) {
		bin_ifs.read(reinterpret_cast<char*>(line.data()), sizeof(double)*len);
	}
	*/
	FILE *ifs = fopen(bin_filename.c_str(), "rb");
	fread(reinterpret_cast<char*>(&nums), sizeof(int32_t),1, ifs);
	fread(reinterpret_cast<char*>(&len), sizeof(int32_t), 1, ifs);
	fread(reinterpret_cast<char*>(&(this->_data_max)),sizeof(double),1,ifs);
	fread(reinterpret_cast<char*>(&(this->_data_min)), sizeof(double), 1, ifs);
	this->_curve_max_min_ary.resize(nums);
	fread(reinterpret_cast<char*>(this->_curve_max_min_ary.data()), sizeof(DataDrawGL::Point2d),nums,ifs);

	auto t1 = CTime::GetMicroS();

	this->_mtx = Mat(nums, Line(len, 0.0));
	for (auto&line : this->_mtx) {
		fread(reinterpret_cast<char*>(line.data()), sizeof(double),len,ifs);
	}

	auto t2 = CTime::GetMicroS();
	cout << t2 - t1 << endl;
	fclose(ifs);
}