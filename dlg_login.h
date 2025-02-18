#pragma once

/////////////////////////////////////////////////////
//dlg_login.h : 登录对话

#include <QDialog>
#include "ui_dlg_login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dlg_LoginClass; };
QT_END_NAMESPACE

class Dlg_Login : public QDialog
{
	Q_OBJECT

public:
	Dlg_Login(QWidget* parent = nullptr);
	~Dlg_Login();

	void closeEvent(QCloseEvent* e);           //鼠标点击关闭事件

signals:
	void admin_lgn();                          //管理员成功登录信号
	void user_lgn();                           //用户成功登录信号
	void log_success(const QString& username); //用户成功登录后传递用户名信号

public slots:
	void on_btn_userlgn_clicked();             //用户登录按钮功能实现
	void on_btn_adminlgn_clicked();            //管理员登录按钮功能实现
	 
private:
	Ui::Dlg_LoginClass *ui;
};