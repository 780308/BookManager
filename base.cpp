#include "base.h"
#include "msgbox.h"
#include "dlg_login.h"
#include <QCloseEvent>

Base::Base(QWidget* parent) :QMainWindow(parent) 
{
};

Base::~Base() 
{
}

void Base::on_btn_exit_clicked() 
{
	//退出程序逻辑的实现
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 退出系统？ < / div>",
		QIcon(":/x64/Debug/resource/pic/exit.png")).exec();

	if (ret == QMessageBox::Yes) {
		exit(0);
	}
	else {
		return;
	}
}

void Base::on_btn_switch_clicked() 
{
	//切换账号逻辑的实现
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 切换账号？ < / div>",
		QIcon(":/x64/Debug/resource/pic/switch.png")).exec();

	//若选择切换账号，发送对应信号并隐藏当前窗口
	if (ret == QMessageBox::Yes) {
		emit switch_acnt();
		hide();
	}
	else {
		return;
	}
}

void Base::closeEvent(QCloseEvent* event)
{
	//鼠标点击关闭窗口事件
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 退出系统？ < / div>",
		QIcon(":/x64/Debug/resource/pic/exit.png")).exec();

	if (ret == QMessageBox::Yes) {
		event->accept();  // 接受关闭事件，关闭窗口
	}
	else {
		event->ignore();  // 忽略关闭事件，保持窗口不变
	}
}