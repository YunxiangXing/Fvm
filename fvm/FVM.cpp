#include"fvm/FVM.h"
using namespace FVM;
void Fvm::pgrad(Point& g, int id, int i, Point& df, Point& Sf,double y) {
	if (id != -1) {
		gradcon temp;
		temp.data = 1.0 / mesh_eles[i].getvol() * (0.5 * df * g + y) * Sf;
		temp.id = id;
		Gradelements[i].push_back(temp);
	}
}
void Fvm::cal_gradient() {
	//1.计算全局的未知梯度系数
	//每个单元只涉及五个未知系数
	//step1: all fai
	grad.resize(mesh_eles.size());
	Gradelements.resize(mesh_eles.size());

	for (int i = 0; i < grad.size(); i++) {
		vector<Point>Sf = mesh_eles[i].getelementsurfacenormal();
		double Vc = mesh_eles[i].getvol();
		uint64_t id[4];
		id[0] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numC);
		id[1] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numD);
		id[2] = faceid(mesh_eles[i].numA, mesh_eles[i].numC, mesh_eles[i].numD);
		id[3] = faceid(mesh_eles[i].numB, mesh_eles[i].numC, mesh_eles[i].numD);
		grad[i].C = 1.0 / (2 * Vc) * (Sf[0] + Sf[1] + Sf[2] + Sf[3]);//C系数,id = i

		grad[i].f1 = 1.0 / (2 * Vc) * Sf[0];//f1, id = map
		if (sharedface[id[0]][1] == -1) grad[i].idf1 = -1;
		else
			if (sharedface[id[0]][0] == i) grad[i].idf1 = sharedface[id[0]][1];
			else grad[i].idf1 = sharedface[id[0]][0];

		grad[i].f2 = 1.0 / (2 * Vc) * Sf[1];
		if (sharedface[id[1]][1] == -1) grad[i].idf2 = -1;
		else
			if (sharedface[id[1]][0] == i) grad[i].idf2 = sharedface[id[1]][1];
			else grad[i].idf2 = sharedface[id[1]][0];

		grad[i].f3 = 1.0 / (2 * Vc) * Sf[2];
		if (sharedface[id[2]][1] == -1) grad[i].idf3 = -1;
		else
			if (sharedface[id[2]][0] == i) grad[i].idf3 = sharedface[id[2]][1];
			else grad[i].idf3 = sharedface[id[2]][0];

		grad[i].f4 = 1.0 / (2 * Vc) * Sf[3];
		if (sharedface[id[3]][1] == -1) grad[i].idf4 = -1;
		else
			if (sharedface[id[3]][0] == i) grad[i].idf4 = sharedface[id[3]][1];
			else grad[i].idf4 = sharedface[id[3]][0];
	}
	
	//step2: update
	for (int i = 0; i < grad.size(); i++) {
		vector<Point>Sf = mesh_eles[i].getelementsurfacenormal();
		Point df[4], rf[4], rC, rF[4];
		rC = mesh_eles[i].getcent();//当前单元质心
		if (grad[i].idf1 != -1) {
			rF[0] = mesh_eles[grad[i].idf1].getcent();//周围单元质心
			rf[0] = mesh_eles[i].getface_ABC();//共享面面心
		}
		if (grad[i].idf2 != -1) { 
			rF[1] = mesh_eles[grad[i].idf2].getcent();
			rf[1] = mesh_eles[i].getface_ABD();
		}
		if (grad[i].idf3 != -1) { 
			rF[2] = mesh_eles[grad[i].idf3].getcent();
			rf[2] = mesh_eles[i].getface_ACD();
		}
		if (grad[i].idf4 != -1) { 
			rF[3] = mesh_eles[grad[i].idf4].getcent();
			rf[3] = mesh_eles[i].getface_BCD();
		}
		df[0] = rf[0] - 0.5 * (rC + rF[0]);
		df[1] = rf[1] - 0.5 * (rC + rF[1]);
		df[2] = rf[2] - 0.5 * (rC + rF[2]);
		df[3] = rf[3] - 0.5 * (rC + rF[3]);
		//gradC
		for (int j = 0; j < 4; j++) {
			pgrad(grad[i].C, i, i, df[j], Sf[j], 0.5);
			pgrad(grad[i].f1, grad[i].idf1, i, df[j], Sf[j], 0.0);
			pgrad(grad[i].f2, grad[i].idf2, i, df[j], Sf[j], 0.0);
			pgrad(grad[i].f3, grad[i].idf3, i, df[j], Sf[j], 0.0);
			pgrad(grad[i].f4, grad[i].idf4, i, df[j], Sf[j], 0.0);
		}

		if (grad[i].idf1 != -1) {
			//gradF-f1
			pgrad(grad[grad[i].idf1].C, grad[i].idf1, i, df[0], Sf[0], 0.5);
			pgrad(grad[grad[i].idf1].f1, grad[grad[i].idf1].idf1, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf1].f2, grad[grad[i].idf1].idf2, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf1].f3, grad[grad[i].idf1].idf3, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf1].f4, grad[grad[i].idf1].idf4, i, df[0], Sf[0], 0.0);
		}

		if (grad[i].idf2 != -1) {
			//gradF-f2
			pgrad(grad[grad[i].idf2].C, grad[i].idf2, i, df[0], Sf[0], 0.5);
			pgrad(grad[grad[i].idf2].f1, grad[grad[i].idf2].idf1, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf2].f2, grad[grad[i].idf2].idf2, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf2].f3, grad[grad[i].idf2].idf3, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf2].f4, grad[grad[i].idf2].idf4, i, df[0], Sf[0], 0.0);
		}

		if (grad[i].idf3 != -1) {
			//gradF-f3
			pgrad(grad[grad[i].idf3].C, grad[i].idf3, i, df[0], Sf[0], 0.5);
			pgrad(grad[grad[i].idf3].f1, grad[grad[i].idf3].idf1, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf3].f2, grad[grad[i].idf3].idf2, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf3].f3, grad[grad[i].idf3].idf3, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf3].f4, grad[grad[i].idf3].idf4, i, df[0], Sf[0], 0.0);
		}

		if (grad[i].idf4 != -1) {
			//gradF-f4
			pgrad(grad[grad[i].idf4].C, grad[i].idf4, i, df[0], Sf[0], 0.5);
			pgrad(grad[grad[i].idf4].f1, grad[grad[i].idf4].idf1, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf4].f2, grad[grad[i].idf4].idf2, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf4].f3, grad[grad[i].idf4].idf3, i, df[0], Sf[0], 0.0);
			pgrad(grad[grad[i].idf4].f4, grad[grad[i].idf4].idf4, i, df[0], Sf[0], 0.0);
		}

	}
}
void Fvm::findsharedface() {
	uint64_t num = mesh_nodes.size();
	for (int i = 0; i < mesh_eles.size(); i++) {
		vector<uint64_t>temp;
		vector<int>data;
		data.resize(2);
		data[1] = -1;
		temp.push_back(mesh_eles[i].numA);
		temp.push_back(mesh_eles[i].numB);
		temp.push_back(mesh_eles[i].numC);
		temp.push_back(mesh_eles[i].numD);
		sort(temp.begin(), temp.end());
		uint64_t id[4];
		uint64_t m = temp[3] * num * num;
		id[0] = temp[0] + temp[1] * num + temp[2] * num * num;
		id[1] = temp[0] + temp[1] * num + temp[3] * num * num;
		id[2] = temp[0] + temp[2] * num + temp[3] * num * num;
		id[3] = temp[1] + temp[2] * num + temp[3] * num * num;
		data[0] = i;
		for (int j = 0; j < 4; j++) {
			auto result = sharedface.insert({ id[j],data });
			if (!result.second) {
				//已经存放过
				auto temp = sharedface[id[j]];
				temp[1] = data[0];
				sharedface[id[j]] = temp;
			}
		}
	}
	for (int i = 0; i < mesh_bj.size(); i++) {
		uint64_t id = faceid(mesh_bj[i][0], mesh_bj[i][1], mesh_bj[i][2]);
		auto r = sharedface.find(id);
		if (r == sharedface.end()) {
			cout << "Error in " << __LINE__ << endl;
		}
		if (sharedface[id][1] != -1) {
			cout << "Error in " << __LINE__ << endl;
		}
	}
}
inline uint64_t Fvm::faceid(uint64_t a, uint64_t b, uint64_t c) {
	vector<uint64_t>temp;
	temp.push_back(a);
	temp.push_back(b);
	temp.push_back(c);
	sort(temp.begin(), temp.end());
	uint64_t num = mesh_nodes.size();
	uint64_t id = temp[0] + temp[1] * num + temp[2] * num * num;
	return id;
}
void Fvm::init(string cwd) {

	readRmsh(cwd);
	//findneighbor();
	findsharedface();
	cal_gradient();

	ap.clear();
	anb.clear();
	bp.clear();
	ap.resize(mesh_eles.size());
	anb.resize(ap.size());
	bp.resize(ap.size());
	for (int i = 0; i < ap.size(); i++) {
		ap[i] = 0.0;
		bp[i] = 0.0;
		anb[i].resize(ap.size());
		for (int j = 0; j < ap.size(); j++) {
			anb[i][j] = 0.0;
		}
	}

	for (int i = 0; i < mesh_bj.size(); i++) {
		uint64_t id = faceid(mesh_bj[i][0], mesh_bj[i][1], mesh_bj[i][2]);
		fvm_bj.insert({ id, mesh_bj[i][3] });
	}
}
void Fvm::cal_Diff(string cwd) {
	//计算单元面与哪个单元连接：
	//1. 建立单元面哈希表

	init(cwd);
	for (int i = 0; i < mesh_eles.size(); i++) {
		vector<Point>Sf = mesh_eles[i].getelementsurfacenormal();
		uint64_t id[4];
		id[0] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numC);
		id[1] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numD);
		id[2] = faceid(mesh_eles[i].numA, mesh_eles[i].numC, mesh_eles[i].numD);
		id[3] = faceid(mesh_eles[i].numB, mesh_eles[i].numC, mesh_eles[i].numD);
		Point f[4];
		f[0] = mesh_eles[i].getface_ABC();
		f[1] = mesh_eles[i].getface_ABD();
		f[2] = mesh_eles[i].getface_ACD();
		f[3] = mesh_eles[i].getface_BCD();
		//step1: 判断是内部面还是外部面
		double gDiff[4] = { 0.0,0.0,0.0,0.0 };
		for (int j = 0; j < 4; j++) {
			if (sharedface[id[j]][1] == -1) {
				//边界条件 id[j]
				if (fvm_bj[id[j]] == 1 || fvm_bj[id[j]] == 3) {
					Point ecb = f[j] - mesh_eles[i].getcent();
					double dcb = norm(ecb);
					ecb = 1.0 / dcb * ecb;
					Point Eb = (Sf[j] * Sf[j]) / (ecb * Sf[j]) * ecb;
					gDiff[j] = norm(Eb) / dcb;
					Point Tb = Sf[j] - Eb;
					if (fvm_bj[id[j]] == 1) bp[i] += gDiff[j] * 1.0;
					else bp[i] += gDiff[j] * 0.0;
				}
			}
			else {
				int share;
				if (sharedface[id[j]][0] != i) share = sharedface[id[j]][0];
				else share = sharedface[id[j]][1];
				Point CF = mesh_eles[share].getcent() - mesh_eles[i].getcent();
				double Dcf = norm(CF);
				Point ecf = 1.0 / Dcf * CF;
				double Ef = norm((Sf[j] * Sf[j]) / (ecf * Sf[j]) * ecf);
				gDiff[j] = Ef / Dcf;
				anb[i][share] += -gDiff[j];
				//gradfai_f * Tf
				//gradfai_f = g * fai_c + (1 - g) * fai_F
				Point Tf;
				Point dCfi, dfiF;
				Tf = Sf[j] - Ef;
				dCfi = f[j] - mesh_eles[i].getcent();
				dfiF = mesh_eles[share].getcent() - f[j];
				float gfi = norm(dCfi) / (norm(dCfi) + norm(dfiF));
				//grad_ffi = gfi * grad_fF + (1 - gfi) * grad_fC
				Point grad_ffi;
				for (int k = 0; k < Gradelements[share].size(); k++) {
					anb[i][Gradelements[share][k].id] += gfi * Gradelements[share][k].data * Tf;
				}
				for (int k = 0; k < Gradelements[i].size(); k++) {
					anb[i][Gradelements[i][k].id] += (1.0 - gfi) * Gradelements[i][k].data * Tf;
				}
			}
		}
		ap[i] += gDiff[0] + gDiff[1] + gDiff[2] + gDiff[3];
	}

	//AX = b
	for (int i = 0; i < ap.size(); i++) {
		for (int j = 0; j < ap.size(); j++) {
			if (i == j) {
				anb[i][j] += ap[i];
			}
		}
	}
	auto res = solver_equtionGaussSeidel(anb, bp);

	WritingRes(res);
}