#pragma once

/////////////////////////////////////////////////////
//cell_bkmgr.h : 管理员主界面的图书管理页面

#include <QWidget>
#include <qstandarditemmodel.h>
#include "ui_cell_bkmgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_BkMgrClass; };
QT_END_NAMESPACE

class Cell_BkMgr : public QWidget
{
	Q_OBJECT

public:
	Cell_BkMgr(QWidget *parent = nullptr);
	~Cell_BkMgr();

	void init(QString str_cond = "");  //初始化页面

public slots:
	void on_le_qry_textChanged();      //输入文本框文字变化后搜索功能实现
	void on_btn_imp_clicked();         //导入按钮功能实现
	void on_btn_dlt_clicked();         //删除按钮功能实现
	void on_btn_set_clicked();         //更新按钮功能实现
	void on_btn_ref_clicked();         //刷新按钮功能实现
	 
private:
	Ui::Cell_BkMgrClass *ui;
	QStandardItemModel model;
};