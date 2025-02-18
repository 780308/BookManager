#pragma once

/////////////////////////////////////////////////////
//cell_brbrw.h : 用户主界面的图书查询/借阅页面

#include <QWidget>
#include <QStandardItemModel>
#include "ui_cell_bkbrw.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_BkBrwClass; };
QT_END_NAMESPACE

class Cell_BkBrw : public QWidget
{
	Q_OBJECT

public:
	Cell_BkBrw(QWidget *parent = nullptr);
	~Cell_BkBrw();

	void init(QString str_cond="");  //初始化个人借阅记录
	void getid(QString self_name);   //根据登录时的用户名查询用户信息，
									 //并显示到对应控件上
public slots:
	void on_le_qry_textChanged();    //输入文本框文字变化后功能实现
	void on_btn_brw_clicked();       //借阅按钮功能实现
	void on_btn_ref_clicked();       //刷新按钮功能实现


private:
	Ui::Cell_BkBrwClass *ui;
	QString self_id="";              //用户id
	QStandardItemModel model;        //使用标准模型在界面控件上显示记录
};