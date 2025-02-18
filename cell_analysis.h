#pragma once

/////////////////////////////////////////////////////
//cell_analysis.h : 管理员主界面的统计分析页面

#include <QWidget>
#include "ui_cell_analysis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cell_AnalysisClass; };
QT_END_NAMESPACE

class Cell_Analysis : public QWidget
{
	Q_OBJECT

public:
	Cell_Analysis(QWidget *parent = nullptr);
	~Cell_Analysis();

	void init();              //初始化整个界面
	void init_label();        //初始化文字标签
	void init_chart();        //初始化图表
	void clear_container();   //在初始化图表之前清除之前存在的图表
private:
	Ui::Cell_AnalysisClass *ui;
};