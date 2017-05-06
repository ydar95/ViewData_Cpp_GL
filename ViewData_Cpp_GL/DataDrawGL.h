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
	uint32_t buff_block_nums;
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
	std::shared_ptr<RealType>	_data;			//	ps:���� Mat Line ģʽ��

	/// curve
	std::shared_ptr<Vec2>		_curve_vec;		//	��������ӳ�������� ӳ�䷶Χ [0:len,min,max]
	//Vec_Vec3					_curve_colour;	//	����������ɫ
	std::shared_ptr<GLuint>		_curve_indexs;	//	vbo��Ҫ����Ⱦ˳������


	/// block	�� ��ɫͼ&��ɫͼ ��Ϊblock , ��ɫ rgb ,��ɫ gray
	std::shared_ptr<Vec3>		_block_rgb;
	std::shared_ptr<RealType>	_block_gray;	
	std::shared_ptr<Vec2>		_block_position;//����
	std::shared_ptr<GLuint>		_block_indexs;

	uint32_t					vao[2];
	uint32_t					vbo[6];
public:
	int32_t						_nums;			//	��������
	int32_t						_len;			//	���߽ڵ�����
	RealType					_data_min;		//	��������С��
	RealType					_data_max;		//	����������
	std::vector<Vec2>			_curve_max_min_ary;
public:
	DataDrawGL():buff_len(20000),buff_nums(200){
		_curve_indexs.reset(new GLuint[buff_len*buff_nums], std::default_delete<GLuint[]>());
		_curve_vec.reset(new Vec2[buff_len*buff_nums], std::default_delete<Vec2[]>());
		vao[0] = vao[1] = 0;
		vbo[0] = vbo[1] = vbo[2] = vbo[3] = vbo[4] = vbo[5] = 0;
		create_curve_buff();
		
		// һ���� һ����ɫ
		_block_rgb.reset(new Vec3[buff_len*buff_nums], std::default_delete<Vec3[]>());
		_block_gray.reset(new RealType[buff_len*buff_nums], std::default_delete<RealType[]>());
		// һ����һ������
		_block_position.reset(new Vec2[buff_len*buff_nums], std::default_delete<Vec2[]>());
		// ��ǰ����������������
		buff_block_nums = (buff_len - 1)*(buff_nums - 1);
		_block_indexs.reset(new GLuint[buff_block_nums], std::default_delete<GLuint[]>());
		
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

	// ��ascii���ݱ���Ϊ ���������� ,���������ݽṹ����д�� �ú�����ʵ����;
	static void NoramlData2BinData(const std::string &, const std::string &);

private:
	


	static void mat_transpose(const Mat&, Mat&);

	// ����
	void create_curve_data();
	void create_curve_vbo();
	void create_curve_buff();

	// �Ҷ�ͼ & ��ɫͼ , �������������� ��ɫ���鲻ͬ...
	void create_block_data();
	void create_block_vbo();
	void create_block_buff();

	void load_bin_data(const std::string&);
};
#endif