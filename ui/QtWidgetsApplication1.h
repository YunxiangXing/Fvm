#pragma once
#ifndef QTWIGGETSAPPLICATION1
#define QTWIGGETSAPPLICATION1
#include <QMessageBox>
#include <QtWidgets/QWidget>
#include "ui_QtWidgetsApplication1.h"
#include <QProcess>
#include <QTimer>
#include "LBM\LBM1.h"
#include "meshdim.h"
#include "SphereDialog.h"
#include "LBM\LBM_Thread.h"
class QtWidgetsApplication1 : public QWidget
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();
private:
    Ui::QtWidgetsApplication1Class ui;
    LBM3D* LBM_test_mesh = NULL;
    col LBM_col = col::Not_define;
    bj LBM_bj = bj::Not_define;
    QTimer* LBM_timer = NULL;
    LBM_Thread* lbm_thread = NULL;
private slots:
    void setmeshpoint_Button();
    void Sphere_Button();
    void Output_Button();
    void Por_Button();
    void SRT_Button();
    void MRT_Button();
    void Result_Button();
    void Per_Button();
    void Cal_Button();
    void Bj_Button();
    void Timer_out();
};
#endif 