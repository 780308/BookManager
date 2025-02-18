#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchart.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QDebug>
#include <qrandom.h>
#include "sqlmgr.h"
#include "cell_analysis.h"

Cell_Analysis::Cell_Analysis(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Cell_AnalysisClass())
{
	ui->setupUi(this);
}

Cell_Analysis::~Cell_Analysis()
{
	delete ui;
}

void Cell_Analysis::init()
{
    init_label();
    init_chart();
}

void Cell_Analysis::init_label()
{
    //在数据库中查询借阅总数最多的用户和被借总数最多的图书，并在页面对应控件上各随机展示一个
    QVector<QStringList> l = SqlMgr::getinstance()->get_users("where brwcnt = (select MAX(brwcnt) from user)");
    if (l.isEmpty())
        return;
    int rdm_useridx = QRandomGenerator::global()->bounded(l.size());
    QStringList user = l[rdm_useridx];
    qDebug() << user;

    QVector<QStringList> b = SqlMgr::getinstance()->get_books("where cnt = (select MAX(cnt) from book)");
    if (b.isEmpty())
        return;
    int rdm_bkidx = QRandomGenerator::global()->bounded(b.size());
    QStringList book = b[rdm_bkidx];
    qDebug() << book;

    ui->lb_bookname->setText(book[1]);
    ui->lb_username->setText(user[1]);
}

void Cell_Analysis::init_chart()
{
    //初始化图表之前首先清空显示控件中已存在的其他内容
    clear_container();

    //获取所有具有起始借阅时间的记录
    QVector<QStringList> l = SqlMgr::getinstance()->get_records();
    QStringList brw_time;
    for (int i = 0; i < l.size(); i++)
    {
        QString item = l[i][4];
        if (QString("") != item)
            brw_time << item;
    }
    qDebug() << brw_time;

    QDate baseDate = QDate::fromString("2024-06-11", "yyyy-MM-dd"); // 基准日期

    //使用QMap统计每个日期的借阅记录数量
    QMap<QString, int> dateCounts; 

    // 使用天数差作为索引
    for (int i = 0; i < brw_time.size(); i++) {
        dateCounts[brw_time[i]]++;
    }

    qDebug() << dateCounts;

    // 创建折线图数据
    QLineSeries* lineSeries = new QLineSeries();

    // 设置最小和最大日期
    QDateTime minDate = QDateTime::fromString("2024-06-11", "yyyy-MM-dd");
    QDateTime maxDate = QDateTime::currentDateTime();

    //获取范围内的所有日期
    QDate startDate = minDate.date();
    QDate endDate = maxDate.date();
    for (QDate date = startDate; date <= endDate; date = date.addDays(1)) {
        QString dateStr = date.toString("yyyy-MM-dd");
        int borrowCount = dateCounts.value(dateStr, 0);
        QDateTime dateTime = QDateTime::fromString(dateStr, "yyyy-MM-dd");
        qint64 timestamp = dateTime.toMSecsSinceEpoch();
        lineSeries->append(timestamp, borrowCount);
    }
    qDebug() << dateCounts;

    lineSeries->setPointsVisible(true);

    //创建图表对象并将数据添加到图表
    QChart* chart = new QChart();
    chart->setTitle("Borrowing Count Over Time");
    chart->addSeries(lineSeries);

    //设置轴
    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("Borrow Count");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);

    //创建一个ChartView来显示图表
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //创建一个布局并添加ChartView到chartContainer中
    QLayout* existingLayout = ui->container->layout();
    if (existingLayout) {
        existingLayout->addWidget(chartView);
    }
    else {
        // 如果没有布局，创建一个新的布局并设置
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(chartView);
        ui->container->setLayout(layout);
    }
}

void Cell_Analysis::clear_container()
{
    if (ui->container->layout() != nullptr) {
        QLayoutItem* item = ui->container->layout()->takeAt(0);
        if (item != nullptr) {
            QWidget* widget = item->widget();
            if (widget != nullptr) {
                delete widget; // 删除该widget
            }
            delete item; // 删除布局项
        }
    }
}