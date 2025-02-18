#include "cell_bkmgr.h"
#include "sqlmgr.h"
#include "msgbox.h"
#include "au_singlebook.h"
#include "qfiledialog.h"
#include <qregularexpression.h>
#include <QDeBug>

Cell_BkMgr::Cell_BkMgr(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Cell_BkMgrClass())
{
	ui->setupUi(this);
	//使用标准模型在tableview控件上显示图书信息
	ui->tableView->setModel(&model);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Cell_BkMgr::~Cell_BkMgr()
{
	delete ui;
}

void Cell_BkMgr::init(QString str_cond)
{
	//根据查询关键词初始化页面
	auto l = SqlMgr::getinstance()->get_books(str_cond);
	model.clear();
	model.setHorizontalHeaderLabels(QStringList{ "id","书名","作者","类型一","类型二","在架数量","被借总数"});
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

void Cell_BkMgr::on_le_qry_textChanged()
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

void Cell_BkMgr::on_btn_imp_clicked()
{
	//导入按钮功能实现
	MsgBox msgbox = MsgBox(nullptr,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 添加方式？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png"));

	msgbox.set_btn_text(QMessageBox::Yes, "单个添加");
	msgbox.set_btn_text(QMessageBox::No, "批量导入");
	int ret = msgbox.exec();

	if (ret == QMessageBox::Yes)
	{
		qDebug() << QString("单个添加");
		AU_SingleBook au_singlebook;
		au_singlebook.exec();
	}
	else if (ret == QMessageBox::No)
	{
		//批量导入，参考资料
		auto str_path = QFileDialog::getOpenFileName(nullptr, "输入文件路径");

		if (!str_path.isEmpty())
		{
			QFile f(str_path);
			f.open(QFile::ReadOnly);
			QVector<QStringList> vec_data;

			//使用正则表达式设置读取文件时的分隔符
			QRegularExpression re("[,，;；\\s]+");

			while (!f.atEnd())
			{
				//一次读取一行内容
				QString str = f.readLine().trimmed();

				//清除无关字符
				auto l = str.split(re, Qt::SkipEmptyParts);

				//只读取当前行的第一条数据，若数据不完整，用""补全
				while (l.size() < 5) {
					l.append("");
				}
				l.append(QString::number(0));

				if (l.size() > 6) {
					l = l.mid(0, 6);
				}

				vec_data.push_back(l);
			}
			qDebug() << "\n\n";
			qDebug() << vec_data;

			SqlMgr::getinstance()->add_books(vec_data);
		}
	}
	else { return; }
	init();
}

void Cell_BkMgr::on_btn_set_clicked()
{
	//更新图书按钮功能实现
	int r = ui->tableView->currentIndex().row();
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中图书！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}
	else
	{
		auto id = model.item(r, 0)->text();
		AU_SingleBook au_singlebook;
		
		//将当前选中行的图书信息传到添加/更新图书信息对话
		QStringList info;
		info << model.item(r, 1)->text();
		info << model.item(r, 2)->text();
		info << model.item(r, 3)->text();
		info << model.item(r, 4)->text();
		info << model.item(r, 5)->text();

		au_singlebook.set_type(QString(id),info);
		au_singlebook.exec();
		init();
	}
}

void Cell_BkMgr::on_btn_dlt_clicked()
{
	//删除按钮功能实现
	int r = ui->tableView->currentIndex().row();

	//无选中判定
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中图书！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认删除？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png")).exec();

	if (ret == QMessageBox::Yes)
	{
		QString id = model.item(r, 0)->text();
		QString str_sql = QString("where bookid = %1 and request != '已归还'").arg(id);

		//非法删除判定：若当前图书有外借库存，判定删除操作非法
		if (SqlMgr::getinstance()->get_records(str_sql).size())
		{
			(!MsgBox(this,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 删除失败！有外借库存！ < / div>",
				QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
			return;
		}
		SqlMgr::getinstance()->dlt_books(id);
		init();
	}
}

void Cell_BkMgr::on_btn_ref_clicked()
{
	init();
}