/*

 */


#ifndef _YDAR_HEAD_DATA_DRAW_GL_H_
#define _YDAR_HEAD_DATA_DRAW_GL_H_ 1

#include<vector>
#include<string>
#include"WindowControl.h"
class DataDrawGL
{
private:
	enum VBO_ARRAY_INDEX
	{
		enum_curve = 0,
		enum_color ,
		enum_gary 
	};
	const int DRAW_NUM = 1;
public:
	struct Vec3d{
		GLdouble x;
		GLdouble y;
		GLdouble z;
		Vec3d(GLdouble _x = 0.0, GLdouble _y = 0.0, GLdouble _z = 0.0) {
			x = _x;
			y = _y;
			z = _z;
		}
	};
	typedef std::vector<double>	Line;	//一行
	typedef std::vector<Line>	Mat;	//一个矩阵
	typedef std::vector<Vec3d>	Vec_Vec3d;
	typedef WindowControl::Point2d Point2d;
	typedef std::vector<Point2d>	Vec_Vec2d;
//private:
	Mat _mtx;
private:
	Vec_Vec2d _vec;
	Vec_Vec2d _vec_color;
	Vec_Vec3d _gray;
	Vec_Vec3d _colour;
	Vec_Vec3d _curve_colour;
	std::vector<GLuint> _indexs_curve;
	std::vector<GLuint> _indexs_color;
	uint32_t vao[2];
	uint32_t vbo[6];
public:
	GLdouble	_data_min;		//数据中最小的
	GLdouble	_data_max;		//数据中最大的
	std::vector<Point2d>  _curve_max_min_ary;

public:
	DataDrawGL(){}
	DataDrawGL(const std::string&);
public:
	void Open(const std::string&);
	void DrawCurve(int index);
private:
	void load_data(const std::string&);
	void mat_transpose(const Mat&, Mat&);
	void get_all_curve_maxmin();
	void create_vec();
	void create_vbo_curve();
};
#endif