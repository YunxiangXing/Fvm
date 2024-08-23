#ifndef MESH_H
#define MESH_H
#include"base\base.h"
#include<QtWidgets>
using namespace std;
class Mesh{
public:
	Mesh(int dim1,int dim2,int dim3,QTextEdit *text):dim1(dim1),dim2(dim2),dim3(dim3),textedit(text) {
		point.resize(dim1);
		for (int i = 0; i < dim1; i++) {
			point[i].resize(dim2);
			for (int j = 0; j < dim2; j++) {
				point[i][j].resize(dim3);
			}
		}
		textedit->append("Size of build Mesh Matix: " + QString::number(dim1) + "*" + QString::number(dim2) + "*" + QString::number(dim3));
	}
	Mesh() {
		point.resize(dim1);
		for (int i = 0; i < dim1; i++) {
			point[i].resize(dim2);
			for (int j = 0; j < dim2; j++) {
				point[i][j].resize(dim3);
			}
		}
	};
	~Mesh() {
		textedit->append("Delete Mesh Matix");
	}
	void setdim123(int dimX,int dimY,int dimZ);
	bool reading(string);
	bool output(string);
	void sphere(double cirx, double ciry, double cirz, double R);
	void setQTextedit(QTextEdit *Text);
	vector<vector<vector<int>>>& getpoint();
	int getdim1();
	int getdim2();
	int getdim3();
	QTextEdit* gettextedit();
	double getpolor();
private:
	void buildpoint();
	double polor();
	int dim1 = 0;
	int dim2 = 0;
	int dim3 = 0;
	string dir;
	QTextEdit* textedit = NULL;
	vector<vector<vector<int>>> point;
};
enum class bj {
	Not_define = -1,
	Zhou_He = 0,
	Zhou_qi = 1
};
enum class col {
	Not_define = -1,
	SRT = 0,
	MRT = 1
};
enum class inlet {
	x = 0,
	y = 1,
	z = 2
};
#endif
