#include"meshdim.h"
meshdimDialog::meshdimDialog(QWidget* parent)
	:QDialog(parent) {
	setWindowTitle("input mesh dimXYZ");
	QPushButton* mesh_file = new QPushButton("open mesh file", this);
	QHBoxLayout* layout1 = new QHBoxLayout();
	QHBoxLayout* layout2 = new QHBoxLayout();
	QHBoxLayout* layout3 = new QHBoxLayout();
	QHBoxLayout* layout4 = new QHBoxLayout();
	param1Edit = new QLineEdit(this);
	param2Edit = new QLineEdit(this);
	param3Edit = new QLineEdit(this);
	param4Edit = new QLineEdit(this);
	QIntValidator* validator1 = new QIntValidator(0, 999, this);
	QIntValidator* validator2 = new QIntValidator(0, 999, this);
	QIntValidator* validator3 = new QIntValidator(0, 999, this);
	param1Edit->setValidator(validator1);
	param2Edit->setValidator(validator2);
	param3Edit->setValidator(validator3);
	QLabel* label1 = new QLabel("Lattice X:");
	QLabel* label2 = new QLabel("Lattice Y:");
	QLabel* label3 = new QLabel("Lattice Z:");
	QLabel* label4 = new QLabel("Input Mesh file:");
	auto* layout = new QVBoxLayout;
	layout1->addWidget(label1);
	layout2->addWidget(label2);
	layout3->addWidget(label3);
	layout4->addWidget(label4);
	layout1->addWidget(param1Edit);
	layout2->addWidget(param2Edit);
	layout3->addWidget(param3Edit);
	layout4->addWidget(param4Edit);
	layout4->addWidget(mesh_file);
	layout->addLayout(layout1);
	layout->addLayout(layout2);
	layout->addLayout(layout3);
	layout->addLayout(layout4);
	QPushButton* okButton = new QPushButton("Ok", this);
	QPushButton* cancelButton = new QPushButton("Cancel", this);
	connect(okButton, &QPushButton::clicked, this, &meshdimDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &meshdimDialog::reject);
	layout->addWidget(okButton);
	layout->addWidget(cancelButton);
	setLayout(layout);
	connect(mesh_file, &QPushButton::clicked, this, &meshdimDialog::setmesh_file);
}
void meshdimDialog::setmesh_file() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::homePath(), tr("All Files (*.*)"));

	// 如果用户选择了文件
	if (!fileName.isEmpty()) {
		// 显示选择的文件路径
		param4Edit->setText(fileName);
	}
	else {
		QMessageBox::warning(this, "Error", "Failed to open file.");
	}
}
meshdimDialog::~meshdimDialog() {
	delete param1Edit;
	delete param2Edit;
	delete param3Edit;
	delete param4Edit;
};
int meshdimDialog::param1() {
	return param1Edit->text().toInt();
}
int meshdimDialog::param2() {
	return param2Edit->text().toInt();
}
int meshdimDialog::param3() {
	return param3Edit->text().toInt();
}
QString meshdimDialog::param4() {
	return param4Edit->text();
}