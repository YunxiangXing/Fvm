#ifndef SPHEREDIALOG_H
#define SPHEREDIALOG_H
#include<QDialog>
#include<QtWidgets>
class SphereDialog :public QDialog {
	Q_OBJECT

public:
	SphereDialog(QWidget* = nullptr);
	~SphereDialog() {
		delete param1Edit;
		delete param2Edit;
		delete param3Edit;
		delete param4Edit;
		delete param5Edit;
		delete param6Edit;
		delete param7Edit;
		delete param8Edit;
	};
	void setmesh_file();
	double param1();
	double param2();
	double param3();
	double param4();
	int param5();
	int param6();
	int param7();
	QString param8();
private:
	QLineEdit* param1Edit;
	QLineEdit* param2Edit;
	QLineEdit* param3Edit;
	QLineEdit* param4Edit;
	QLineEdit* param5Edit;
	QLineEdit* param6Edit;
	QLineEdit* param7Edit;
	QLineEdit* param8Edit;
private slots:
};

#endif
