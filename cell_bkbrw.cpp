#include "cell_bkbrw.h"
#include "sqlmgr.h"
#include "msgbox.h"
#include <QDeBug>

Cell_BkBrw::Cell_BkBrw(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Cell_BkBrwClass())
{
	ui->setupUi(this);
	//在tableview控件上使用标准模型来显示图书信息数据
	//设置信息为不可编辑，且只能选中单行
	ui->tableView->setModel(&model);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Cell_BkBrw::~Cell_BkBrw()
{
	delete ui;
}

void Cell_BkBrw::init(QString str_cond)
{
	//若当前用户的id未能成功获得，无法初始化
	if (QString("") == self_id) return;

	//获取所有图书信息数据
	auto l = SqlMgr::getinstance()->get_books(str_cond);

	//使用模型显示数据
	model.clear();
	model.setHorizontalHeaderLabels(QStringList{ "id","书名","作者","类型一","类型二","在架数量","被借总数" });
	//适应页面变化
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

void Cell_BkBrw::getid(QString self_name)
{
	//通过传入的用户名获取当前用户的id
	QString str_sql = QString("where username='%1' ")
		.arg(self_name);
	auto l = SqlMgr::getinstance()->get_users(str_sql);
	self_id = l[0][0];
	qDebug() <<" BkBrw: " << l[0][0];
}

void Cell_BkBrw::on_le_qry_textChanged()
{
	//结合组合框控件当前的搜索属性与文本输入框的已输入文本对图书信息进行查询
	int idx = ui->comboBox->currentIndex();
	QString str_cond;
	switch (idx)
	{
	case 0:
	{
		//Sql语句实现模糊查询
		str_cond = QString("where bookname like '%%1%'")
			.arg(ui->le_qry->text());
		break;
	}
	case 1:
	{
		str_cond = QString("where author like '%%1%'")
			.arg(ui->le_qry->text());
		break;
	}
	case 2:
	{
		str_cond = QString("where type1 like '%%1%' or type2 like '%%1%'")
			.arg(ui->le_qry->text());
		break;
	}
	case 3:
	{
		str_cond = QString("where bookid=%1")
			.arg(ui->le_qry->text());
		break;
	}
	default:
		return; break;
	}
	//根据查询结果重新初始化页面
	init(str_cond);
}

void Cell_BkBrw::on_btn_brw_clicked()
{
	//借阅图书按钮功能实现
	int r = ui->tableView->currentIndex().row();

	//无选中判定
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中图书！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	int access = model.item(r, 5)->text().toInt();
	//无库存判定
	if (access < 1)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有库存！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	//确认操作对话
	int ret=MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认申请？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png")).exec();

	if (ret==QMessageBox::Yes)
	{
		//新添借阅记录数据
		SqlMgr::getinstance()->borrow_books(self_id, model.item(r, 0)->text(), model.item(r, 1)->text());
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 申请成功！ < / div>",
			QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
	}
}

void Cell_BkBrw::on_btn_ref_clicked()
{
	//刷新页面按钮功能实现
	init();
}