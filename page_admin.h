#pragma once

/////////////////////////////////////////////////////
//page_admin.h : 管理员主界面

#include "base.h"
#include "ui_page_admin.h"
#include "cell_analysis.h"
#include "cell_bkmgr.h"
#include "cell_rcdmgr.h"
#include "cell_usermgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Page_AdminClass; };
QT_END_NAMESPACE

class Page_Admin : public Base
{
	Q_OBJECT

public:
	Page_Admin(QWidget *parent = nullptr);
	~Page_Admin();

	void initpage();         //初始化界面
	void dealmenu();         //实现导航栏切换当前页面
	void pop_message();      //基类Base的对应抽象函数的实现，为管理员登录提示信息

private:
	Ui::Page_AdminClass *ui;

	Cell_Analysis* analy;    //统计分析页面类指针
	Cell_BkMgr* bkmgr;       //图书管理页面类指针
	Cell_RcdMgr* rcdmgr;     //借阅记录页面类指针
	Cell_UserMgr* usermgr;   //用户管理页面类指针

	int msg_cnt = 0;         //统计登录提示信息弹出次数
};