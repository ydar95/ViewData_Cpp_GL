#if 1
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<vector>
#include<string>
#include<iostream>
//#include"DataDrawGL.h"
#include"WindowControl.h"
#include"Count.h"
using namespace std;

//DataDrawGL dd;
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
	/*
	for (int i = 0; i < dd_ary.size(); i++) {
		glPushMatrix();	//在当前 gl 的状态上 复制一个新的状态 
		glLoadIdentity();// 初始化矩阵
		glTranslated(bias_x, 0, 0);	// 平移坐标系
		if(!((bias_x+ dd_ary[i]._mtx[0].size())<wc._x_min // 
			|| bias_x > wc._x_max
			))
			dd_ary[i].DrawCurve(display_index + 1);
		bias_x += dd_ary[i]._mtx[0].size();
		glPopMatrix();
	}
	*/
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

	auto t0 = CTime::GetMicroS();
	//DataDrawGL::NoramlData2BinData("../1.txt", "../1.bin");
	//dd.Open("../1.bin");
	auto t1 = CTime::GetMicroS();
	cout << t1 - t0 << endl;
	//for (int i = 0; i < 10; i++)
		//dd_ary.push_back(dd);
	wc = WindowControl(w, h, 0, 1000, 0, 1);
	glutMainLoop();
	return 0;

}
#endif