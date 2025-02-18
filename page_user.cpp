#include "Page_user.h"
#include "msgbox.h"
#include <QDeBug>

Page_user::Page_user(QWidget *parent)
	: Base(parent)
	, ui(new Ui::Page_userClass())
	, bkbrw(new Cell_BkBrw(this))
	, self(new Cell_Self(this))
	//初始化各成员指针
{
	ui->setupUi(this);
	setWindowIcon(QIcon(":/x64/Debug/resource/pic/books.png"));
	ui->btn_info->setIcon(QIcon(":/x64/Debug/resource/pic/info.png"));
	ui->btn_qry->setIcon(QIcon(":/x64/Debug/resource/pic/qry.png"));
	ui->btn_exit->setIcon(QIcon(":/x64/Debug/resource/pic/exit_w.png"));
	ui->btn_switch->setIcon(QIcon(":/x64/Debug/resource/pic/switch_w.png"));
	//设置主界面各按钮图标

	initpage();
}

Page_user::~Page_user()
{
	delete ui;
}

void Page_user::pop_message()
{
	//用户页面提示信息
	(!MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 10px;' > 尊敬的用户，如有将逾期图书请尽快归还！ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
}

void Page_user::initpage()
{
	//实现堆栈窗口逻辑
	ui->stackedWidget->addWidget(self);
	ui->stackedWidget->addWidget(bkbrw);

	ui->stackedWidget->setCurrentIndex(0);

	auto l = ui->menu->children();
	for (auto it : l)
	{
		if (it->objectName().contains("btn"))
		{
			//将点击导航栏按钮信号和显示对应页面函数连接起来
			connect(static_cast<QToolButton*>(it), &QToolButton::clicked, this, &Page_user::dealmenu);
		}
	}
	self->init();
	bkbrw->init();
}

void Page_user::dealmenu()
{
	//实现导航栏逻辑
	auto str = sender()->objectName();
	do
	{
		if ("btn_info" == str)
		{
			ui->stackedWidget->setCurrentIndex(0);
			break;
		}
		if ("btn_qry" == str)
		{
			ui->stackedWidget->setCurrentIndex(1);
			break;
		}

	} while (false);
}

void Page_user::set_username(const QString& username) 
{
	//用户登录成功后将登录对话中输入的用户名传递到用户信息和图书借阅的页面中
	bkbrw->getid(username);
	self->getid(username);

	pop_message();
}