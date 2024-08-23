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
		//压力速度耦合算法
		void simple();
		inline int faceid(int, int, int);
		//virtual void write() override;
		//
	private:
		void pgrad(Point&, int, int, Point&, Point&, double);
		//vector<Element_cvfem>mesh_eles;//单元
		//vector<Point>mesh_nodes;//节点
		//vector<vector<int>>mesh_nei;//邻居
		//vector<vector<int>>mesh_neihaxi;//快速检索邻居节点哈希表
		//vector<vector<int>>mesh_bj;//边界三角形网格
		//vector<Point>mesh_gradient;//单元的梯度
		map<int, vector<int>>sharedface;
		vector<vector<gradcon>>Gradelements;
		vector<gradvalue>grad;
	};
}
#endif