#include "QtWidgetsApplication1.h"
#include"x64/Debug/uic/ui_QtWidgetsApplication1.h"
//#include <mesh.h>
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QWidget(parent)
{
    LBM_test_mesh = new LBM3D();
    ui.setupUi(this);
    LBM_test_mesh->getMesh()->setQTextedit(ui.textEdit);
    QFont font("TimesRomen", 12); // 使用微软雅黑字体，大小为 12
    ui.textEdit->setFont(font); // 将字体应用到 QTextEdit 控件上
    connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication1::setmeshpoint_Button);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::Sphere_Button);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &QtWidgetsApplication1::Output_Button);
    connect(ui.Porosity,&QPushButton::clicked,this, &QtWidgetsApplication1::Por_Button);
    connect(ui.SRTLBM, &QPushButton::clicked, this, &QtWidgetsApplication1::SRT_Button);
    connect(ui.MRTLBM, &QPushButton::clicked, this, &QtWidgetsApplication1::MRT_Button);
    connect(ui.OutputResult, &QPushButton::clicked, this, &QtWidgetsApplication1::Result_Button);
    connect(ui.Permeability, &QPushButton::clicked, this, &QtWidgetsApplication1::Per_Button);
    connect(ui.ZhouHe, &QPushButton::clicked, this, &QtWidgetsApplication1::Bj_Button);
    connect(ui.Calculate, &QPushButton::clicked, this, &QtWidgetsApplication1::Cal_Button);
}
QtWidgetsApplication1::~QtWidgetsApplication1()
{
    if (lbm_thread) {
        lbm_thread->quit(); // 结束线程的事件循环
        lbm_thread->wait(); // 等待线程结束
        delete lbm_thread;
    }
    delete LBM_test_mesh;
}


void QtWidgetsApplication1::setmeshpoint_Button() {
    meshdimDialog inputdim;
    if (inputdim.exec() == QDialog::Accepted) {
        int dim1 = inputdim.param1() + 7;
        int dim2 = inputdim.param2() + 1;
        int dim3 = inputdim.param3() + 1;
        QString mesh_file = inputdim.param4();
        LBM_test_mesh->getMesh()->setdim123(dim1, dim2, dim3);
        LBM_test_mesh->getMesh()->reading(mesh_file.toStdString());
    }
}
void QtWidgetsApplication1::Sphere_Button() {
    SphereDialog spheremesh;
    if (spheremesh.exec() == QDialog::Accepted) {
        double cirx = spheremesh.param1();
        double ciry = spheremesh.param2();
        double cirz = spheremesh.param3();
        double R = spheremesh.param4();
        int dim1 = spheremesh.param5() + 7;
        int dim2 = spheremesh.param6() + 1;
        int dim3 = spheremesh.param7() + 1;
        LBM_test_mesh->getMesh()->setdim123(dim1, dim2, dim3);
        LBM_test_mesh->getMesh()->sphere(cirx, ciry, cirz, R);
    }
}
void QtWidgetsApplication1::Output_Button() {
    if (LBM_test_mesh->getMesh()->getdim1() == 0 && LBM_test_mesh->getMesh()->getdim2() == 0 && LBM_test_mesh->getMesh()->getdim3() == 0) {
        QMessageBox::information(this, "error", "not found mesh!");
    }
    else {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), QDir::homePath(), tr(".dat", "All Files (*.*)"));
        if (!fileName.isEmpty()) {
            LBM_test_mesh->getMesh()->output(fileName.toStdString());
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to save file.");
        }
    }
}
void QtWidgetsApplication1::Por_Button() {
    ui.textEdit->append("porosity = " + QString::number(LBM_test_mesh->getMesh()->getpolor()) + "%");
}
void QtWidgetsApplication1::SRT_Button() {
    ui.textEdit->append("model: SRT-LBM");
    LBM_col = col::SRT;
}
void QtWidgetsApplication1::MRT_Button() {
    ui.textEdit->append("model: MRT-LBM");
    LBM_col = col::MRT;
}
void QtWidgetsApplication1::Result_Button() {
    if (LBM_col == col::Not_define || LBM_bj == bj::Not_define) {
        QMessageBox::information(this, "error", "not found Result!");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save File"), QDir::homePath(), tr("dat file(*.dat);;All Files (*.*)"));
    if (!fileName.isEmpty()) {
        LBM_test_mesh->outputlast(fileName.toStdString());
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save file.");
    }
}
void QtWidgetsApplication1::Per_Button() {
    if (LBM_col == col::Not_define || LBM_bj == bj::Not_define) {
        QMessageBox::information(this, "error", "not found Result!");
        return;
    }
    ui.textEdit->append("permeability = " + QString::number(LBM_test_mesh->kLB()));
}
void QtWidgetsApplication1::Bj_Button() {
    ui.textEdit->append("boundary = Zhou-He");
    LBM_bj = bj::Zhou_He;
}
void QtWidgetsApplication1::Timer_out() {
    LBM_test_mesh->getMesh()->gettextedit()->append("E");
}
void QtWidgetsApplication1::Cal_Button() {
    if (LBM_col == col::Not_define || LBM_bj == bj::Not_define) {
        QMessageBox::information(this, "error", "not set model");
        return;
    }
    //LBM_timer = new QTimer();
    //connect(LBM_timer, &QTimer::timeout, this, &QtWidgetsApplication1::Timer_out);
    //LBM_timer->start(1000);
    LBM_test_mesh->build();
    lbm_thread = new LBM_Thread(this);
    lbm_thread->setbuildcommand(LBM_test_mesh, LBM_col, LBM_bj);
    connect(lbm_thread, &LBM_Thread::outputMessage, this, [&](const QString& message) {
        ui.textEdit->append(message); // 在界面上实时显示输出
        });
    connect(lbm_thread, &LBM_Thread::buildFinished, this, [&](bool success) {
        if (success) {
            
        }
        else {
            
        }
        });
    lbm_thread->start();
    //LBM_test_mesh->cal(LBM_col, LBM_bj);
}