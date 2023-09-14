#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
#include <QtNetwork/QTcpServer>                 //监听套接字
#include <QtNetwork/QTcpSocket>                 //通信套接字//对方的(客户端的)套接字(通信套接字)
#include <vector>
#include <QStringListModel>
#include <QStringList>

#include <QDebug>
#include <thread>
#include <mutex>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSerialPort>
#include <QtDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
//#include <QReadWriteLock>

#define TIME_CYC 500
#define TIME_SEND 1000                   //发送端发送时间间隔（单位：ms）。、、
#define TIME_1S 1000/TIME_SEND          //每 TIME_CYC 毫秒采集一次
#define MINUTE_60S 60
#define BUFFERSIZE 60*MINUTE_60S*12*10//队列寸30分钟内的数据
#define	ARRAY 1//阵列信号数
#define SERIES_NUM ARRAY
#define THREAD_NUM 1//线程数

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChart *drawing;
    QLineSeries* series [SERIES_NUM];


private:
    Ui::MainWindow *ui;
     QTimer *timer1;
      QTimer *timer;
    QStringList portStringList; //保存当前可用的串口号
    /*-------variable------*/
    QSerialPort global_port;
    /*-------funtion------*/
    void sys_init();
    //图像
    QChartView *drawview;
    QTcpSocket* tcpClient;
    void draw_thread(int start, int end);
    //计时器
    QTimer *JSTimer;
    QTime JSTime;
    QString timeStr;

private slots:
//    void updateDisplay();
//    void Func_start();
//    void Func_stop();
    void Clear_data();
    void Draw_Graph();
//    void save_excel();
    void save_txt();
//    void save_txt_1s();
   // void save_graph();
  //  void storeMessage();
    void showValidPort();
    void my_btn_open_clicked();
    void my_btn_close_clicked();
    void my_btn_begin_clicked();
    void my_btn_stop_clicked();
     void closeEvent(QCloseEvent *event);    // 重写closeEvent的申明
//    void Series_display();
   // void Volt_Change();
    //void Volt_Send();
    //void Send_Warn();
    void SI_ARDreadData();
    //加热
//    void Volt_Change();
    void Volt_Send();
    void Current_Send();
    void Set_XY_Range();
    void Response_Send();
};

#endif // MAINWINDOW_H
