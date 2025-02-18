#pragma once

/////////////////////////////////////////////////////
//add_singleuser.h : 添加单个用户的对话

#include <QDialog>
#include "ui_add_singleuser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Add_SingleUserClass; };
QT_END_NAMESPACE

class Add_SingleUser : public QDialog
{
	Q_OBJECT

public:
	Add_SingleUser(QWidget *parent = nullptr);
	~Add_SingleUser();

public slots:
	void on_btn_add_clicked();        //添加按钮功能实现
	void on_btn_cancel_clicked();     //取消按钮功能实现

private:
	Ui::Add_SingleUserClass *ui;
};