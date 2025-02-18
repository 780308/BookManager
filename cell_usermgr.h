#pragma once


/////////////////////////////////////////////////////
//cell_usermgr.h : 管理员主界面的用户信息管理页面

#include <QWidget>
#include "ui_cell_usermgr.h"
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_UserMgrClass; };
QT_END_NAMESPACE

class Cell_UserMgr : public QWidget
{
	Q_OBJECT

public:
	Cell_UserMgr(QWidget *parent = nullptr);
	~Cell_UserMgr();

	void init(QString str_cond="");  //初始化页面

public slots:
	void on_le_qry_textChanged();    //输入文本框文本改变后搜索功能实现
	void on_btn_imp_clicked();       //导入按钮功能实现
	void on_btn_dlt_clicked();       //删除按钮功能实现
	void on_btn_set_clicked();       //修改按钮功能实现
	void on_btn_ref_clicked();       //刷新按钮功能实现

private:
	Ui::Cell_UserMgrClass *ui;
	QStandardItemModel model;
};