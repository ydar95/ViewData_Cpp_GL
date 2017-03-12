/*
	���������һ��dome �����ӽǲ������������
	���������ݴ���ĺ���
 */


#ifndef _YDAR_HEAD_WINDOWS_CONTROL_H_
#define _YDAR_HEAD_WINDOWS_CONTROL_H_ 1

#include<gl/glew.h>
//��� ����ʱȱ��glew������ ��ʹ�������������
//#pragma comment(lib, "glew32.lib")

class WindowControl {
public:
	struct Point2d
	{
		GLdouble x;
		GLdouble y;
		Point2d(GLdouble x = 0.0, GLdouble y = 0.0) {
			this->x = x;
			this->y = y;
		}
	};
	//private:
public:
	GLdouble	_x_min;
	GLdouble	_x_max;
	GLdouble	_y_min;
	GLdouble	_y_max;

	GLdouble	_width;
	GLdouble	_height;

	GLdouble	_z_min;
	GLdouble	_z_max;
public:
	WindowControl() {  }
	///	���캯��
	WindowControl(GLdouble width,GLdouble height, GLdouble x_min = -1.0, GLdouble x_max = 1.0, GLdouble y_min = -1.0, GLdouble y_max = 1.0);

	///	�޸���������ϵ�Ŀɼ�����
	void Ortho(GLdouble x_min, GLdouble x_max, GLdouble y_min, GLdouble y_max, GLdouble z_min=-1.0, GLdouble z_max=1.0);

	///	�޸Ĵ��ڷֱ���
	void Reshape(GLdouble width, GLdouble height);

	///	����������ϵ��ƽ��
	void Move(GLdouble r_x, GLdouble r_y);

	///	�ڴ�������ϵ��ƽ��
	void Move(GLint x0, GLint y0, GLint x1, GLint y1);

	/// ��(x,y)Ϊ��������,s_x,s_y�ǷŴ���,С��1.0��С,����1.0�Ŵ�,����1.0�ߴ粻��
	void Scale(int x, int y, double s_x, double s_y);

	/// ���봰������,���ض�Ӧ����������ϵ����
	WindowControl::Point2d GetWorldPoint2d(GLdouble w_x, GLdouble w_y);

private:

	/// �� Ortho �� Reshape ����,ʹ�ı��Ч����Ч
	void _UpdataOrtho();
};


#endif // _YDAR_HEAD_WINDOWS_CONTROL_H_
