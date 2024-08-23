#include"LBM_Thread.h"
LBM_Thread::LBM_Thread(QObject* parent) :QThread(parent) {

}
LBM_Thread::~LBM_Thread() {

}
void LBM_Thread::setbuildcommand(LBM3D* command,col c,bj b) {
	m_buildCommand = command;
	c_Command = c;
	b_Command = b;
}
void LBM_Thread::run() {
	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);
	connect(&process, &QProcess::readyReadStandardOutput, this, [&]() {
		QString output = process.readAllStandardOutput();
		emit outputMessage(output);
		});
	bool success = this->build();
	process.waitForFinished();
	emit buildFinished(success);
}