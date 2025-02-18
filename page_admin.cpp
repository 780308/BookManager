#include "page_admin.h"
#include "msgbox.h"

Page_Admin::Page_Admin(QWidget *parent)
	: Base(parent)
	, ui(new Ui::Page_AdminClass())
	, analy(nullptr)
	, bkmgr(nullptr)
	, rcdmgr(nullptr)
	, usermgr(nullptr)
	//初始化各成员指针
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(":/x64/Debug/resource/pic/admin.png"));
	ui->btn_bkmgr->setIcon(QIcon(":/x64/Debug/resource/pic/book_mg.png"));
	ui->btn_usermgr->setIcon(QIcon(":/x64/Debug/resource/pic/user_mg.png"));
	ui->btn_analy->setIcon(QIcon(":/x64/Debug/resource/pic/analysis.png"));
	ui->btn_rcdmgr->setIcon(QIcon(":/x64/Debug/resource/pic/record_mg.png"));
	ui->btn_exit->setIcon(QIcon(":/x64/Debug/resource/pic/exit_w.png"));
	ui->btn_switch->setIcon(QIcon(":/x64/Debug/resource/pic/switch_w.png"));
	//设置主界面各按钮图标
	initpage();
}

Page_Admin::~Page_Admin()
{
	delete ui;
}

void Page_Admin::pop_message()
{
	//管理员页面提示信息
	(!MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 10px;' > 尊敬的管理员，请您尽快批准未批准记录！ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
}

void Page_Admin::initpage()
{
	analy = new Cell_Analysis(this);
	bkmgr = new Cell_BkMgr(this);
	rcdmgr = new Cell_RcdMgr(this);
	usermgr = new Cell_UserMgr(this);

	//实现堆栈窗口逻辑
	ui->stackedWidget->addWidget(bkmgr);
	ui->stackedWidget->addWidget(usermgr);
	ui->stackedWidget->addWidget(rcdmgr);
	ui->stackedWidget->addWidget(analy);

	ui->stackedWidget->setCurrentIndex(0);

	auto l = ui->menu->children();
	for (auto it : l)
	{
		if (it->objectName().contains("btn"))
		{
			//将点击导航栏按钮信号和显示对应页面函数连接起来
			connect(static_cast<QToolButton*>(it), &QToolButton::clicked, this, &Page_Admin::dealmenu);
		}
	}
	bkmgr->init();
	usermgr->init();
	rcdmgr->init();
	analy->init();
}

void Page_Admin::dealmenu()
{
	//实现导航栏逻辑
	auto str = sender()->objectName();
	do
	{
		if ("btn_bkmgr"==str)
		{
			ui->stackedWidget->setCurrentIndex(0);
			if (!msg_cnt++) pop_message();
			break;
		}
		if ("btn_usermgr" == str)
		{
			ui->stackedWidget->setCurrentIndex(1);
			if (!msg_cnt++) pop_message();
			break;
		}
		if ("btn_rcdmgr" == str)
		{
			ui->stackedWidget->setCurrentIndex(2);
			if (!msg_cnt++) pop_message();
			break;
		}
		if ("btn_analy" == str)
		{
			ui->stackedWidget->setCurrentIndex(3);
			if (!msg_cnt++) pop_message();
			break;
		}

	} while (false);
}