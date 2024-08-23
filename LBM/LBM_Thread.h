#ifndef LBM_THREAD_H
#define LBM_THREAD_H
#include <QThread>
#include <QStringList>
#include <QProcess>
#include "LBM\LBM1.h"
class LBM_Thread :public QThread {
	Q_OBJECT
public:
	explicit LBM_Thread(QObject* parent = nullptr);
	~LBM_Thread();
	void setbuildcommand(LBM3D*,col,bj);
	bool build() {
		if (m_buildCommand) {
			m_buildCommand->cal(c_Command, b_Command);
			return 1;
		}
		else return 0;
	}
signals:
	void outputMessage(const QString& message);
	void buildFinished(bool success);
protected:
	void run() override;
private:
	LBM3D* m_buildCommand = NULL;
	col c_Command;
	bj b_Command;
};

#endif