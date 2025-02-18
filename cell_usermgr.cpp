#include "cell_usermgr.h"
#include "sqlmgr.h"
#include "msgbox.h"
#include "add_singleuser.h"
#include "qfiledialog.h"
#include <qregularexpression.h>
#include <QDeBug>

Cell_UserMgr::Cell_UserMgr(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Cell_UserMgrClass()){
	ui->setupUi(this);
	ui->tableView->setModel(&model);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Cell_UserMgr::~Cell_UserMgr()
{
	delete ui;
}

void Cell_UserMgr::init(QString str_cond)
{
	auto l = SqlMgr::getinstance()->get_users(str_cond);
	model.clear();
	model.setHorizontalHeaderLabels(QStringList{ "id","用户名","密码","借阅总数","权限" });
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < l.size(); i++)
	{
		QList<QStandardItem*> items;
		for (int j = 0; j < l[i].size(); j++)
		{
			j==2&&l[i][j]!="123456"?items.append(new QStandardItem(QString("******"))):
			items.append(new QStandardItem(l[i][j]));
		}
		model.appendRow(items);
	}
}

void Cell_UserMgr::on_le_qry_textChanged()
{
	int idx = ui->comboBox->currentIndex();
	QString str_cond;
	switch (idx)
	{
	case 0:
	{
		str_cond = QString("where userid=%1")
			.arg(ui->le_qry->text());
		break;
	}
	case 1:
	{
		str_cond = QString("where username like '%%1%'")
			.arg(ui->le_qry->text());
		break;
	}
	case 2:
	{
		QString user_or_admin =
			ui->le_qry->text() == "" ? "" : "!";
		str_cond = QString("where admin %1=''")
			.arg(user_or_admin);
		break;
	}
	default:
		return; break;
	}
	init(str_cond);
}

void Cell_UserMgr::on_btn_imp_clicked()
{
	MsgBox msgbox = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 添加方式？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png"));

	msgbox.set_btn_text(QMessageBox::Yes, "单个添加");
	msgbox.set_btn_text(QMessageBox::No, "批量导入");
	int ret = msgbox.exec();

	if (ret == QMessageBox::Yes)
	{
		qDebug() << QString("单个添加");
		Add_SingleUser add_singleuser;
		add_singleuser.exec();
		
	}
	else if (ret == QMessageBox::No)
	{
		//批量导入
		auto str_path = QFileDialog::getOpenFileName(nullptr, "输入文件路径");

		if (!str_path.isEmpty())
		{
			QFile f(str_path);
			f.open(QFile::ReadOnly);
			QVector<QStringList> vec_data;
			QRegularExpression re("[,，;；\\s]+");

			while (!f.atEnd())
			{
				//清除无关字符的语句
				QString str = f.readLine().trimmed();
				auto l = str.split(re,Qt::SkipEmptyParts);

				while (l.size() < 4) {
					l.append("");
				}
				if (l.size() > 4) {
					l = l.mid(0, 4);
				}

				vec_data.push_back(l);
			}
			qDebug() << "\n\n";
			qDebug() << vec_data;

			SqlMgr::getinstance()->add_user(vec_data);
		}
	}
	else { return; }
	init();
}

void Cell_UserMgr::on_btn_set_clicked()
{
	int r = ui->tableView->currentIndex().row();
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中用户！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	MsgBox msgbox = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 修改操作？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png"));

	msgbox.set_btn_text(QMessageBox::Yes, "激活权限");
	msgbox.set_btn_text(QMessageBox::No, "重置密码");

	int ret_choice = msgbox.exec();

	//bhv=0，操作为重置密码，bhv=1，操作为激活权限
	int bhv = 0;
	if (ret_choice == QMessageBox::Yes)
	{
		if (model.item(r, 4)->text() == "管理员")
		{
			(!MsgBox(this,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 权限已激活！ < / div>",
				QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
			return;
		}
		int ret_act = MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认激活？ < / div>",
			QIcon(":/x64/Debug/resource/pic/question.png")).exec();
		if (ret_act == QMessageBox::Yes)
		{
			qDebug() << "激活请求发出";
			auto id = model.item(r, 0)->text();
			qDebug() << id;
			SqlMgr::getinstance()->update_users(id,++bhv);
			init();
		}
	}
	else
	{
		if (model.item(r, 2)->text() == "123456")
		{
			(!MsgBox(this,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 密码已重置！ < / div>",
				QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
			return;
		}

		int ret_rst = MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认重置？ < / div>",
			QIcon(":/x64/Debug/resource/pic/question.png")).exec();
		if (ret_rst == QMessageBox::Yes)
		{
			auto id = model.item(r, 0)->text();
			SqlMgr::getinstance()->update_users(id,bhv);
			init();
		}
	}
}

void Cell_UserMgr::on_btn_dlt_clicked()
{
	int r = ui->tableView->currentIndex().row();
	if (r < 0)
	{
		(!MsgBox(this,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 没有选中用户！ < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
		return;
	}

	int ret = MsgBox(this,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认删除？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png")).exec();

	if (ret == QMessageBox::Yes) 
	{
		QString id = model.item(r, 0)->text();
		QString str_sql = QString("where userid = %1 and (request = '已借未还' or request = '申请归还')").arg(id);
		if (SqlMgr::getinstance()->get_records(str_sql).size())
		{
			(!MsgBox(this,
				"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 删除失败！有未还图书！ < / div>",
				QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
			return;
		}
		SqlMgr::getinstance()->dlt_user(id);
		init();
	}
}

void Cell_UserMgr::on_btn_ref_clicked()
{
	init();
}