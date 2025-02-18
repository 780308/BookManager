#pragma once

/////////////////////////////////////////////////////
//msgbox.h : 自定义消息对话框类

#include <QMessageBox>

class MsgBox :public QMessageBox
{
public:
	//默认消息对话框：无父指针，无文本，无标题，按钮为Yes|No，图标为默认图标
	MsgBox(QWidget* parent = nullptr, const QString& text = " ",
		 QIcon icon = QIcon(":/x64/Debug/resource/pic/default.png"), StandardButtons btns=Yes|No,
		const QString& title = " ", StandardButton dftbtn=NoButton);

	void set_btn_text(StandardButton btn, const QString& btn_txt);  //修改按钮文本

	MsgBox& operator !();                                           //'!'运算符重载

	void closeEvent(QCloseEvent* event) override;                   //鼠标点击关闭事件重载                 
};