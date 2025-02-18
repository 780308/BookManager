#include "cell_self.h"
#include "sqlmgr.h"
#include "msgbox.h"
#include <qdebug.h>

Cell_Self::Cell_Self(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Cell_SelfClass())
{
	ui->setupUi(this);
	ui->tableView->setModel(&model);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	//将提醒修改初始密码的信号和对应的槽函数连接起来
	connect(this, &Cell_Self::show_pop, this, &Cell_Self::rmd_rstpwd);
}

Cell_Self::~Cell_Self()
{
	delete ui;
}

void Cell_Self::getid(QString self_name)
{
	QString str_sql = QString("where username='%1' ")
		.arg(self_name);

	//通过当前的用户名查找到用户信息
	auto info = SqlMgr::getinstance()->get_users(str_sql)[0];
	self_id = info[0];
	name = info[1];
	ui->le_name->setText(info[1]);
	ui->le_pwd->setText(info[2]);
	ui->le_pwd->setEchoMode(QLineEdit::Password);
	ui->le_name->setReadOnly(true);
	ui->le_pwd->setReadOnly(true);

	//若当前用户密码为初始密码，发送提示修改密码的信号
	if (info[2] == QString("123456"))
		emit show_pop();

	qDebug() <<"Self: " << self_id;
	qDebug() << "Name: " << info[1];
}

void Cell_Self::init(QString str_cond)
{
	ui->le_pwd->setReadOnly(true);
	ui->le_name->setReadOnly(true);
	if (QString("") == self_id) return;

	//若用户修改了账号密码，显示修改后的账号密码
	QString str_sql = QString("where userid=%1 ")
		.arg(self_id);
	auto info = SqlMgr::getinstance()->get_users(str_sql)[0];
	ui->le_name->setText(info[1]);
	ui->le_pwd->setText(info[2]);

	QString user = QString("where userid= %1 ").arg(self_id);
	auto l = SqlMgr::getinstance()->get_records(user + str_cond);
	model.clear();
	model.setHorizontalHeaderLabels(QStringList{ "id","用户id","图书id","图书名称","借阅时间","应还时间","借阅状态" });
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < l.size(); i++)
	{
		QList<QStandardItem*> items;
		for (int j = 0; j < l[i].size(); j++)
			items.append(new QStandardItem(l[i][j]));

		model.appendRow(items);
	}
}

//由于代码复用性较好，搜索功能和申请功能与之前页面中的对应功能的编写相近，不再注释
void Cell_Self::on_comboBox_currentIndexChanged()
{
	int idx = ui->comboBox->currentIndex();
	QString str_cond="";
	switch (idx)
	{
	case 1:
	{
		str_cond = QString("and request='申请借阅'");
		break;
	}
	case 2:
	{
		str_cond = QString("and request='已借未还'");
		break;
	}
	case 3:
	{
		str_cond = QString("and request='申请归还'");
		break;
	}
	case 4:
	{
		str_cond = QString("and request='已归还'");
		break;
	}
	default:
		break;
	}
	init(str_cond);
}

void Cell_Self::on_btn_req_clicked()
{
	int r = ui->tableView->currentIndex().row();
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中记录！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	QString access = model.item(r, 6)->text();
	if (access!=QString("已借未还"))
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 当前不能申请归还！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认申请？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png")).exec();

	if (ret == QMessageBox::Yes)
	{
		SqlMgr::getinstance()->return_books(model.item(r, 0)->text());
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 申请成功！ < / div>",
			QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
	}
	init();
}

void Cell_Self::on_btn_showpwd_clicked()
{
	//显示/隐藏密码按钮功能实现
	if (ui->btn_showpwd->text() == QString("显示密码"))
	{
		ui->btn_showpwd->setText("隐藏密码");
		ui->le_pwd->setEchoMode(QLineEdit::Normal);
	}
	else
	{
		ui->btn_showpwd->setText("显示密码");
		ui->le_pwd->setEchoMode(QLineEdit::Password);
	}
}

void Cell_Self::on_btn_set_clicked()
{
	//修改信息按钮功能实现
	ui->le_pwd->setReadOnly(false);
	ui->le_name->setReadOnly(false);
}

void Cell_Self::on_btn_save_clicked()
{
	//保存修改按钮功能实现
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认保存修改？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png")).exec();

	if (ret == QMessageBox::Yes)
	{
		QString duplicate = QString("where username = '%1'").arg(ui->le_name->text());
		if (!SqlMgr::getinstance()->get_users(duplicate).size() || name == ui->le_name->text()) {
			SqlMgr::getinstance()->set_userinfo(self_id, ui->le_name->text(), ui->le_pwd->text());
			(!MsgBox(nullptr,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 修改成功！ < / div>",
				QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
		}
		else
		{
			(!MsgBox(this,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 用户名已被使用，请更换！ < / div>",
				QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		}
	}
	else { return; }
	init();
}

void Cell_Self::rmd_rstpwd()
{
	//提示修改初始密码对话框
	(!MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 您的密码仍为初始密码，请更换！ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
}

void Cell_Self::on_btn_ref_clicked()
{
	init();
}