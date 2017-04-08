#if 1
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<vector>
#include<string>
#include<iostream>
#include<memory>
#include"DataDrawGL.h"
#include"WindowControl.h"
#include"Count.h"
#include"CircularQueue.h"
using namespace std;

struct Node
{
	DataDrawGL dd;
	int32_t begin;
	int32_t end;
};
std::shared_ptr<CircularQueue<Node>> ptr;


//vector <DataDrawGL> dd_ary;
WindowControl wc;

//鼠标
int mouse_button;
int mouse_state;
int mouse_last_x;
int mouse_last_y;
int display_index=0;

void reshape(int w, int h) {
	wc.Reshape(w, h);
}

void display() {
	auto t = CTime::GetMicroS();
	glClear(GL_COLOR_BUFFER_BIT);
	double bias_x = 0;
	for (int i = 0; i < 21; i++) {
		glPushMatrix();	//在当前 gl 的状态上 复制一个新的状态 
		glLoadIdentity();// 初始化矩阵
		// 平移坐标系
		if (!(((*ptr.get())[i].end) < wc._x_min // 
			|| ((*ptr.get())[i].begin) > wc._x_max)
			) {
			glTranslated((*ptr.get())[i].begin, 0, 0);
			(*ptr.get())[i].dd.DrawCurve(display_index + 1);
		}
		glPopMatrix();
	}
	glFinish();
	t = CTime::GetMicroS() - t;
	cout << "display(us):\t" << t << endl;
}
void move(int x, int y) {
	//cout << "x:" << x << "\t, " << "y:" << y << endl;
	static int inc = 0;
	if (mouse_button == 0 && mouse_state == 0) {
		wc.Move(mouse_last_x, mouse_last_y, x, y);
		mouse_last_x = x;
		mouse_last_y = y;
		int begin = (*ptr.get())[0].begin;
		int end   = (*ptr.get())[0].end;
		if (wc._x_min > end) {
			ptr->moveRight();
			ptr->getPrepareUpdataObject().dd.Open("../1.bin");
			ptr->getPrepareUpdataObject().begin = (*ptr.get())[9].end;
			ptr->getPrepareUpdataObject().end = ptr->getPrepareUpdataObject().begin + ptr->getPrepareUpdataObject().dd._len;
		}
		else if (wc._x_max < begin) {
			ptr->moveLeft();
			ptr->getPrepareUpdataObject().dd.Open("../1.bin");
			ptr->getPrepareUpdataObject().end = (*ptr.get())[-9].begin;
			ptr->getPrepareUpdataObject().begin = ptr->getPrepareUpdataObject().end - ptr->getPrepareUpdataObject().dd._len;
		}


		display();
	}
}
void mouse(int button, int state, int x, int y) {
	double s = 1.1;
	double _s = 1.0 / s;
	mouse_button = button;
	mouse_state = state;
	if (button == 0 && state == 0) {
		mouse_last_x = x;
		mouse_last_y = y;
	}else if (button == 3 && state == 1) {
		wc.Scale(x, y, s, s);
		display();
	}else if (button == 4 && state == 1) {
		wc.Scale(x, y,_s, _s);
		display();
	}
	else if (button == 2 && state == 0) {
		auto p = wc.GetWorldPoint2d(x, y);
		cout << "x:" << p.x << "\t y:" << p.y << endl;
	}
}
int  main(int argc, char *argv[]) {
	int w = 1280;int h = 720;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(w, h);
	glutCreateWindow("View");
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutDisplayFunc(display);
	glutKeyboardUpFunc([](unsigned char key, int x, int y) {if (char(key) == 'z') { display_index ^= 1;display(); }});
	printf("%s\n", glGetString(GL_VENDOR));
	//DataDrawGL::NoramlData2BinData("../1.txt", "../1.bin");
	
	ptr.reset(new CircularQueue<Node>());
	CTIME_COUNT_MILLIS({
	(*ptr.get())[0].dd.Open("../1.bin");
	(*ptr.get())[0].begin = 0;
	(*ptr.get())[0].end = (*ptr.get())[0].dd._len;
	for (int i = 1; i <= 10; i++) {
		(*ptr.get())[i].dd.Open("../1.bin");
		(*ptr.get())[i].begin = (*ptr.get())[i - 1].end;
		(*ptr.get())[i].end = (*ptr.get())[i].begin + (*ptr.get())[i].dd._len;

		(*ptr.get())[-i].dd.Open("../1.bin");
		(*ptr.get())[-i].end = (*ptr.get())[-i + 1].begin;
		(*ptr.get())[-i].begin = (*ptr.get())[-i].end - (*ptr.get())[-i].dd._len;
	}
	});
	wc = WindowControl(w, h, 0, (*ptr.get())[0].dd._len, (*ptr.get())[0].dd._data_min, (*ptr.get())[0].dd._data_max);
	glutMainLoop();
	return 0;
}
#endif