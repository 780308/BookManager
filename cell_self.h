#pragma once

/////////////////////////////////////////////////////
//cell_rcdmgr.h : 管理员主界面的借阅记录管理页面

#include <QWidget>
#include <qstandarditemmodel.h>
#include "ui_cell_self.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_SelfClass; };
QT_END_NAMESPACE

class Cell_Self : public QWidget
{
	Q_OBJECT

public:
	Cell_Self(QWidget *parent = nullptr);
	~Cell_Self();

	void getid(QString self_name);          //根据登录输入的账号获取用户信息并显示在对应控件上
	void init(QString str_cond = "");       //初始化页面

signals:
	void show_pop();                        //登录成功后发送信号提示弹出登录信息

public slots:
	void on_comboBox_currentIndexChanged(); //组合框索引改变后搜索功能实现
	void on_btn_req_clicked();              //申请还书按钮功能实现
	void on_btn_ref_clicked();              //刷新页面按钮功能实现
	void on_btn_showpwd_clicked();          //显示密码按钮功能实现
	void on_btn_set_clicked();              //修改信息按钮功能实现
	void on_btn_save_clicked();             //保存修改按钮功能实现

	void rmd_rstpwd();                      //提示修改初始密码功能实现

private:
	Ui::Cell_SelfClass *ui;
	QString self_id="";                     //当前用户的id
	QString name = "";                      //当前用户的用户名
	QStandardItemModel model;               //用于显示个人借阅信息的模型
};