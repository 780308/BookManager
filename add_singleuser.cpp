#include "add_singleuser.h"
#include "msgbox.h"
#include "sqlmgr.h"

Add_SingleUser::Add_SingleUser(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Add_SingleUserClass())
{
	ui->setupUi(this);
	setWindowIcon(QIcon(":/x64/Debug/resource/pic/add.png"));
	  //设置窗口图标
}

Add_SingleUser::~Add_SingleUser()
{
	delete ui;
}

void Add_SingleUser::on_btn_add_clicked()
{
	MsgBox msgbox = MsgBox(nullptr,
		"< div style = 'text-align: center; font-weight: bold; font-size: 20px;' > 确认添加？ < / div>",
		QIcon(":/x64/Debug/resource/pic/question.png"));
	   //确认操作对话

	int ret = msgbox.exec();

	if (ret == QMessageBox::Yes)
	{
		int idx = ui->comboBox->currentIndex();
		QString str_admin = idx ? "管理员" : "";
		QString str_name = ui->le_name->text();

		SqlMgr::getinstance()->add_user(str_name, str_admin);
		//新添一条用户名为文本框le_name中输入的文本，身份为str_admin的用户数据
	}
	this->close();
	//完成添加操作后关闭窗口
}

void Add_SingleUser::on_btn_cancel_clicked()
{
	this->close();
}