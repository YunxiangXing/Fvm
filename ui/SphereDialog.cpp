#include"SphereDialog.h"
SphereDialog::SphereDialog(QWidget* parent)
	:QDialog(parent) {
	setWindowTitle("Sphere Mesh");
	QHBoxLayout* layout1 = new QHBoxLayout();
	QHBoxLayout* layout2 = new QHBoxLayout();
	QHBoxLayout* layout3 = new QHBoxLayout();
	QHBoxLayout* layout4 = new QHBoxLayout();
	QHBoxLayout* layout5 = new QHBoxLayout();
	QHBoxLayout* layout6 = new QHBoxLayout();
	QHBoxLayout* layout7 = new QHBoxLayout();
	param1Edit = new QLineEdit(this);
	param2Edit = new QLineEdit(this);
	param3Edit = new QLineEdit(this);
	param4Edit = new QLineEdit(this);
	param5Edit = new QLineEdit(this);
	param6Edit = new QLineEdit(this);
	param7Edit = new QLineEdit(this);
	QDoubleValidator* validator1 = new QDoubleValidator(0.0, 999.0, 2, param1Edit);
	QDoubleValidator* validator2 = new QDoubleValidator(0.0, 999.0, 2, param2Edit);
	QDoubleValidator* validator3 = new QDoubleValidator(0.0, 999.0, 2, param3Edit);
	QDoubleValidator* validator4 = new QDoubleValidator(0.0, 999.0, 2, param4Edit);
	QIntValidator* validator5 = new QIntValidator(0, 1000, param5Edit);
	QIntValidator* validator6 = new QIntValidator(0, 1000, param6Edit);
	QIntValidator* validator7 = new QIntValidator(0, 1000, param7Edit);
	param1Edit->setValidator(validator1);
	param2Edit->setValidator(validator2);
	param3Edit->setValidator(validator3);
	param4Edit->setValidator(validator4);
	param5Edit->setValidator(validator5);
	param6Edit->setValidator(validator6);
	param7Edit->setValidator(validator7);
	QLabel* label1 = new QLabel("Sphere coor X:");
	QLabel* label2 = new QLabel("Sphere coor Y:");
	QLabel* label3 = new QLabel("Sphere coor Z:");
	QLabel* label4 = new QLabel("Sphere R:");
	QLabel* label5 = new QLabel("Lattice X:");
	QLabel* label6 = new QLabel("Lattice Y:");
	QLabel* label7 = new QLabel("Lattice Z:");
	auto* layout = new QVBoxLayout;
	layout1->addWidget(label1);
	layout2->addWidget(label2);
	layout3->addWidget(label3);
	layout4->addWidget(label4);
	layout5->addWidget(label5);
	layout6->addWidget(label6);
	layout7->addWidget(label7);
	layout1->addWidget(param1Edit);
	layout2->addWidget(param2Edit);
	layout3->addWidget(param3Edit);
	layout4->addWidget(param4Edit);
	layout5->addWidget(param5Edit);
	layout6->addWidget(param6Edit);
	layout7->addWidget(param7Edit);
	layout->addLayout(layout5);
	layout->addLayout(layout6);
	layout->addLayout(layout7);
	layout->addLayout(layout1);
	layout->addLayout(layout2);
	layout->addLayout(layout3);
	layout->addLayout(layout4);
	QPushButton* okButton = new QPushButton("Ok", this);
	QPushButton* cancelButton = new QPushButton("Cancel", this);
	connect(okButton, &QPushButton::clicked, this, &SphereDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &SphereDialog::reject);
	layout->addWidget(okButton);
	layout->addWidget(cancelButton);
	setLayout(layout);
}
double SphereDialog::param1() {
	return param1Edit->text().toDouble();
}
double SphereDialog::param2() {
	return param2Edit->text().toDouble();
}
double SphereDialog::param3() {
	return param3Edit->text().toDouble();
}
double SphereDialog::param4() {
	return param4Edit->text().toDouble();
}
int SphereDialog::param5() {
	return param5Edit->text().toInt();
}
int SphereDialog::param6() {
	return param6Edit->text().toInt();
}
int SphereDialog::param7() {
	return param7Edit->text().toInt();
}
QString SphereDialog::param8() {
	return param8Edit->text();
}
void SphereDialog::setmesh_file() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::homePath(), tr("All Files (*.*)"));

	// 如果用户选择了文件
	if (!fileName.isEmpty()) {
		// 显示选择的文件路径
		param8Edit->setText(fileName);
	}
	else {
		QMessageBox::warning(this, "Error", "Failed to open file.");
	}
}