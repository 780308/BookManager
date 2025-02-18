#pragma once

/////////////////////////////////////////////////////
//au_singlebook.h : 添加/修改单个图书的对话

#include <QDialog>
#include "ui_au_singlebook.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AU_SingleBookClass; };
QT_END_NAMESPACE

class AU_SingleBook : public QDialog
{
	Q_OBJECT

public:
	AU_SingleBook(QWidget *parent = nullptr);
	~AU_SingleBook();

	void set_type(QString,QStringList);     //更新信息时将原始信息显示到对应控件

public slots:
	void on_btn_ok_clicked();               //确认按钮功能实现
	void on_btn_cancel_clicked();           //取消按钮功能实现

private:
	Ui::AU_SingleBookClass *ui;

	QString m_id="-1";                      //值为-1时实现添加，否则实现更新
};