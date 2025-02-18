#pragma once

/////////////////////////////////////////////////////
//cell_rcdmgr.h : 管理员主界面的借阅记录管理页面

#include <QWidget>
#include <qstandarditemmodel.h>
#include "ui_cell_rcdmgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_RcdMgrClass; };
QT_END_NAMESPACE

class Cell_RcdMgr : public QWidget
{
	Q_OBJECT

public:
	Cell_RcdMgr(QWidget *parent = nullptr);
	~Cell_RcdMgr();

	void init(QString str_cond = "");       //初始化页面

public slots:
	void on_comboBox_currentIndexChanged(); //组合框索引改变后搜索功能实现
	void on_btn_vrf_clicked();              //审核还书按钮功能实现
	void on_btn_ref_clicked();              //刷新页面按钮功能实现
	void on_btn_apr_clicked();              //批准借书按钮功能实现

private:
	Ui::Cell_RcdMgrClass *ui;
	QStandardItemModel model;
};