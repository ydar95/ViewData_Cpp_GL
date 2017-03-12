#ifndef _HEAD_DATA_DRAW_H_
#define _HEAD_DATA_DRAW_H_ 1

#include<string>
#include<vector>
#include<gl/freeglut.h>
using namespace std;
class DataDraw{
public:
	typedef vector<double>	Line;	//一行
	typedef vector<Line>	Mat;	//一个矩阵
	struct Point2d{
		double x;
		double y;
		Point2d(double _x=0.0, double _y=0.0) {
			x = _x;
			y = _y;
		}
	};
private:
	Mat _mtx;				//数据
	//double	_x_once_scale;	//x轴每次放大倍数
	//double	_y_once_scale;	//y轴每次放大倍数
	double	_x_min;
	double	_x_max;
	double	_y_min;
	double  _y_max;
	double  _x_real_scale;
	double  _y_real_scale;
	int		_width;			//现实中真正的窗口尺寸
	int		_height;		//现实中真正的窗口尺寸

	double	_data_min;		//数据中最小的
	double	_data_max;		//数据中最大的

	GLuint	_gl_list_id;	//glNewList 生成的id

	//保存曲线的最值，最大值放在x中，最小值放在y中
	vector<Point2d>  _curve_max_min_ary;
public:
	DataDraw();
	DataDraw(int ,int);		
	DataDraw(const string&, int, int);
	DataDraw(const Mat&, int, int);
public:
	//  打开文件
	bool open(const string&);
	void move(double, double);
	void move(int, int, int, int);
	void scale(int,int,double, double);
	void reshape(int, int);
	GLuint getListId();
	void create_gl_list();
	Point2d getWorldPoint(int, int);
	void updataOrtho2D();
private:
	void create_init();
	bool load_data(Mat &mtx,const string&);
	void mat_transpose(const Mat&, Mat&);

	//得到全部曲线的最大和者最小值，最大值放在x中，最小值放在y中
	void getAllCurveMaxMin();
};


#endif // !_HEAD_VIEW_DATA_H_
