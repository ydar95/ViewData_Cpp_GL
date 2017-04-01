#pragma once
#ifndef _YDAR_HEAD_DATA_DRAW_GL_H_
#define _YDAR_HEAD_DATA_DRAW_GL_H_ 1

#include<vector>
#include<string>
#include<memory>
#include"WindowControl.h"
#include<GL\freeglut.h>
class DataDrawGL
{
private:
	enum VBO_ARRAY_INDEX
	{
		enum_curve = 0,
		enum_color,
		enum_gary
	};
	uint32_t buff_len;
	uint32_t buff_nums;
public:
	typedef typename				GLfloat RealType;
	struct Vec3 {
		RealType x;
		RealType y;
		RealType z;
		Vec3(RealType _x = 0.0, RealType _y = 0.0, RealType _z = 0.0) {
			x = _x;
			y = _y;
			z = _z;
		}
	};
	struct Vec2 {
		RealType x;
		RealType y;
		Vec2(RealType _x = 0.0, RealType _y = 0.0) {
			x = _x;
			y = _y;
		}
	};
	typedef std::vector<Vec3>		Vec_Vec3;
	typedef std::vector<Vec2>		Vec_Vec2;

private:
	typedef std::vector<RealType>	Line;
	typedef std::vector<Line>		Mat;
private:
	std::shared_ptr<RealType>	_data;			//	ps:抛弃 Mat Line 模式，

	std::shared_ptr<Vec2>		_curve_vec;		//	曲线数据映射的坐标点 映射范围 [0:len,min,max]
	//Vec_Vec3					_curve_colour;	//	曲线数据颜色
	std::shared_ptr<GLuint>		_curve_indexs;	//	vbo需要的渲染顺序索引

	Vec_Vec2					_vec_color;
	std::vector<RealType>		_gray;
	Vec_Vec3					_colour;

	std::vector<GLuint>			_indexs_color;
	uint32_t					vao[2];
	uint32_t					vbo[6];
public:
	int32_t						_nums;			//	曲线数量
	int32_t						_len;			//	曲线节点数量
	RealType					_data_min;		//	数据中最小的
	RealType					_data_max;		//	数据中最大的
	std::vector<Vec2>			_curve_max_min_ary;
public:
	DataDrawGL():buff_len(20000),buff_nums(200){
		_curve_indexs.reset(new GLuint[buff_len*buff_nums], std::default_delete<GLuint[]>());
		_curve_vec.reset(new Vec2[buff_len*buff_nums], std::default_delete<Vec2[]>());
		vao[0] = vao[1] = 0;
		vbo[0] = vbo[1] = vbo[2] = vbo[3] = vbo[4] = vbo[5] = 0;
		create_curve_buff();
	}
	~DataDrawGL() {
		if (vbo[0] != 0) {
			glDeleteBuffers(3, vbo);
			vbo[0] = vbo[1] = vbo[2] = 0;
		}
		if (vao[0] != 0) {
			glDeleteVertexArrays(1, &vao[0]);
		}
	}
public:
	void Open(const std::string&);
	void DrawCurve(int index);

	// 将ascii数据保存为 二进制数据 ,二进制数据结构具体写在 该函数的实现中;
	static void NoramlData2BinData(const std::string &, const std::string &);

private:
	


	static void mat_transpose(const Mat&, Mat&);

	void create_curve_data();
	void create_curve_vbo();
	void create_curve_buff();


	void load_bin_data(const std::string&);
};
#endif