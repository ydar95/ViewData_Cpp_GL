/*
	这个类是上一个dome 将对视角操作给分离出来
	不包含数据处理的函数
 */


#ifndef _YDAR_HEAD_WINDOWS_CONTROL_H_
#define _YDAR_HEAD_WINDOWS_CONTROL_H_ 1

#include<gl/glew.h>
//如果 编译时缺少glew的链接 ，使用下面这条语句
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
	///	构造函数
	WindowControl(GLdouble width,GLdouble height, GLdouble x_min = -1.0, GLdouble x_max = 1.0, GLdouble y_min = -1.0, GLdouble y_max = 1.0);

	///	修改世界坐标系的可见坐标
	void Ortho(GLdouble x_min, GLdouble x_max, GLdouble y_min, GLdouble y_max, GLdouble z_min=-1.0, GLdouble z_max=1.0);

	///	修改窗口分辨率
	void Reshape(GLdouble width, GLdouble height);

	///	在世界坐标系的平移
	void Move(GLdouble r_x, GLdouble r_y);

	///	在窗口坐标系的平移
	void Move(GLint x0, GLint y0, GLint x1, GLint y1);

	/// 以(x,y)为缩放中心,s_x,s_y是放大倍率,小于1.0缩小,大于1.0放大,等于1.0尺寸不变
	void Scale(int x, int y, double s_x, double s_y);

	/// 传入窗口坐标,返回对应的世界坐标系坐标
	WindowControl::Point2d GetWorldPoint2d(GLdouble w_x, GLdouble w_y);

private:

	/// 给 Ortho 和 Reshape 调用,使改变的效果起效
	void _UpdataOrtho();
};


#endif // _YDAR_HEAD_WINDOWS_CONTROL_H_
