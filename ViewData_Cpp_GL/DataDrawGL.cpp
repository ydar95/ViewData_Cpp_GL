#include"DataDrawGL.h"
#include"Count.h"
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include<limits> 
#include<algorithm>
#include<list>
using namespace std;

void DataDrawGL::NoramlData2BinData(const std::string &normal_filename, const std::string &bin_filename) {
	ifstream normal_ifs;
	stringstream ss;
	string str_line;
	Line data_line;
	RealType val;
	Mat tmp_mtx;
	normal_ifs.open(normal_filename);
	RealType max_val = (numeric_limits<RealType>::min)();
	RealType min_val = (numeric_limits<RealType>::max)();
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
			fscanf(c_ifs, "%f", &val);
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

	Vec_Vec2 min_max_ary;
	for (const auto&line : tmp_mtx) {
		RealType curve_max_val = (numeric_limits<RealType>::min)();
		RealType curve_min_val = (numeric_limits<RealType>::max)();
		for (const auto&val : line) {
			curve_max_val = max(curve_max_val, val);
			curve_min_val = min(curve_min_val, val);
		}
		min_max_ary.push_back(DataDrawGL::Vec2(curve_min_val, curve_max_val));
	}


	//生成bin数据
	ofstream bin_ofs(bin_filename, std::ios::binary);

	/*	bias(b) 0---31  32---63	 64---127  128---191
	*			nums	len		  max_val    min_val
	*			192~~~~192+nums*sizeof(RealType)*2-1
	*			<line_min,line_max>*nums
	*			192+nums*sizeof(RealType)*2~~~~end
	*			data(double)
	*	@yd 数据储存格式
	*/


	bin_ofs.write(reinterpret_cast<const char*>(&nums), sizeof(int32_t));
	bin_ofs.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
	bin_ofs.write(reinterpret_cast<const char*>(&max_val), sizeof(RealType));
	bin_ofs.write(reinterpret_cast<const char*>(&min_val), sizeof(RealType));

	bin_ofs.write(reinterpret_cast<const char*>(min_max_ary.data()), min_max_ary.size() * sizeof(DataDrawGL::Vec2));

	for (auto&line : tmp_mtx) {
		bin_ofs.write(reinterpret_cast<const char*>(line.data()), len * sizeof(RealType));
	}
	bin_ofs.close();
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
void DataDrawGL::load_bin_data(const std::string& bin_filename) {
	FILE *ifs = fopen(bin_filename.c_str(), "rb");
	fread(reinterpret_cast<char*>(&_nums), sizeof(int32_t), 1, ifs);
	fread(reinterpret_cast<char*>(&_len), sizeof(int32_t), 1, ifs);
	fread(reinterpret_cast<char*>(&(this->_data_max)), sizeof(RealType), 1, ifs);
	fread(reinterpret_cast<char*>(&(this->_data_min)), sizeof(RealType), 1, ifs);
	this->_curve_max_min_ary.resize(_nums);
	fread(reinterpret_cast<char*>(this->_curve_max_min_ary.data()), sizeof(DataDrawGL::Vec2), _nums, ifs);
	this->_data.reset(new RealType[_nums*_len], std::default_delete<RealType[]>());
	fread(reinterpret_cast<char*>(_data.get()), sizeof(RealType)*_len*_nums,1 , ifs);

	fclose(ifs);
}
void DataDrawGL::Open(const std::string& filename) {

	this->load_bin_data(filename);
	this->create_curve_data();
	this->create_curve_vbo();
}
void DataDrawGL::DrawCurve(int index) {
	glBindVertexArray(vao[0]);
	glColor3f(0.5, 0.8, 0.1);
	int bias = 0;
	for (int i = 0; i < _nums; i++) {
		glDrawElements(GL_LINE_STRIP, _len, GL_UNSIGNED_INT, ((GLuint*)NULL + (bias)));
		bias += _len;
	}
}
void DataDrawGL::create_curve_data() {
	auto _data_ptr = _data.get();
	auto _vec_ptr = _curve_vec.get();
	auto _indexs_ptr = _curve_indexs.get();
	int  inc = 0;
	for (uint32_t num = 0; num < _nums; ++num) {
		for (uint32_t index = 0; index < _len; ++index) {
			*(_vec_ptr++)=Vec2(index, *(_data_ptr++));
			*(_indexs_ptr++) = inc++;
		}
	}
}
void DataDrawGL::create_curve_vbo() {



	//---------------			创建曲线
	glBindVertexArray(vao[0]);
	//glGenBuffers(3, vbo + 0);
	//顶点
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _nums*_len * sizeof(Vec2), _curve_vec.get());

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, _curve_colour.size() * sizeof(Vec3), _curve_colour.data(), GL_DYNAMIC_DRAW);
	//glColorPointer(3, GL_FLOAT, 0, nullptr);
	//glEnableClientState(GL_COLOR_ARRAY);

	// GL_ELEMENT_ARRAY_BUFFER（表示索引数据），用索引数据初始化缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _nums*_len * sizeof(GLuint), _curve_indexs.get());
}
void DataDrawGL::create_curve_buff() {
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW初始化失败！\n");
		exit(EXIT_FAILURE);
	}
	if (vbo[0] != 0) {
		glDeleteBuffers(3, vbo);
		vbo[0] = vbo[1] = vbo[2] = 0;
	}
	if (vao[0] != 0) {
		glDeleteVertexArrays(1, &vao[0]);
	}

	glGenVertexArrays(1, vao);

	glBindVertexArray(vao[0]);
	glGenBuffers(3, vbo + 0);
	//顶点
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, buff_nums*buff_len * sizeof(Vec2), nullptr, GL_DYNAMIC_DRAW_ARB);
	glVertexPointer(2, GL_FLOAT, 0, nullptr);
	glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, _curve_colour.size() * sizeof(Vec3), _curve_colour.data(), GL_DYNAMIC_DRAW);
	//glColorPointer(3, GL_FLOAT, 0, nullptr);
	//glEnableClientState(GL_COLOR_ARRAY);

	// GL_ELEMENT_ARRAY_BUFFER（表示索引数据），用索引数据初始化缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buff_nums*buff_len * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW_ARB);
}

void DataDrawGL::create_block_data() {
	
}