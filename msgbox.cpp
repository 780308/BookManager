#include "msgbox.h"
#include <QAbstractButton>

MsgBox::MsgBox(QWidget* parent, const QString& text, QIcon icon,
	StandardButtons btns, const QString& title, StandardButton dftbtn):
	QMessageBox(parent)
{
	setWindowTitle(title);
	setText(text);
	setStandardButtons(btns);
	setDefaultButton(dftbtn);
	setWindowIcon(icon);
	//将文本设置为接受html语言
	setTextFormat(Qt::RichText);
}

void MsgBox::set_btn_text(StandardButton btn, const QString& btn_txt)
{
	//修改按钮文本
	QAbstractButton* diy_btn = button(btn);
	if (diy_btn)
		diy_btn->setText(btn_txt);
}

MsgBox& MsgBox::operator!()
{
	//'!'运算符重载：隐藏按钮，只显示信息
	QList<QAbstractButton*> buttons = this->buttons();
	for (QAbstractButton* button : buttons)
	{
		button->hide();
	}
	return *this;
}

void MsgBox::closeEvent(QCloseEvent* event)
{
	//将鼠标点击关闭事件和点击“No/Cancel”按钮事件区分开
	QDialog::done(Rejected - 1);
}