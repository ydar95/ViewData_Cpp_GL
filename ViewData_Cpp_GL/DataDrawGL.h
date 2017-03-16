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
	std::shared_ptr<RealType>	data_;			//	ps:���� Mat Line ģʽ��

	Vec_Vec2					_curve_vec;		//	��������ӳ�������� ӳ�䷶Χ [0:len,min,max]
	Vec_Vec3					_curve_colour;	//	����������ɫ
	std::vector<GLuint>			_indexs_curve;	//	vbo��Ҫ����Ⱦ˳������

	Vec_Vec2					_vec_color;
	std::vector<RealType>		_gray;
	Vec_Vec3					_colour;

	std::vector<GLuint>			_indexs_color;
	uint32_t					vao[2];
	uint32_t					vbo[6];
public:
	GLdouble					_data_min;		//��������С��
	GLdouble					_data_max;		//����������
	std::vector<Vec2>			_curve_max_min_ary;
public:
	DataDrawGL() {}
	DataDrawGL(const std::string&bin_filename) { Open(bin_filename);}
public:
	void Open(const std::string&);
	void DrawCurve(int index);

	// ��ascii���ݱ���Ϊ ���������� ,���������ݽṹ����д�� �ú�����ʵ����;
	static void NoramlData2BinData(const std::string &, const std::string &);

private:
	static void mat_transpose(const Mat&, Mat&);

	void create_curve_data();
	void create_vbo_curve();
	void load_bin_data(const std::string&);
};
#endif