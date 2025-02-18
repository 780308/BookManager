#pragma once

/////////////////////////////////////////////////////
//sqlmgr.h : SQlite数据库处理类

#include <QSqlDataBase>

class SqlMgr
{
public:
	SqlMgr();
	~SqlMgr();

	//采用单例模式，全局只创建唯一对象
	static SqlMgr* instance;       
	static SqlMgr* getinstance();

	//初始化
	void init();

	//登录逻辑实现
	bool login(QString str_name, QString str_pwd, int admin);

	//获取用户
	QVector<QStringList> get_users(QString str_cond = "");

	//单个添加用户
	void add_user(QString str_name, QString& str_admin);

	//批量导入用户
	void add_user(QVector<QStringList>);

	//激活用户权限
	void update_users(QString str_id="1",int bhv=0);

	//修改用户账号密码
	void set_userinfo(QString id,QString username,QString password);

	//删除用户
	bool dlt_user(QString str_id);

	//获取图书
	QVector<QStringList> get_books(QString str_cond = "");

	//添加图书
	void add_books(QVector<QStringList>);

	//修改图书信息
	void update_books(QStringList, QString);

	//删除图书
	bool dlt_books(QString str_id);

	//归还图书
	void return_books(QString recordid);

	//申请借阅图书
	void borrow_books(QString userid, QString bookid,QString bookname);

	//批准借阅
	void verify_borrow(QString recordid,QString userid, QString bookid);

	//审核归还
	void verify_return(QString recordid, QString bookid);

	//获取借阅记录
	QVector<QStringList> get_records(QString str_cond = "");

private:
	//数据库对象
	QSqlDatabase db;
};