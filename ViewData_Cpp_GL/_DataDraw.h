#ifndef _HEAD_DATA_DRAW_H_
#define _HEAD_DATA_DRAW_H_ 1

#include<string>
#include<vector>
#include<gl/freeglut.h>
using namespace std;
class DataDraw{
public:
	typedef vector<double>	Line;	//һ��
	typedef vector<Line>	Mat;	//һ������
	struct Point2d{
		double x;
		double y;
		Point2d(double _x=0.0, double _y=0.0) {
			x = _x;
			y = _y;
		}
	};
private:
	Mat _mtx;				//����
	//double	_x_once_scale;	//x��ÿ�ηŴ���
	//double	_y_once_scale;	//y��ÿ�ηŴ���
	double	_x_min;
	double	_x_max;
	double	_y_min;
	double  _y_max;
	double  _x_real_scale;
	double  _y_real_scale;
	int		_width;			//��ʵ�������Ĵ��ڳߴ�
	int		_height;		//��ʵ�������Ĵ��ڳߴ�

	double	_data_min;		//��������С��
	double	_data_max;		//����������

	GLuint	_gl_list_id;	//glNewList ���ɵ�id

	//�������ߵ���ֵ�����ֵ����x�У���Сֵ����y��
	vector<Point2d>  _curve_max_min_ary;
public:
	DataDraw();
	DataDraw(int ,int);		
	DataDraw(const string&, int, int);
	DataDraw(const Mat&, int, int);
public:
	//  ���ļ�
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

	//�õ�ȫ�����ߵ���������Сֵ�����ֵ����x�У���Сֵ����y��
	void getAllCurveMaxMin();
};


#endif // !_HEAD_VIEW_DATA_H_
