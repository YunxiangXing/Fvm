#include"cvfem/FVM.h"
using namespace FVM;
void Fvm::pgrad(Point& g, int id, int i, Point& df, Point& Sf,double y) {
	gradcon temp;
	temp.data = 1.0 / mesh_eles[i].getvol() * (0.5 * df * g + y) * Sf;
	temp.id = id;
	Gradelements[i].push_back(temp);
}
void Fvm::cal_gradient() {
	//1.����ȫ�ֵ�δ֪�ݶ�ϵ��
	//ÿ����Ԫֻ�漰���δ֪ϵ��
	//step1: all fai
	grad.resize(mesh_eles.size());
	Gradelements.resize(mesh_eles.size());

	for (int i = 0; i < grad.size(); i++) {
		vector<Point>Sf = mesh_eles[i].getelementsurfacenormal();
		double Vc = mesh_eles[i].getvol();
		int id[4];
		id[0] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numC);
		id[1] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numD);
		id[2] = faceid(mesh_eles[i].numA, mesh_eles[i].numC, mesh_eles[i].numD);
		id[3] = faceid(mesh_eles[i].numB, mesh_eles[i].numC, mesh_eles[i].numD);
		grad[i].C = 1.0 / (2 * Vc) * (Sf[0] + Sf[1] + Sf[2] + Sf[3]);//Cϵ��,id = i

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
		rC = mesh_eles[i].getcent();//��ǰ��Ԫ����
		if (grad[i].idf1 != -1) {
			rF[0] = mesh_eles[grad[i].idf1].getcent();//��Χ��Ԫ����
			rf[0] = mesh_eles[i].getface_ABC();//����������
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
		//gradF-f1
		pgrad(grad[grad[i].idf1].C, grad[i].idf1, i, df[0], Sf[0], 0.5);
		pgrad(grad[grad[i].idf1].f1, grad[grad[i].idf1].idf1, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf1].f2, grad[grad[i].idf1].idf2, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf1].f3, grad[grad[i].idf1].idf3, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf1].f4, grad[grad[i].idf1].idf4, i, df[0], Sf[0], 0.0);
		//gradF-f2
		pgrad(grad[grad[i].idf2].C, grad[i].idf2, i, df[0], Sf[0], 0.5);
		pgrad(grad[grad[i].idf2].f1, grad[grad[i].idf2].idf1, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf2].f2, grad[grad[i].idf2].idf2, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf2].f3, grad[grad[i].idf2].idf3, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf2].f4, grad[grad[i].idf2].idf4, i, df[0], Sf[0], 0.0);
		//gradF-f3
		pgrad(grad[grad[i].idf3].C, grad[i].idf3, i, df[0], Sf[0], 0.5);
		pgrad(grad[grad[i].idf3].f1, grad[grad[i].idf3].idf1, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf3].f2, grad[grad[i].idf3].idf2, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf3].f3, grad[grad[i].idf3].idf3, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf3].f4, grad[grad[i].idf3].idf4, i, df[0], Sf[0], 0.0);
		//gradF-f4
		pgrad(grad[grad[i].idf4].C, grad[i].idf4, i, df[0], Sf[0], 0.5);
		pgrad(grad[grad[i].idf4].f1, grad[grad[i].idf4].idf1, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf4].f2, grad[grad[i].idf4].idf2, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf4].f3, grad[grad[i].idf4].idf3, i, df[0], Sf[0], 0.0);
		pgrad(grad[grad[i].idf4].f4, grad[grad[i].idf4].idf4, i, df[0], Sf[0], 0.0);
	}
}
void Fvm::findsharedface() {
	int num = mesh_nodes.size();
	for (int i = 0; i < mesh_eles.size(); i++) {
		vector<int>temp;
		vector<int>data;
		data.resize(2);
		data[1] = -1;
		temp.push_back(mesh_eles[i].numA);
		temp.push_back(mesh_eles[i].numB);
		temp.push_back(mesh_eles[i].numC);
		temp.push_back(mesh_eles[i].numD);
		sort(temp.begin(), temp.end());
		int id[4];
		id[0] = temp[0] + temp[1] * num + temp[2] * num * num;
		id[1] = temp[0] + temp[1] * num + temp[3] * num * num;
		id[2] = temp[0] + temp[2] * num + temp[3] * num * num;
		id[3] = temp[1] + temp[2] * num + temp[3] * num * num;
		data[0] = i;
		for (int j = 0; j < 4; j++) {
			auto result = sharedface.insert({ id[j],data });
			if (!result.second) {
				//�Ѿ���Ź�
				auto temp = sharedface[id[j]];
				temp[1] = data[0];
				sharedface[id[j]] = temp;
			}
		}
	}
}
inline int Fvm::faceid(int a, int b, int c) {
	vector<int>temp;
	temp.push_back(a);
	temp.push_back(b);
	temp.push_back(c);
	sort(temp.begin(), temp.end());
	int id = temp[0] + temp[1] * mesh_eles.size() + temp[2] * mesh_eles.size() * mesh_eles.size();
	return id;
}
void Fvm::cal() {
	//���㵥Ԫ�����ĸ���Ԫ���ӣ�
	//1. ������Ԫ���ϣ��
	ap.clear();
	anb.clear();
	bp.clear();
	ap.resize(mesh_eles.size());
	anb.resize(ap.size());
	bp.resize(ap.size());
	for (int i = 0; i < ap.size(); i++) {
		anb[i].resize(ap.size());
	}

	for (int i = 0; i < mesh_eles.size(); i++) {
		vector<Point>Sf = mesh_eles[i].getelementsurfacenormal();
		int id[4];
		id[0] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numC);
		id[1] = faceid(mesh_eles[i].numA, mesh_eles[i].numB, mesh_eles[i].numD);
		id[2] = faceid(mesh_eles[i].numA, mesh_eles[i].numC, mesh_eles[i].numD);
		id[3] = faceid(mesh_eles[i].numB, mesh_eles[i].numC, mesh_eles[i].numD);
		//step1: �ж����ڲ��滹���ⲿ��
		double gDiff[4] = { 0.0,0.0,0.0,0.0 };
		for (int j = 0; j < 4; j++) {
			if (sharedface[id[j]][1] == -1) {

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
				Tf = Sf[j] - Ef;

			}
		}
		ap[i] += gDiff[0] + gDiff[1] + gDiff[2] + gDiff[3];

	}
}