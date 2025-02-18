#include "cell_rcdmgr.h"
#include "sqlmgr.h"
#include "msgbox.h"
#include <qdebug.h>

//使用标准模型在tableview控件上显示借阅记录信息的功能与前面页面基本相同，不再赘述
Cell_RcdMgr::Cell_RcdMgr(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Cell_RcdMgrClass())
{
	ui->setupUi(this);
	ui->tableView->setModel(&model);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Cell_RcdMgr::~Cell_RcdMgr()
{
	delete ui;
}

void Cell_RcdMgr::init(QString str_cond)
{
	auto l = SqlMgr::getinstance()->get_records(str_cond);
	model.clear();
	model.setHorizontalHeaderLabels(QStringList{ "id","用户id","图书id","图书名称","借阅时间","应还时间","借阅状态"});
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

void Cell_RcdMgr::on_comboBox_currentIndexChanged()
{
	//根据组合框控件当前的索引实现搜索功能
	int idx = ui->comboBox->currentIndex();
	QString str_cond;
	switch (idx)
	{
	case 1:
	{
		str_cond = QString("where request='申请借阅'");
		break;
	}
	case 2:
	{
		str_cond = QString("where request='已借未还'");
		break;
	}
	case 3:
	{
		str_cond = QString("where request='申请归还'");
		break;
	}
	case 4:
	{
		str_cond = QString("where request='已归还'");
		break;
	}
	default:
		break;
	}
	init(str_cond);
}


void Cell_RcdMgr::on_btn_vrf_clicked()
{
	//审核还书按钮功能实现
	int r = ui->tableView->currentIndex().row();

	//无选中判定
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中记录！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	//非法选中判定
	QString request = model.item(r, 6)->text();
	if (request!=QString("申请归还"))
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 无法批准还书！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}
	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认批准还书？ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png")).exec();
	if (ret == QMessageBox::Yes)
	{
		SqlMgr::getinstance()->verify_return(model.item(r, 0)->text(),model.item(r, 2)->text());
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 批准成功！ < / div>",
			QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
	}
	init();
}

void Cell_RcdMgr::on_btn_apr_clicked()
{
	//批准借书按钮功能实现
	int r = ui->tableView->currentIndex().row();

	//无选中判定
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中记录！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	//非法选中判定
	QString request = model.item(r, 6)->text();
	if (request != QString("申请借阅"))
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 已批准借阅！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}
	int ret= MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认批准借阅？ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png")).exec();
	if (ret==QMessageBox::Yes)
	{
		SqlMgr::getinstance()->verify_borrow(model.item(r, 0)->text(),
			model.item(r, 1)->text(), model.item(r, 2)->text());
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 批准成功！ < / div>",
			QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
	}
	init();
}

void Cell_RcdMgr::on_btn_ref_clicked()
{
	init();
}