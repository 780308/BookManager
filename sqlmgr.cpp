#include "sqlmgr.h"
#include <msgbox.h>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

//初始化静态成员
SqlMgr* SqlMgr::instance = nullptr;

SqlMgr::SqlMgr()
{
}

SqlMgr::~SqlMgr()
{
}

SqlMgr* SqlMgr::getinstance()
{
	//在单例模式中获取全局唯一对象
	if (nullptr == instance) {
		instance = new SqlMgr();
	}
	return instance;
}

void SqlMgr::init()
{
	//导入程序内置数据库
	QString absolutePath = QCoreApplication::applicationDirPath()+ "/resource/db/bk_mgr.db";
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(absolutePath);

	if (!db.open()) {
		       (!MsgBox(nullptr,
			"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 数据库导入失败! < / div>",
			QIcon(":/x64/Debug/resource/pic/error.png"))).exec();
	}
}

bool SqlMgr::login(QString str_name, QString str_pwd, int admin)
{
	//实现登录逻辑：在数据库中查找用户名、密码以及权限与登录对话的对应输入相符的用户数据，如果有匹配项则登录成功，否则失败
	QSqlQuery q(db);
	QString str_sql;
	if (admin) {
		//管理员登录
		str_sql = QString("select * from user where username='%1' and password='%2' and admin!=''")
			.arg(str_name).arg(str_pwd);
	}
	else{
		//用户登录
		str_sql = QString("select * from user where username='%1' and password='%2'")
			.arg(str_name).arg(str_pwd);
	}
	if (q.exec(str_sql))
	{
		//登录成功
		while (q.next()) {
			return true;
		}
	}
	else
	{
		qDebug() << q.lastError().text();
	}

	//登录失败提示
	(!MsgBox(nullptr,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 用户名/密码错误！ < / div>",
		QIcon(":/x64/Debug/resource/pic/error.png"))).exec();

	return false;
}

QVector<QStringList> SqlMgr::get_users(QString str_cond)
{
	//实现获取用户：在数据库中使用传入的搜索语句搜索满足对应条件的用户数据
	QSqlQuery q(db);
	QString str_sql;

	str_sql = QString("select * from user %1").arg(str_cond);
	qDebug() << str_sql;

	QVector<QStringList> vec;
	if (q.exec(str_sql))
	{
		int cols = q.record().count();
		QStringList l;
		while (q.next())
		{
			l.clear();
			for (int i = 0; i < cols; i++)
			{
				l << q.value(i).toString();
			}
			vec.push_back(l);
		}
	}
	else { qDebug() << q.lastError().text(); }
	return vec;
}

void SqlMgr::add_user(QString str_name,QString& str_admin)
{
	//实现添加用户：由输入的文本插入一条新的用户数据
	QSqlQuery q(db);
	QString str_sql = QString("insert into user values(null,'%1','123456',0,'%2')")
		.arg(str_name).arg(str_admin);
	if (!q.exec(str_sql))
	{
		qDebug() << q.lastError().text();
	}
}

void SqlMgr::add_user(QVector<QStringList> v)
{
	//实现批量导入：根据输入的文件路径进行批量插入
	db.transaction();
	QSqlQuery q(db);
	for (auto it : v)
	{
		QString str_sql = QString("insert into user VALUES(NULL,'%1','%2','%3','%4');")
			.arg(it[0])
			.arg(it[1])
			.arg(it[2])
			.arg(it[3]);
		bool ret = q.exec(str_sql);
		if (!ret)
			qDebug() << q.lastError().text();
	}

	db.commit();

}

void SqlMgr::update_users(QString str_id,int bhv)
{
	//实现更新用户：根据选择的具体更新操作，激活当前选中用户的管理员权限或者将密码重置为初始密码
	QSqlQuery q(db);
	QString str_sql;
	qDebug() << "激活操作";
	if (bhv)
	{
		str_sql = QString("update user set admin='管理员' where userid=%1")
			.arg(str_id);
	}
	else
	{
		str_sql = QString("update user set password='123456' where userid=%1")
			.arg(str_id);
	}
	
	if (!q.exec(str_sql))
	{
		qDebug() << q.lastError().text();
	}
}

void SqlMgr::set_userinfo(QString id, QString username, QString password)
{
	//实现修改账号密码
	QSqlQuery q(db);
	QString str_sql = QString("update user set username='%1', password='%2' where userid=%3")
		.arg(username)
		.arg(password)
		.arg(id);
	bool ret = q.exec(str_sql);
	if (!ret)
	{
		qDebug() << q.lastError().text();
	}
}

bool SqlMgr::dlt_user(QString str_id)
{
	//实现删除用户
	QSqlQuery q(db);
	QString str_sql = QString("delete from user where userid=%1")
		.arg(str_id);
	bool ret = q.exec(str_sql);
	if (!ret)
	{
		qDebug() << q.lastError().text();
	}
	return ret;
}

//
//图书管理部分
//
QVector<QStringList> SqlMgr::get_books(QString str_cond)
{
	//实现获取图书
	QSqlQuery q(db);
	QString str_sql;

	str_sql = QString("select * from book %1;").arg(str_cond);

	QVector<QStringList> vec;
	if (q.exec(str_sql))
	{
		int cols = q.record().count();
		QStringList l;
		while (q.next())
		{
			l.clear();
			for (int i = 0; i < cols; i++)
			{
				l << q.value(i).toString();
			}
			vec.push_back(l);
		}
	}
	else { qDebug() << q.lastError().text(); }
	return vec;
}

void SqlMgr::add_books(QVector<QStringList> v)
{
	//实现批量导入图书
	db.transaction();
	QSqlQuery q(db);
	for (auto it : v)
	{
		QString str_sql = QString("insert into book VALUES(NULL,'%1','%2','%3','%4','%5','%6');")
			.arg(it[0])
			.arg(it[1])
			.arg(it[2])
			.arg(it[3])
			.arg(it[4])
			.arg(it[5]);
		bool ret = q.exec(str_sql);
		if (!ret)
			qDebug() << q.lastError().text();
	}
	db.commit();
}

void SqlMgr::update_books(QStringList l,QString id)
{
	//实现更新图书信息
	QSqlQuery q(db);
	QString str_sql = QString("update book "
		                      "set bookname='%1',author='%2',type1='%3',type2='%4',access=%5 "
		                      "where bookid=%7 ;")
		.arg(l[0])
		.arg(l[1])
		.arg(l[2])
		.arg(l[3])
		.arg(l[4])
		.arg(id);

	bool ret = q.exec(str_sql);
	if (!ret)
		qDebug() << q.lastError().text();

}

bool SqlMgr::dlt_books(QString str_id)
{
	//实现删除图书
	QSqlQuery q(db);
	QString str_sql = QString("delete from book where bookid=%1")
		.arg(str_id);
	bool ret= q.exec(str_sql);
	if (!ret)
	{
		qDebug() << q.lastError().text();
		//最终可改为报错弹窗
		return false;
	}
	(!MsgBox(nullptr,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 删除成功！ < / div>",
		QIcon(":/x64/Debug/resource/pic/default.png"))).exec();
	return ret;
}

void SqlMgr::return_books(QString recordid)
{
	//实现申请归还操作：将当前选中的记录的借阅状态改为“申请归还”
	QSqlQuery q(db);
	QString str_sql = QString("UPDATE record SET request='申请归还' WHERE recordid =%1 ")
		.arg(recordid);
	bool ret = q.exec(str_sql);
	if (!ret)
	{
		qDebug() << q.lastError().text();
	}
}

void SqlMgr::borrow_books(QString userid, QString bookid,QString bookname)
{
	//实现申请借阅：新添一条当前用户申请借阅当前选中图书的借阅记录，借阅状态为“申请借阅”
	QSqlQuery q(db);
	QString str_sql = QString("insert into record VALUES(NULL,%1,%2,'%3','','','%4')")
		.arg(userid)
		.arg(bookid)
		.arg(bookname)
		.arg("申请借阅");
	bool ret = q.exec(str_sql);
	if (!ret)
	{
		qDebug() << q.lastError().text();
	}
}

void SqlMgr::verify_borrow(QString recordid, QString userid, QString bookid)
{
	//实现批准借阅：将当前选中的记录的借阅状态改为“已借未还”
	QSqlQuery q(db);
	QString str[3];
	QDateTime start = QDateTime::currentDateTime();
	QDateTime end = QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch() + 3600 * 24 * 90);

	str[0] = QString("UPDATE record SET start='%1', end='%2' ,request='%3'  WHERE recordid =%4 ")
		.arg(start.toString("yyyy-MM-dd"))
		.arg(end.toString("yyyy-MM-dd,23:59"))
		.arg("已借未还")
		.arg(recordid);

	//用户借阅总数加1，图书被借总数加1，图书在架数量减1
	str[1] = QString("UPDATE user SET brwcnt=brwcnt+1  WHERE userid =%1 ").arg(userid);
	str[2] = QString("UPDATE book SET cnt=cnt+1, access=access-1  WHERE bookid =%1").arg(bookid);

	for (int i = 0; i < 3; i++)
	{
		if (!q.exec(str[i]))
		{
			qDebug() << q.lastError().text();
		}
	}
}

void SqlMgr::verify_return(QString recordid, QString bookid)
{
	//实现批准归还：将当前选中的记录的借阅状态改为“已归还”
	QSqlQuery q(db);
	QString str[2];

	str[0] = QString("UPDATE record SET request='%1'  WHERE recordid =%2 ")
		.arg("已归还")
		.arg(recordid);

	//图书在架数量加1
	str[1] = QString("UPDATE book SET access=access+1  WHERE bookid =%1").arg(bookid);
	for (int i = 0; i < 2; i++)
	{
		if (!q.exec(str[i]))
		{
			qDebug() << q.lastError().text();
		}
	}
}

//
//借阅记录部分
//

QVector<QStringList> SqlMgr::get_records(QString str_cond)
{
	//获取借阅记录
	QSqlQuery q(db);
	QString str_sql;

	str_sql = QString("select * from record %1;").arg(str_cond);
	qDebug() << str_sql;

	QVector<QStringList> vec;
	if (q.exec(str_sql))
	{
		int cols = q.record().count();
		QStringList l;
		while (q.next())
		{
			l.clear();
			for (int i = 0; i < cols; i++)
			{
				l << q.value(i).toString();
			}
			vec.push_back(l);
		}
	}
	else { qDebug() << q.lastError().text(); }
	return vec;
}