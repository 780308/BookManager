#include "dlg_login.h"
#include "Page_user.h"
#include "page_admin.h"
#include "sqlmgr.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建并初始化数据库类的唯一对象
    SqlMgr::getinstance()->init();

    Dlg_Login dlg;
    Page_user user_page;
    Page_Admin admin_page;

    //用户登陆成功之后传输用户名，调用初始化页面函数
    QObject::connect(&dlg, &Dlg_Login::log_success, &user_page, &Page_user::set_username);
    QObject::connect(&dlg, &Dlg_Login::log_success, &user_page, &Page_user::initpage);

    //连接登陆成功信号和显示对应主界面槽函数
    QObject::connect(&dlg, &Dlg_Login::user_lgn, [&]() {
        user_page.show();
        });

    QObject::connect(&dlg, &Dlg_Login::admin_lgn, [&]() {
        admin_page.show();
        });

    //连接切换账号信号和返回登录对话槽函数
    QObject::connect(&user_page, &Page_user::switch_acnt, [&]() {
        dlg.show();
        });

    QObject::connect(&admin_page, &Page_Admin::switch_acnt, [&]() {
        dlg.show();
        });

    dlg.show();

    return a.exec();
}