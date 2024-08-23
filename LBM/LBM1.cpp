#include<LBM/LBM1.h>
LBM3D::LBM3D() {
	this->LBM_mesh = new Mesh();
};
void LBM3D::build() {
	if (this->LBM_mesh->getdim1() == 0) {
		this->LBM_mesh->gettextedit()->append("Please input mesh file before cal");
		return;
	}
	NX = this->LBM_mesh->getdim1();
	NY = this->LBM_mesh->getdim2();
	NZ = this->LBM_mesh->getdim3();
	this->LBM_mesh->gettextedit()->append("Building matix...");
	rho.resize(NX + 1);
	f.resize(NX + 1);
	F.resize(NX + 1);
	u.resize(NX + 1);
	u1.resize(NX + 1);
	for (int i = 0; i < NX + 1; i++) {
		f[i].resize(NY + 1);
		F[i].resize(NY + 1);
		u[i].resize(NY + 1);
		u1[i].resize(NY + 1);
		rho[i].resize(NY + 1);
		cout << "����" << i << endl;
		for (int j = 0; j < NY + 1; j++) {
			f[i][j].resize(NZ + 1);
			F[i][j].resize(NZ + 1);
			u[i][j].resize(NZ + 1);
			u1[i][j].resize(NZ + 1);
			rho[i][j].resize(NZ + 1);
			for (int k = 0; k < NZ + 1; k++) {
				f[i][j][k].resize(19);
				F[i][j][k].resize(19);
				u[i][j][k].resize(3);
				u1[i][j][k].resize(3);
			}
		}
	}
	this->LBM_mesh->gettextedit()->append("Build completed");
}
Mesh* LBM3D::getMesh() {
	return this->LBM_mesh;
}
double LBM3D::pos(int y, int z) {
	double ux = 0, uux = 0;
	//���ۼ���ģ���м����������ٶȣ�yoz���ϵ�(y,z)��
	y = y - NX / 2; z = z - NX / 2;
	float nY = NY;
	for (int i = 0; i < 150; i++) {
		uux += pow(-1.0, i) * cosh((2.0 * i + 1.0) * pi * y / nY) * cos((2.0 * i + 1.0) * pi * z / nY) / (pow(2.0 * i + 1.0, 3.0) * cosh((2.0 * i + 1.0) * pi / 2.0));
	}
	double v = (tao_f - 0.5) / 3;
	ux = 1e-5 / v / 2.0 / 3.0 * (nY * nY / 4.0 - 1.0 * z * z - 8.0 * nY * nY / pi / pi / pi * uux);
	return ux;
}
double LBM3D::k() {
	double t = 0;
	//������֤scģ�͵���͸��
	double n[31] = { 1,1.418649,2.012564 ,2.331523 ,2.564809 ,2.534787 ,2.873609 ,
	3.340163 ,3.536763 ,3.504092 ,3.253622 ,2.689757 ,2.037769 ,1.809341 ,1.877347 ,1.534685 ,0.903471 ,0.28579 ,
	-0.55126,-1.27872 ,1.01335 ,5.492491 ,4.615388 ,-0.5736,-2.86592,-4.70922 ,-6.87008 ,0.14553 ,12.51891 ,9.742811 ,5.566269 };
	double V = R * R * R * 4 * pi / 3, Vmax = pi / 6;
	cout << "V=" << V << endl;
	for (int i = 0; i < 31; i++) {
		t += n[i] * pow(V / Vmax, i * 1.0 / 3.0);
	}
	//t�������������V�µ���͸��
	t = 1 / (6 * pi * t * R);
	return t;
}
void LBM3D::sphere(float s_R) {
	//scģ�͵ĳ�ʼ�ṹ���죻����ģ�ͱ߳�1������뾶R
	//����λ����(0.5,0.5,0.5)
	R = s_R;
	float d = 1.0 / NX;
	float x1, y1, z1;
	float D;
	for (int i = 0; i <= NX; i++)
		for (int j = 0; j <= NY; j++)
			for (int k = 0; k <= NZ; k++) {
				x1 = i * d - 0.5;
				y1 = j * d - 0.5;
				z1 = k * d - 0.5;
				D = x1 * x1 + y1 * y1 + z1 * z1;
				if (D < R * R) {
					LBM_mesh->getpoint()[i][j][k] = 1;
				}
				else
					LBM_mesh->getpoint()[i][j][k] = 0;
			}
	LL = 1;//ʵ������ģ�͵�λ����
}
//�����ʼ����Ľṹ
void LBM3D::outputfirst() {
	//�Լ��޸�λ��
	ofstream file("C:\\Users\\yunxiang.xing\\Desktop\\LBM\\MRTLBM\\MRTLBM\\��ͨ��1.dat");
	file << "VARIABLES=\"x\",\"y\",\"z\",\"u\"" << "  "
		<< "ZONE T=\"BOX\",I=" << NZ + 1 << ",J=" << NY + 1 << ",K=" << NX + 1
		<< "," << endl;
	for (int i = 4; i <= NX - 4; i++)
		for (int j = 1; j <= NY - 1; j++)
			for (int k = 1; k <= NZ - 1; k++) {
				file << i << " " << j << " " << k << " " << LBM_mesh->getpoint()[i][j][k] << endl;
			}
}
//��ʼ��
void LBM3D::init() {
	for (int i = 0; i <= NX; i++)
		for (int j = 0; j <= NY; j++)
			for (int k = 0; k <= NZ; k++) {
				//���ó�ʼѹ���ݶȵı仯���Ը����ȶ�
				float dp = 1e-5;
				rho[i][j][k] = 1.0 + dp * (NX - i);
				//p[i][j][k] = rho[i][j][k] / 3.0;
				u[i][j][k][0] = 0.0;
				u[i][j][k][1] = 0.0;
				u[i][j][k][2] = 0.0;
				for (int l = 0; l < Q; l++) {
					f[i][j][k][l] = feq(l, rho[i][j][k], u[i][j][k]);
				}
			}
}
//���ɳڼ���
float inline LBM3D::feq(int l, float rho, vector<float> u) {
	float eu, uv, fe;
	eu = (e[l][0] * u[0] + e[l][1] * u[1] + e[l][2] * u[2]);
	uv = (u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
	fe = w[l] * rho * (1.0 + 3.0 * eu + 4.5 * eu * eu - 1.5 * uv);
	return fe;
}
//���ɳڼ���
void LBM3D::mfeq(int i, int j, int k) {
	float omiga[19], f0[19];
	for (int l = 0; l < 19; l++)
		f0[l] = feq(l, rho[i][j][k], u[i][j][k]);
	for (int l = 0; l < 19; l++) {
		omiga[l] = 0;
		for (int ll = 0; ll < 19; ll++) {
			omiga[l] += invM_S_M[l][ll] * (f0[ll] - f[i][j][k][ll]);
		}
	}
	for (int l = 0; l < 19; l++) {
		F[i][j][k][l] = f[i][j][k][l] + omiga[l];
	}
}
//�ݻ�����
void LBM3D::evolution_coll(col co) {
	//��ײ����
	int i, j, k, l;
	int ip, jp, kp;
	for (i = 0; i <= NX; i++)
		for (j = 0; j <= NY; j++)
			for (k = 0; k <= NZ; k++)
				if (LBM_mesh->getpoint()[i][j][k] == 0) {
					//���ɳ�
					if (co == col::SRT)
						for (l = 0; l < Q; l++) {
							F[i][j][k][l] = f[i][j][k][l] - 1.0 / tao_f * (f[i][j][k][l] - feq(l, rho[i][j][k], u[i][j][k]));
						}
					else if(co == col::MRT)
						mfeq(i, j, k);
				}
	int r[19] = { 0,2,1,4,3,6,5,8,7,10,9,12,11,14,13,16,15,18,17 };
	for (i = 0; i <= NX; i++)
		for (j = 0; j <= NY; j++)
			for (k = 0; k <= NZ; k++)
				if (LBM_mesh->getpoint()[i][j][k] == 0)
					for (l = 0; l < Q; l++) {
						//���ַ�ʽ���������򵥣�������ٶȷ������μ���
						//λ��ip��jp��kp��L�����������������i��j��k
						ip = i - e[l][0];
						jp = j - e[l][1];
						kp = k - e[l][2];
						//���ڱ߽�����
						if (ip > NX)
							continue;
						if (ip < 0)
							continue;
						if (jp > NY)
							jp = 0;
						if (jp < 0)
							jp = NY;
						if (kp > NZ)
							kp = 0;
						if (kp < 0)
							kp = NZ;
						//(ip,jp,kp)�ǹ�����ʱ��������������i,j,kλ�õ�L������������ӷ���
						if (LBM_mesh->getpoint()[ip][jp][kp] == 1) {
							f[i][j][k][l] = F[i][j][k][r[l]];
						}
						//(ip,jp,kp)��������ʱ��������������
						if (LBM_mesh->getpoint()[ip][jp][kp] == 0) {
							f[i][j][k][l] = F[ip][jp][kp][l];
						}
					}
}
void LBM3D::evolution_bir(bj s) {
	int i, j, k;
	for (i = 0; i <= NY; i++)
		for (j = 0; j <= NZ; j++) {
			//����ѹ���߽�
			//ѹ���ݶȣ����Сһ�㣬С�ڵ���1e-5
			float dp = 1e-5;
			rho[0][i][j] = 1 + dp * NX;
			u[0][i][j][0] = 1.0 - (f[0][i][j][3] + f[0][i][j][4] + f[0][i][j][5] + f[0][i][j][6]
				+ f[0][i][j][11] + f[0][i][j][12] + f[0][i][j][17] + f[0][i][j][18] + f[0][i][j][0]
				+ 2 * (f[0][i][j][2] + f[0][i][j][8] + f[0][i][j][14] + f[0][i][j][10] + f[0][i][j][16])) / rho[0][i][j];
			f[0][i][j][1] = f[0][i][j][2] + 1.0 / 3.0 * rho[0][i][j] * u[0][i][j][0];
			f[0][i][j][7] = f[0][i][j][8] + rho[0][i][j] * u[0][i][j][0] / 6.0 - 0.5 *
				(f[0][i][j][3] + f[0][i][j][11] + f[0][i][j][17] - f[0][i][j][4] - f[0][i][j][12] - f[0][i][j][18]);
			f[0][i][j][13] = f[0][i][j][14] + rho[0][i][j] * u[0][i][j][0] / 6.0 + 0.5 *
				(f[0][i][j][3] + f[0][i][j][11] + f[0][i][j][17] - f[0][i][j][4] - f[0][i][j][12] - f[0][i][j][18]);
			f[0][i][j][9] = f[0][i][j][10] + rho[0][i][j] * u[0][i][j][0] / 6.0 - 0.5 *
				(f[0][i][j][5] + f[0][i][j][11] + f[0][i][j][18] - f[0][i][j][6] - f[0][i][j][17] - f[0][i][j][12]);
			f[0][i][j][15] = f[0][i][j][16] + rho[0][i][j] * u[0][i][j][0] / 6.0 + 0.5 *
				(f[0][i][j][5] + f[0][i][j][11] + f[0][i][j][18] - f[0][i][j][6] - f[0][i][j][17] - f[0][i][j][12]);
			//����ѹ���߽�
			rho[NX][i][j] = 1.0;
			u[NX][i][j][0] = -1.0 + (f[NX][i][j][3] + f[NX][i][j][4] + f[NX][i][j][5] + f[NX][i][j][6]
				+ f[NX][i][j][11] + f[NX][i][j][12] + f[NX][i][j][17] + f[NX][i][j][18] + f[NX][i][j][0] +
				2 * (f[NX][i][j][1] + f[NX][i][j][7] + f[NX][i][j][13] + f[NX][i][j][9] + f[NX][i][j][15])) / rho[NX][i][j];
			f[NX][i][j][2] = f[NX][i][j][1] - 1.0 / 3.0 * rho[NX][i][j] * u[NX][i][j][0];
			f[NX][i][j][8] = f[NX][i][j][7] - 1.0 / 6.0 * rho[NX][i][j] * u[NX][i][j][0] + 0.5 *
				(f[NX][i][j][3] + f[NX][i][j][11] + f[NX][i][j][17] - f[NX][i][j][4] - f[NX][i][j][12] - f[NX][i][j][18]);
			f[NX][i][j][14] = f[NX][i][j][13] - 1.0 / 6.0 * rho[NX][i][j] * u[NX][i][j][0] - 0.5 *
				(f[NX][i][j][3] + f[NX][i][j][11] + f[NX][i][j][17] - f[NX][i][j][4] - f[NX][i][j][12] - f[NX][i][j][18]);
			f[NX][i][j][10] = f[NX][i][j][9] - 1.0 / 6.0 * rho[NX][i][j] * u[NX][i][j][0] + 0.5 *
				(f[NX][i][j][5] + f[NX][i][j][11] + f[NX][i][j][18] - f[NX][i][j][6] - f[NX][i][j][17] - f[NX][i][j][12]);
			f[NX][i][j][16] = f[NX][i][j][15] - 1.0 / 6.0 * rho[NX][i][j] * u[NX][i][j][0] - 0.5 *
				(f[NX][i][j][5] + f[NX][i][j][11] + f[NX][i][j][18] - f[NX][i][j][6] - f[NX][i][j][17] - f[NX][i][j][12]);
		}
}
void LBM3D::evolution(col coll,bj bir) {
	evolution_coll(coll);
	evolution_bir(bir);
	//��������
	int i, j, k, l;
	for (i = 1; i < NX; i++)
		for (j = 0; j <= NY; j++)
			for (k = 0; k <= NZ; k++) {
				//ֻ��Ҫ����������
				if (LBM_mesh->getpoint()[i][j][k] == 0) {
					u1[i][j][k][0] = u[i][j][k][0];
					u1[i][j][k][1] = u[i][j][k][1];
					u1[i][j][k][2] = u[i][j][k][2];
					rho[i][j][k] = 0.0;
					u[i][j][k][0] = 0.0;
					u[i][j][k][1] = 0.0;
					u[i][j][k][2] = 0.0;
					for (l = 0; l < Q; l++) {
						rho[i][j][k] += f[i][j][k][l];
						u[i][j][k][0] += e[l][0] * f[i][j][k][l];
						u[i][j][k][1] += e[l][1] * f[i][j][k][l];
						u[i][j][k][2] += e[l][2] * f[i][j][k][l];
					}
					//�ܶ�С��0ʱ����
					if (abs(rho[i][j][k]) < 1e-3)
						cout << "rho(i,j,k)=" << rho[i][j][k] << endl;
					u[i][j][k][0] /= rho[i][j][k];
					u[i][j][k][1] /= rho[i][j][k];
					u[i][j][k][2] /= rho[i][j][k];
				}
			}
}
//��������׽���������С��1e-5��ֻ��Ҫ�ȴ���ṹ����͸�ʼ����ȶ�����
void LBM3D::errorr() {
	double temp1, temp2;
	temp1 = 0;
	temp2 = 0;
	for (int i = 1; i < NX; i++)
		for (int j = 1; j < NY; j++)
			for (int k = 1; k < NZ; k++) {
				temp1 += (u[i][j][k][0] - u1[i][j][k][0]) * (u[i][j][k][0] - u1[i][j][k][0]);
				temp2 += u1[i][j][k][0] * u1[i][j][k][0];
			}
	temp1 = sqrt(temp1);
	temp2 = sqrt(temp2);
	Er = temp1 / (temp2 + 1e-30);
	LBM_mesh->gettextedit()->append("error = " + QString::number(Er));
	cout << Er << endl;/**/
	int i, j, k, num = 0;
	//���ģ�����ĵ���ٶ�
	do {
		num++;
		i = NX / 2;
		j = NY / 2;
		k = NZ / 2;
		if (LBM_mesh->getpoint()[i][j][k] == 0) {
			cout << "����Ϊ��x=" << i << " y=" << j << " z=" << k << " ";
			std::cout << "�м����ٶȣ�" << u[i][j][k][0] << endl;
		}
		else
			i++;
	} while (LBM_mesh->getpoint()[i][j][k] == 1 && num < 10);
	/*for (int i = 1; i < NX; i++)
		for (int j = 1; j < NY; j++)
			for (int k = 1; k < NZ; k++) {
				temp1 += u[i][j][k][0];
			}
	temp2 = (NX - 1) * (NY - 1) * (NZ - 1) + 1e-30;
	cout << "�����ƽ���ٶ�Ϊ��";
	cout << temp1 / temp2 << endl;*/
}
double LBM3D::getEr() {
	return Er;
}
//���������
void LBM3D::outputlast(string file) {
	//�����޸�
	ofstream out(file);
	out << "Title=\"LBM Lid Driven Flow\"" << endl
		<< "VARIABLES=\"X\",\"Y\",\"Z\",\"U\",\"V\",\"W\",\"P\"" << endl
		<< "ZONE T= \"BOX\", I= " << NZ - 2 + 1 << ", J=" << NY - 2 + 1 << ", K=" << NX - 8 + 1
		<< "," << endl;
	for (int i = 4; i <= NX - 4; i++)
		for (int j = 1; j <= NY - 1; j++)
			for (int k = 1; k <= NZ - 1; k++) {
				if (LBM_mesh->getpoint()[i][j][k] == 0)
					out << i << " " << j << " " << k << " " << u[i][j][k][0] << " " << u[i][j][k][1]
					<< " " << u[i][j][k][2] << " " << sqrt(u[i][j][k][0] * u[i][j][k][0] +
						u[i][j][k][1] * u[i][j][k][1] + u[i][j][k][2] * u[i][j][k][2]) << endl;
				else {
					out << i << " " << j << " " << k << " " << 0 << " " << 0
						<< " " << 0 << " " << 0 << endl;
				}
			}
}
//��͸�ʼ���
double LBM3D::kLB() {
	double uz = 0;
	//��׽��ʼ���ʱi=5��i<=NX-5�����˻���㣬��ʱNX��NY��NZ��8��
	//�����scģ�ͻ���Ҷ��������������i=0��i<NX�����ӻ���㣬ͬʱҲӦ�õ���NX�Ĵ�С��NY��NZһ�£�
	for (int i = 0; i <= NX; i++)
		for (int j = 1; j < NY; j++)
			for (int k = 1; k < NZ; k++) {
				uz += u[i][j][k][0];
			}
	double v = (tao_f - 0.5) / 3;
	//��͸�ʼ��㣺������˵���˴���NX-9������Ӧ����һ�£���NX=NY=NZ��Ӧ��ΪNX-1��
	uz = uz * v * 3 / 1e-5 / ((NX - 1) * (NY - 1) * (NZ - 1));
	return uz;
}
void LBM3D::setinlet(inlet dim) {
	if (dim == LBM_inlet) {
		return;
	}
	if (dim == inlet::x) {
		this->LBM_mesh->setdim123(NY, NX, NZ);
		this->build();
	}
}
int LBM3D::getnum() {
	return num;
}
void LBM3D::cal(col coll,bj bir) {
	//����ģ�ʹ�С
	LL = NX;
	LBM_mesh->gettextedit()->append("calcalute");
	init();
	double k1, k2 = 100;
	for (num = 1; num <= 1000; num++) {
		evolution(coll,bir);
		if (num % 100 == 0) {
			LBM_mesh->gettextedit()->append("number of iterations = " + QString::number(num));
			errorr();
			k1 = (LL / NX) * (LL / NX) * kLB();
			if (abs(k1 - k2) < 1e-7)
				break;
			else {
				k2 = k1;
			}
			cout << "��͸�� kLB= " << k1 << endl;
			if (Er < 1e-5)
				break;
		}
	}
	//����Ҷ����֤���м������ٶ�
	/*cout << "�����ٶ�Ϊ: " << endl;
	cout << "p=[" << endl;
	for (int y = 0; y <= NY; y++) {
		cout << pos(y, NX/2) << endl;
	}
	cout << "]" << endl;
	cout << "LBM�����ٶ�Ϊ: " << endl;
	cout << "p1=[" << endl;
	for (int y = 0; y <= NY; y++) {

		cout << u[NX/2][y][NZ/2][0] << endl;
	}
	cout << "]";*/
}