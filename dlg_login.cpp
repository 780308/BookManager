#include "dlg_login.h"
#include "msgbox.h"
#include "sqlmgr.h"
#include <QCloseEvent>

Dlg_Login::Dlg_Login(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Dlg_LoginClass())
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(":/x64/Debug/resource/pic/login.png"));
	//设置窗口图标
}

Dlg_Login::~Dlg_Login()
{
	delete ui;
}

void Dlg_Login::on_btn_adminlgn_clicked()
{
	//管理员登录按钮功能实现，若登录成功，发送对应信号
	if (SqlMgr::getinstance()->login(ui->le_name->text(), ui->le_pwd->text(),1))
	{
		emit admin_lgn();
		hide();
		ui->le_name->setText("");
		ui->le_pwd->setText("");
	}
}

void Dlg_Login::on_btn_userlgn_clicked()
{
	//用户登录按钮功能实现，若登录成功，发送对应信号
	if (SqlMgr::getinstance()->login(ui->le_name->text(), ui->le_pwd->text(),0))
	{
		emit log_success(ui->le_name->text());
		emit user_lgn();
		hide();
		ui->le_name->setText("");
		ui->le_pwd->setText("");
	}
}

void Dlg_Login::closeEvent(QCloseEvent* event)
{
	//鼠标点击关闭事件
	int ret = MsgBox(nullptr, 
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 是否退出？ < / div>",
		QIcon(":/x64/Debug/resource/pic/exit_b.png")).exec();

	if (ret == QMessageBox::Yes) {
		event->accept();  //接受关闭事件，关闭窗口
	}
	else {
		event->ignore();  //忽略关闭事件，保持窗口不变
	}
}