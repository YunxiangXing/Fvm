#include"mesh.h"
bool Mesh::reading(string temp_dir) {
	//导入多孔介质结构--计算其渗透率
	dir = temp_dir;
	ifstream file1;
	string ct;
	int at;
	//多孔介质结构的三维数组大小
	int i = 1, j = 1, k = 1, solid_num = 0, fluid_num = 0;
	//多孔介质结构数组文件位置：括号内位置应自行修改
	int temp_dim1 = dim1 - 7;
	int temp_dim2 = dim2 - 1;
	int temp_dim3 = dim3 - 1;
	file1.open(dir);
	if (!file1.is_open()) {
		textedit->append("Failed to open file");
		return 0;
	}
	while (!file1.eof()) {
		getline(file1, ct, '\n');
		istringstream iss(ct);
		while (iss >> at) {
			//之所以此处是i+4，是为了在压力梯度方向施加一个4个过渡面，有助于流体稳定，文献中有这么设置。
			if (i > temp_dim1 || j > temp_dim2 || k > temp_dim3) {
				textedit->append("Lattice size error!\n");
				return 0;
			}
			else if (i < 0 || j < 0 || k < 0) {
				textedit->append("Lattice size error!\n");
				return 0;
			}
			point[i + 3][j][k] = 1 - at;
			if (point[i + 3][j][k] == 1)
				solid_num++;
			else if (point[i + 3][j][k] == 0)
				fluid_num++;
			if (k < temp_dim3) {
				k++;
			}
			else {
				if (j < temp_dim2) {
					j++;
					k = 1;
				}
				else {
					if (i < temp_dim1) {
						i++;
						j = 1;
						k = 1;
					}
				}
			}
		}
	}
	//固体相点数量和固相率
	if (fluid_num + solid_num != temp_dim1 * temp_dim2 * temp_dim3) {
		textedit->append("Reading Mesh Error,please check your inputfile!\n");
		return 0;
	}
	textedit->append("Reading mesh complete");
	textedit->append("porosity is: " + QString::number(fluid_num * 1.0 / (1.0 * temp_dim1 * temp_dim2 * temp_dim3)) + "%");
	return 1;
}
int Mesh::getdim1() {
	return dim1;
}
int Mesh::getdim2() {
	return dim2;
}
int Mesh::getdim3() {
	return dim3;
}
vector<vector<vector<int>>>& Mesh::getpoint() {
	return point;
}
void Mesh::setdim123(int dimX, int dimY, int dimZ) {
	if (dimX == 0 || dimY == 0 || dimZ == 0) {	
		textedit->append("lattice error\n"); 
		Error_LBM();
	}
	dim1 = dimX;
	dim2 = dimY;
	dim3 = dimZ;
	buildpoint();
	textedit->append("set X Y Z Lattice = " + QString::number(dim1) + " " + QString::number(dim2) + " " + QString::number(dim3));
}
void Mesh::setQTextedit(QTextEdit* Text) {
	textedit = Text;
}
void Mesh::buildpoint() {
	point.clear();
	point.resize(dim1 + 1);
	for (int i = 0; i < dim1 + 1; i++) {
		point[i].resize(dim2 + 1);
		for (int j = 0; j < dim2 + 1; j++) {
			point[i][j].resize(dim3 + 1);
		}
	}
}
void Mesh::sphere(double cirx,double ciry,double cirz,double R) {
	int cir_x = floor(cirx);
	int cir_y = floor(ciry);
	int cir_z = floor(cirz);
	int r = floor(R) + 1;
	if (cir_x - R <= 0 || cir_x + R >= dim1 - 8) {
		textedit->append("circle X coor/R error!");
		return;
	}
	if (cir_y - R <= 0 || cir_y + R >= dim2 - 2) {
		textedit->append("circle Y coor/R error!");
		return;
	}
	if (cir_z - R <= 0 || cir_z + R >= dim3 - 2) {
		textedit->append("circle Z coor/R error!");
		return;
	}
	for (int i = cir_x - R; i <= cir_x + R; i++)
		for (int j = cir_y - R; j <= cir_y + R; j++)
			for (int k = cir_z - R; k <= cir_z + R; k++) {
				double ditance = sqrt(pow(i - cirx, 2) + pow(j - ciry, 2) + pow(k - cirz, 2));
				if (ditance <= R) point[i + 4][j + 1][k + 1] = 1;
			}
	textedit->append("sphere completely!");
}
bool Mesh::output(string temp_dir) {
	textedit->append("ouput dir: " + QString::fromStdString(temp_dir));
	ofstream file(temp_dir);
	if (!file.is_open()) {
		textedit->append("not found file");
		return 0;
	}
	file << "VARIABLES=\"x\",\"y\",\"z\",\"u\"" << "  "
		<< "ZONE T=\"BOX\",I=" << dim3 - 1 << ",J=" << dim2 - 1 << ",K=" << dim1 - 7
		<< ",F=POINT" << endl;
	for (int i = 4; i <= dim1 - 4; i++)
		for (int j = 1; j <= dim2 - 1; j++)
			for (int k = 1; k <= dim3 - 1; k++) {
				file << i << " " << j << " " << k << " " << point[i][j][k] << endl;
			}
	return 1;
}
QTextEdit* Mesh::gettextedit() {
	return this->textedit;
}
double Mesh::getpolor() {
	return polor();
}
double Mesh::polor() {
	int pol = 0;
	for (int i = 4; i <= dim1 - 4; i++)
		for (int j = 1; j <= dim2 - 1; j++)
			for (int k = 1; k <= dim3 - 1; k++) {
				if (point[i][j][k] == 0) pol++;
			}
	return pol * 1.0 / (dim1 - 7) / (dim2 - 1) / (dim3 - 1);
}