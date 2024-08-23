#ifndef LBM1_H
#define LBM1_H
#include"base\base.h"
#include"base\mesh.h"
#include<QtWidgets>
using namespace std;
//�������ͣ�����
struct coor {
	int x = 0, y = 0, z = 0;
	void A(int a, int b, int c) {
		x = a;
		y = b;
		z = c;
	};
};
//LBM��ά�����ɳڼ�����
class LBM3D{
public:
	LBM3D();
	Mesh* getMesh();
	void sphere(float s_R);//scģ�ͣ�����������һ�������
	void init();//��ʼ��
	float inline feq(int, float, vector<float>);//���ɳ���ײ
	void mfeq(int, int, int);//���ɳ���ײ
	void evolution(col, bj);//�ݻ�����
	void errorr();//��׽���������
	double getEr();
	int getnum();
	void outputfirst();//�������Ľṹ��������֤�����Ƿ���ȷ
	void outputlast(string);//���������ɺ�ṹ������tecplot���ӻ�
	double pos(int, int);//��ά�����岴��Ҷ�������ٶȼ���
	void cal(col, bj);//����LBM���̵ļ��㷽��
	double kLB();//��͸��
	void evolution_coll(col);
	void evolution_bir(bj);
	void build();
	void setinlet(inlet);
	double k();//scģ��������͸��
	//~LBM3D() {
	//	cout << "��������" << endl;
	//	for (int i = 0; i < NX + 1; i++) {
	//		for (int j = 0; j < NY + 1; j++) {
	//			for (int k = 0; k < NZ + 1; k++) {
	//				delete[]f[i][j][k];
	//				delete[]F[i][j][k];
	//				delete[]u[i][j][k];
	//				delete[]u1[i][j][k];
	//				//cout << "�ͷ�" << "f1[" << i << "][" << j << "][" << k << "]" << endl;
	//			}
	//			delete[]f[i][j];
	//			delete[]F[i][j];
	//			delete[]u[i][j];
	//			delete[]u1[i][j];
	//			//cout << "�ͷ�" << "f1[" << i << "][" << j <<"]" << endl;
	//		}
	//		delete[]f[i];
	//		delete[]F[i];
	//		delete[]u[i];
	//		delete[]u1[i];
	//		cout << "�ͷ�" << "f/F/u/u1[" << i << "]" << endl;
	//	}
	//	delete[]u;
	//	delete[]u1;
	//	delete[]f;
	//	delete[]F;
	//};
	~LBM3D() {
		delete LBM_mesh;
	}
private:
	//D3Q19���ٶȷ�����Ȩ��
	QTimer* LBM_timer = NULL;
	inlet LBM_inlet;
	const int Q = 19;
	int e[19][3] = { {0,0,0},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1},{1,1,0},
		{-1,-1,0},{1,0,1},{-1,0,-1},{0,1,1},{0,-1,-1},{1,-1,0},{-1,1,0},{1,0,-1},{-1,0,1},
		{0,1,-1},{0,-1,1}
	};
	float w[19] = { 1.0 / 3,1.0 / 18,1.0 / 18,1.0 / 18,1.0 / 18,1.0 / 18,1.0 / 18,1.0 / 36,
		1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,1.0 / 36,
		1.0 / 36,1.0 / 36 };
	Mesh* LBM_mesh = NULL;
	//��������
	int num = 0;
	//���
	int NX = 0, NY = 0, NZ = 0;
	double Er;
	double Er1;
	//scģ������뾶��С
	float R;
	//����ģ�͵ĳߴ�
	double LL;
	vector<vector<vector<vector<float>>>>f, F, u, u1;
	vector<vector<vector<float>>>rho;
	const double pi = 3.1415926535;
	//��ά��������ӵ�����
	//���㲴��Ҷ��ʱ��ѡȡNX=NY=NZ=50
	//����scģ�ͻ�BCC��FCCʱ�̿�ѡȡNX=NY=NZ=50
	//�����׽���ʱһ��ѡȡNX=209��NY=NZ=201
	//�ֲ�����������ٶȺͺ���ܶ�
	//�����׽��ʵ�����
	//���ɳڵ�ת������
	double M[19][19] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{-30,-11,-11,-11,-11,-11,-11,8,8,8,8,8,8,8,8,8,8,8,8},
		{12,-4,-4,-4,-4,-4,-4,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,-1,0,0,0,0,1,-1,1,-1,0,0,1,-1,1,-1,0,0},
		{0,-4,4,0,0,0,0,1,-1,1,-1,0,0,1,-1,1,-1,0,0},
		{0,0,0,1,-1,0,0,1,-1,0,0,1,-1,-1,1,0,0,1,-1},
		{0,0,0,-4,4,0,0,1,-1,0,0,1,-1,-1,1,0,0,1,-1},
		{0,0,0,0,0,1,-1,0,0,1,-1,1,-1,0,0,-1,1,-1,1},
		{0,0,0,0,0,-4,4,0,0,1,-1,1,-1,0,0,-1,1,-1,1},
		{0,2,2,-1,-1,-1,-1,1,1,1,1,-2,-2,1,1,1,1,-2,-2},
		{0,-4,-4,2,2,2,2,1,1,1,1,-2,-2,1,1,1,1,-2,-2},
		{0,0,0,1,1,-1,-1,1,1,-1,-1,0,0,1,1,-1,-1,0,0},
		{0,0,0,-2,-2,2,2,1,1,-1,-1,0,0,1,1,-1,-1,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,-1,-1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,-1,-1},
		{0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,-1,-1,0,0},
		{0,0,0,0,0,0,0,1,-1,-1,1,0,0,1,-1,-1,1,0,0},
		{0,0,0,0,0,0,0,-1,1,0,0,1,-1,1,-1,0,0,1,-1},
		{0,0,0,0,0,0,0,0,0,1,-1,-1,1,0,0,-1,1,1,-1} };
	//ת������������
	double InvM[19][19] = {
		{1.0 / 19.0,-5.0 / 399.0,1.0 / 21.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,0.1,-0.1,0,0,0,0,1.0 / 18.0,-1.0 / 18.0,0,0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,-0.1,0.1,0,0,0,0,1.0 / 18.0,-1.0 / 18.0,0,0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,0,0,0.1,-0.1,0,0,-1.0 / 36.0,1.0 / 36.0,1.0 / 12.0,-1.0 / 12.0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,0,0,-0.1,0.1,0,0,-1.0 / 36.0,1.0 / 36.0,1.0 / 12.0,-1.0 / 12.0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,0,0,0,0,0.1,-0.1,-1.0 / 36.0,1.0 / 36.0,-1.0 / 12.0,1.0 / 12.0,0,0,0,0,0,0},
		{1.0 / 19.0,-11.0 / 2394,-1.0 / 63.0,0,0,0,0,-0.1,0.1,-1.0 / 36.0,1.0 / 36.0,-1.0 / 12.0,1.0 / 12.0,0,0,0,0,0,0},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252.0,0.1,0.025,0.1,0.025,0,0,1.0 / 36.0,1.0 / 72.0,1.0 / 12.0,1.0 / 24.0,0.25,0,0,0.125,-0.125,0},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,-0.1,-0.025,-0.1,-0.025,0,0,1.0 / 36.0,1.0 / 72.0,1.0 / 12.0,1.0 / 24.0,0.25,0,0,-0.125,0.125,0},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0.1,0.025,0,0,0.1,0.025,1.0 / 36.0,1.0 / 72.0,-1.0 / 12.0,-1.0 / 24.0,0,0,0.25,-0.125,0,0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,-0.1,-0.025,0,0,-0.1,-0.025,1.0 / 36.0,1.0 / 72.0,-1.0 / 12.0,-1.0 / 24.0,0,0,0.25,0.125,0,-0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0,0,0.1,0.025,0.1,0.025,-1.0 / 18.0,-1.0 / 36.0,0,0,0,0.25,0,0,0.125,-0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0,0,-0.1,-0.025,-0.1,-0.025,-1.0 / 18.0,-1.0 / 36.0,0,0,0,0.25,0,0,-0.125,0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0.1,0.025,-0.1,-0.025,0,0,1.0 / 36.0,1.0 / 72.0,1.0 / 12.0,1.0 / 24.0,-0.25,0,0,0.125,0.125,0},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,-0.1,-0.025,0.1,0.025,0,0,1.0 / 36.0,1.0 / 72.0,1.0 / 12.0,1.0 / 24.0,-0.25,0,0,-0.125,-0.125,0},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0.1,0.025,0,0,-0.1,-0.025,1.0 / 36.0,1.0 / 72.0,-1.0 / 12.0,-1.0 / 24.0,0,0,-0.25,-0.125,0,-0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,-0.1,-0.025,0,0,0.1,0.025,1.0 / 36.0,1.0 / 72.0,-1.0 / 12.0,-1.0 / 24.0,0,0,-0.25,0.125,0,0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0,0,0.1,0.025,-0.1,-0.025,-1.0 / 18.0,-1.0 / 36.0,0,0,0,-0.25,0,0,0.125,0.125},
		{1.0 / 19.0,4.0 / 1197.0,1.0 / 252,0,0,-0.1,-0.025,0.1,0.025,-1.0 / 18.0,-1.0 / 36.0,0,0,0,-0.25,0,0,-0.125,-0.125}
	};
	//Ϊ���㷽�㣬���Ч�ʣ����ɳ�ʹ�ô˾���M-1*S*M
	float invM_S_M[19][19] = {
	{0.947368421052632,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684},
	{-0.0526315789473684,0.904511278195489,-0.0097744360902256,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684},
	{-0.0526315789473684,-0.0097744360902256,0.904511278195489,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.904511278195489,-0.00977443609022557,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.00977443609022557,0.904511278195489,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.904511278195489,-0.00977443609022557,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.00977443609022557,0.904511278195489,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.790225563909774,0.104511278195489,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.104511278195489,0.790225563909774,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,0.790225563909774,0.104511278195489,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.104511278195489,0.790225563909774,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,0.790225563909774,0.104511278195489,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.104511278195489,0.790225563909774,-0.166917293233083,0.0616541353383459,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684},
	{-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,0.790225563909774,0.104511278195489,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,0.104511278195489,0.790225563909774,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,0.790225563909774,0.104511278195489,-0.166917293233083,0.0616541353383459},
	{-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,0.104511278195489,0.790225563909774,0.0616541353383459,-0.166917293233083},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,0.790225563909774,0.104511278195489},
	{-0.0526315789473684,-0.0526315789473684,-0.0526315789473684,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,-0.166917293233083,0.0616541353383459,-0.0526315789473684,-0.0526315789473684,-0.166917293233083,0.0616541353383459,0.0616541353383459,-0.166917293233083,0.104511278195489,0.790225563909774}
	};
	float tao_f = 1.0;//�ɳ�ϵ��
	double Sv = 1.0 / tao_f;//���ɳ��ɳ�ϵ��
	//���ɳڶԽ��ɳھ���
	double S[19] = { 0,Sv,Sv,0,8.0 * (2.0 - Sv) / (8.0 - Sv),0,8.0 * (2.0 - Sv) / (8.0 - Sv),
	0,8.0 * (2.0 - Sv) / (8.0 - Sv) ,Sv,Sv,Sv,Sv,Sv,Sv,Sv,8.0 * (2.0 - Sv) / (8.0 - Sv),
	8.0 * (2.0 - Sv) / (8.0 - Sv) ,8.0 * (2.0 - Sv) / (8.0 - Sv) };
};
#endif // LBM1