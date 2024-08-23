#ifndef CVFEM_H
#define CVFEM_H
#include"cvfem/Mesh_cvfem.h"
#include<map>
#include <algorithm>
//1.over-relaxed: Ef = Sf * Sf / (ecf * Sf) * ecf
//
namespace FVM {
	struct gradvalue {
		Point C, f1, f2, f3, f4;
		int idf1 = -1, idf2 = -1, idf3 = -1, idf4 = -1;
	};
	struct gradcon {
		Point data;
		int id;
	};
	class Fvm :public Mesh_cvfem {
	public:
		void findsharedface();
		void fvm_diff();
		void cal();
		void cal_gradient();
		void boundary();
		//ѹ���ٶ�����㷨
		void simple();
		inline int faceid(int, int, int);
		//virtual void write() override;
		//
	private:
		void pgrad(Point&, int, int, Point&, Point&, double);
		//vector<Element_cvfem>mesh_eles;//��Ԫ
		//vector<Point>mesh_nodes;//�ڵ�
		//vector<vector<int>>mesh_nei;//�ھ�
		//vector<vector<int>>mesh_neihaxi;//���ټ����ھӽڵ��ϣ��
		//vector<vector<int>>mesh_bj;//�߽�����������
		//vector<Point>mesh_gradient;//��Ԫ���ݶ�
		map<int, vector<int>>sharedface;
		vector<vector<gradcon>>Gradelements;
		vector<gradvalue>grad;
	};
}
#endif