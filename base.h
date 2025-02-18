#pragma once

/////////////////////////////////////////////////////
//base.h : 继承自QMainWindow类的抽象类，用于派生主界面类

#include <QMainWindow>

class Base : public QMainWindow
{
    Q_OBJECT

public:
    Base(QWidget* parent = nullptr);
    virtual ~Base();

    virtual void initpage()=0;
    virtual void pop_message() = 0;    //将初始化界面和登录提示设置为纯虚函数

    void closeEvent(QCloseEvent* e);   //鼠标点击关闭事件的对话

signals:
    void switch_acnt();                //将鼠标点击切换账号设置成信号
     
public slots:
    void on_btn_switch_clicked();      //切换账号按钮功能实现
    void on_btn_exit_clicked();        //退出登录按钮功能实现
    //抽象类不需要添加ui界面
};