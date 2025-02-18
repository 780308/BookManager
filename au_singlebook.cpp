#include "au_singlebook.h"
#include "sqlmgr.h"

AU_SingleBook::AU_SingleBook(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::AU_SingleBookClass())
{
	ui->setupUi(this);
	setWindowIcon(QIcon(":/x64/Debug/resource/pic/admin.png"));
	//设置窗口图标
}

AU_SingleBook::~AU_SingleBook()
{
	delete ui;
}

void AU_SingleBook::set_type(QString id,QStringList info)
{
	//当更新信息时，将传入的原始信息显示到页面的对应控件上
	m_id = id;
	ui->le_name->setText(info[0]);
	ui->le_author->setText(info[1]);
	ui->cb1->setCurrentText(info[2]);
	ui->cb2->setCurrentText(info[3]);
	ui->spinBox->setValue(info[4].toInt());
}

void AU_SingleBook::on_btn_ok_clicked()
{
	//将修改后的信息存入字符串链表中
	QStringList l;
	l << ui->le_name->text();
	l << ui->le_author->text();
	l << ui->cb1->currentText();
	l << ui->cb2->currentText();
	l << QString::number(ui->spinBox->value());
	l << QString::number(0);

	if (m_id != QString("-1"))
	{
		//更新信息分支
		SqlMgr::getinstance()->update_books(l,m_id);
	}
	else
	{
		//插入信息分支
		QVector<QStringList> v;
		v.push_back(l);
		SqlMgr::getinstance()->add_books(v);
	}
	//完成操作后关闭对话
	this->close();
}

void AU_SingleBook::on_btn_cancel_clicked()
{
	this->close();
}