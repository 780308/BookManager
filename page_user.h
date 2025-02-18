#pragma once

/////////////////////////////////////////////////////
//page_user.h : 用户主界面

#include "ui_Page_user.h"
#include "base.h"
#include "cell_self.h"
#include "cell_bkbrw.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Page_userClass; };
QT_END_NAMESPACE

class Page_user : public Base
{
	Q_OBJECT

public:
	Page_user(QWidget *parent = nullptr);
	~Page_user();

	void dealmenu();         //实现导航栏切换当前页面
	void pop_message();      //基类Base的对应抽象函数的实现，为用户登录提示信息

public slots:
	void initpage();         //初始化页面   

	//根据登录成功之后登录对话发送的用户名，设置两个子页面需要的当前用户信息
	void set_username(const QString& username);

private:
	Ui::Page_userClass *ui;

	Cell_BkBrw* bkbrw;       //图书查询/借阅页面类指针
	Cell_Self* self;         //个人信息页面类指针
};