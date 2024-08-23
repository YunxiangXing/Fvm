#ifndef	MESHDIM_H
#define MESHDIM_H
#include<QDialog>
#include<QtWidgets>
class meshdimDialog :public QDialog {
	Q_OBJECT

public:
	meshdimDialog(QWidget* = nullptr);
	~meshdimDialog();
	int param1();
	int param2();
	int param3();
	QString param4();
private:
	QLineEdit* param1Edit;
	QLineEdit* param2Edit;
	QLineEdit* param3Edit;
	QLineEdit* param4Edit;
private slots:
	void setmesh_file();
};

#endif