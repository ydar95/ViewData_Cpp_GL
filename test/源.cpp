#include<iostream>
#include<iomanip>
#include"CircularQueue.h"
using namespace std;
CircularQueue<int> cq(10, 10);

int main() {
	for (int i = 0; i <= 10; i++) {
		cq[i] = i;
		cq[-i] = -i;
	}
	for (int i = 0; i < 21; i++) {
		cout << setfill(' ') << setw(3) << cq[i] << "  ";
	}
	cout << endl;
	for (int i = 11; i <= 31; i++) {
		cq.moveRight();
		//cq.getPrepareUpdataObject() = i;
		for (int i = 0; i < 21; i++) {
			cout << setfill(' ') << setw(3)<<cq[i] << "  ";
		}
		cout << endl;
	}
	return 0;
}