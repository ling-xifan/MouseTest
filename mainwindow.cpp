#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

using namespace std;

//设置socket信息
//static int port = 8090;
//static char* ServerIP = "192.168.4.1";
QString currentCOM = "";
//192.168.4.1:8090
static int time_init = 0, time_end = 500;
static int range_min = 0, range_max = 100;
static int Started = 0;                         //标志是否已开始采集
static double time_range;
static QString save_fileName;                   //文件保存名
static vector<vector<float>> array_signal;          //二维数组
static vector<vector<float>> array_signal_save;      //二维数组
static vector<float> array_time;                    //时间数组
static vector<float> array_time_save;                //时间数组
static vector<float> array_cnt_save;                  //计数数组
static QByteArray buffer;
static int index = -1;
static int cnt = -1;
static int index_save = 0;
static QString volt = "0000";
int dataToSend[3] = {0, 0, 0};  // 要发送的3个int类型数据
static char warn = '0';
static int up = 0;
static int warnres = 20;
static int upflag = 0,downflag = 0,sendflag = 0;
static int dp1=1,dp2=1,dp3=1,dp4=1,dp5=1,dp6=1;
static QListView* RT_list[ARRAY];
static QStringList text_content[ARRAY];//记录文本的变量
static QStringListModel *model_text[ARRAY];
static QColor Graph_color[ARRAY] = {
                         QColor(47, 79, 79),//DarkSlateGray

                       };

//static QTimer *timer_0;//test
static QTimer *timer;
static thread* array_graph[THREAD_NUM];
static QFile file;

//线程同步所需的信号量
static std::mutex bMutex;//互斥锁，防止添加数据或显示数据的时候被打断
static int reading = 0;

/************************************************画图线程函数**********************************************/
//每个线程画step条曲线
void MainWindow::draw_thread(int start, int end) {
qDebug() << "begin drawing"<<endl;
    while(reading) qDebug() << "Reading"<<endl;//等待读结束
qDebug() << "begin drawing1"<<endl;
    for(int num = start; num <= end; num++) {
        series[num]->setPen(QPen(Graph_color[num], 1, Qt::SolidLine));  // 设置折线显示效果

        if(dp1 == 0)  series[0]->setPen(QColor(0,0,0,0));

//        qDebug() << "dp: " << array_signal[num][0] << endl;

        for (int i = 0; i <= time_end; i++)
            series[num]->append(-i, array_signal[num][i]/1000);

        //设置listview
        QString s = QString::number(round(array_signal[num][0]/1000));
        text_content[num].clear();
        text_content[num] << s;
    }

    qDebug() << "drawing" << array_signal[0][0] << endl;
    return;
}

void idle_thread() {
    //空线程函数
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sys_init();
    //定时扫描和更新串口
    timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::showValidPort);//实时更新端口号
    timer->start(1000); //每1000ms定时检测串口状态

    JSTimer = new QTimer;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //扫描可用串口
    portStringList += info.portName();
    ui->comboVolt1_2->addItems(portStringList);


    array_signal.resize(ARRAY, vector<float>(BUFFERSIZE));
    array_signal_save.resize(ARRAY, vector<float>(120));
    array_time.resize(BUFFERSIZE);
    array_time_save.resize(120);
    array_cnt_save.resize(BUFFERSIZE);
        RT_list[0] = ui->list_ch1;
//        RT_list[1] = ui->list_ch2;
//        RT_list[2] = ui->list_ch3;
//        RT_list[3] = ui->list_ch4;
//        RT_list[4] = ui->list_ch5;
//        RT_list[5] = ui->list_ch6;

    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    for (int i = 0; i < ARRAY; i++)     //这里并不是很懂是什么
        model_text[i] = new QStringListModel(text_content[i]);
    for (int i = 0; i < ARRAY; i++)
        series[i] = new QLineSeries;

    /*****************设置标题和图标***********************/
    setWindowTitle(u8"单通道TO电子鼻小鼠测试");

    /*****************设置计时器***********************/
   // ui->lcdNumber->display("00:00");
    JSTimer = new QTimer;

    /**********************连接槽函数************************/
//    connect(JSTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
    connect(ui->pushOpen,&QPushButton::clicked,this,&MainWindow::my_btn_open_clicked);
    connect(ui->pushClose,&QPushButton::clicked,this,&MainWindow::my_btn_close_clicked);
    connect(ui->pushStart,&QPushButton::clicked,this,&MainWindow::my_btn_begin_clicked);
    connect(ui->pushStop,&QPushButton::clicked,this,&MainWindow::my_btn_stop_clicked);
    connect(ui->pushClear,&QPushButton::clicked,this,&MainWindow::Clear_data);
   connect(&global_port,&QSerialPort::readyRead,this,&MainWindow::SI_ARDreadData);
    connect(ui->action_txt, SIGNAL(triggered()), this, SLOT(save_txt()));
//    connect(ui->action_excel, SIGNAL(triggered()), this, SLOT(save_excel()));
//    connect(ui->action_graph, SIGNAL(triggered()), this, SLOT(save_graph()));
   // connect(ui->comboCHchoose, SIGNAL(currentIndexChanged(QString)), this, SLOT(Series_display()));

//    connect(ui->comboVolt1, SIGNAL(currentIndexChanged(QString)), this, SLOT(Volt_Change()));
    connect(ui->pushVoltage, SIGNAL(clicked()), this, SLOT(Volt_Send()));
    connect(ui->pushVoltage_2, SIGNAL(clicked()), this, SLOT(Current_Send()));
    connect(ui->pushVoltage_3, SIGNAL(clicked()), this, SLOT(Set_XY_Range()));
    connect(ui->pushVoltage_4, SIGNAL(clicked()), this, SLOT(parameters_send()));
    connect(ui->pushVoltage_5, SIGNAL(clicked()), this, SLOT(Freq_send()));
    connect(ui->pushVoltage_6, SIGNAL(clicked()), this, SLOT(timer_up_send()));
//    connect(ui->pushVoltage_2, SIGNAL(clicked()), this, SLOT(Send_Warn()));


    /***********************画图*****************************/
    drawing = new QChart;
    drawing->setTitle(u8"阵列数据曲线");
    drawview = new QChartView(drawing);
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    //建立数据源队列

    //建立坐标轴
    QBrush AxisColor;
    AxisColor.setColor(Qt::black);
    axisX->setRange(-500, 0);                // 设置范围
    axisX->setLabelFormat("%d");                            // 设置刻度的格式
    axisX->setGridLineVisible(true);                        // 网格线可见
    axisX->setTickCount(6);                                 // 设置多少个大格
    axisX->setMinorTickCount(1);                            // 设置每个大格里面小刻度线的数目
    axisX->setTitleText(u8"时间");                             // 设置描述

    axisY->setRange(range_min, range_max);
    axisY->setLabelFormat("%.1f");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(8);
    axisY->setMinorTickCount(1);
    axisY->setTitleText(u8"数据");
    //为曲线添加坐标轴
    drawing->addAxis(axisX, Qt::AlignBottom);               // 下：Qt::AlignBottom  上：Qt::AlignTop
    drawing->addAxis(axisY, Qt::AlignLeft);                 // 左：Qt::AlignLeft    右：Qt::AlignRight
    drawing->legend()->hide();                              //隐藏图例
    //chart放入chartview内
    drawview->setRenderHint(QPainter::Antialiasing);        //防止图形走样
    ui->verticalLayout_5->addWidget(drawview);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// 重写closeEvent: 确认退出对话框
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {
        event->accept(); // 接受退出信号，程序退出
    }
}
/*************功能：串口初始化**********************/
void MainWindow::sys_init()
{
    // port config
    global_port.setBaudRate(QSerialPort::Baud115200);  //波特率
    global_port.setParity(QSerialPort::NoParity)  ;   //无奇偶校验
    global_port.setDataBits(QSerialPort::Data8);    //8位数据位
    global_port.setStopBits(QSerialPort::OneStop);  // 1位停止位
}
/*******************************************槽函数************************************************/
/*************功能：打开串口**********************/
void MainWindow::my_btn_open_clicked()
{
    /*-------------port_name-----------------*/
    if(global_port.isOpen())
    {
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Serial port has opened!"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }
    else
    {
       // qDebug() <<ui->comboVolt1->currentText();
        global_port.setPortName(ui->comboVolt1_2->currentText());//设置端口号
        if(global_port.open(QIODevice::ReadWrite))//打开串口函数，会返回一个bool值判断是否打开
        {
            QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("message"),QString("Open!"));//弹出提示
            QTimer::singleShot(500,m_box,SLOT(accept()));//定时500ms显示后，关闭弹窗
            m_box->exec();
        }else
        {
            QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Failed to open the serial port"));
            QTimer::singleShot(500,m_box,SLOT(accept()));
            m_box->exec();
        }
    }


}
/*************功能：关闭串口**********************/
void MainWindow::my_btn_close_clicked()
{
    if(Started == 0)
    {
     //   qDebug() << "close";
        global_port.close();
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("Message"),QString("Closed"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }
   else
    {
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Collecting!"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }

}
/*************功能：开始采集**********************/
void MainWindow::my_btn_begin_clicked()
{
    if(global_port.isOpen())
    {

        if(Started == 0)
        {

            QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("Success"),QString("START"));
            QTimer::singleShot(500,m_box,SLOT(accept()));
            m_box->exec();
            Started = 1;
            //设置采集定时器
          //  qDebug() <<"sart:";
            timer1 = new QTimer;
            timer1->start(TIME_CYC);//TIME_CYCms采集一次
          //  qDebug() <<"sart:";
            for (int j = 0; j < THREAD_NUM; j++)//THREAD_NUM采集线程数
                 array_graph[j] = new std::thread(idle_thread);//创建空闲线程
            for (int j = 0; j < THREAD_NUM; j++)
                 array_graph[j]->join();
            connect(timer1, SIGNAL(timeout()), this, SLOT(Draw_Graph()));//连接定时器

            //获取当前时间给计时器
            JSTime = JSTime.currentTime();
            JSTimer->start(100);

        }
        else
        {
            QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Collecting!"));
            QTimer::singleShot(1000,m_box,SLOT(accept()));
            m_box->exec();
        }

    }
    else
    {
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("please open the port"));
        QTimer::singleShot(1000,m_box,SLOT(accept()));
        m_box->exec();
    }
}
/*************功能：停止采集**********************/
void MainWindow::my_btn_stop_clicked()
{
    if(global_port.isOpen())
    {
    if(Started == 1)
    {
        Started = 0;
        for (int j = 0; j < THREAD_NUM; j++)
        {
            delete array_graph[j];//删除线程
        }
        timer1->stop();
        delete timer1;//删除采集线程
    JSTimer->stop();
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("Message"),QString("STOP"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }
    else
    {
//        QMessageBox::warning(this, "error", u8"没有开始采集！");
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Did not start Collecting!"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }
    }
    else
    {
        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("error"),QString("Did not open the seriport"));
        QTimer::singleShot(500,m_box,SLOT(accept()));
        m_box->exec();
    }
}

/*************功能：串口接收数据**********************/
void MainWindow::SI_ARDreadData()
{
    reading = 1;
//    //30ms延时(保证一轮数据发送完全)
//    QEventLoop loop;
//    QTimer::singleShot(30, &loop, SLOT(quit()));
//    loop.exec();

    //接收数据
    qDebug()<<"000";
    QByteArray SIard_data = global_port.readAll();
   qDebug()<<"rece_ARD:"<<SIard_data;
    qDebug()<<"001";

    //因为会多返回一次空信息，所以这里要设置条件
    if(SIard_data==""){}
    else
    {
//        qDebug()<<"002";
        //数据点计数
        cnt++;
        array_cnt_save.insert(array_cnt_save.begin(), cnt);
        if(index < BUFFERSIZE) index++;  //用来判断数据是否超过边界

        //数据类型转换及截取
        QByteArray SIard_cut("\r");
        QByteArray SIard_cutdata;
        for(int i = 0; i < ARRAY; i++)
        {
            if (index >= BUFFERSIZE)  array_signal.at(i).pop_back();
            int cutcnt = SIard_data.indexOf(SIard_cut);
//            qDebug()<<"position:"<<cutcnt;
            SIard_cutdata = SIard_data.left(cutcnt);
            qDebug()<<"rece_ARD1:"<<SIard_cutdata;
            array_signal.at(i).insert(array_signal.at(i).begin(), SIard_cutdata.toFloat());
            array_signal_save.at(i).insert(array_signal_save.at(i).begin(), SIard_cutdata.toFloat());
            SIard_data = SIard_data.remove(0, cutcnt+1);
        }


    }
    reading = 0;
}
/*************功能：清空数据区和曲线******************/
//只有在不采集的情况下才能清空
//不仅清空显示，缓存里已有的数据也会清空

void MainWindow::Clear_data()
{
    if (Started) {
        //do nothing
    }
    else {
        for (int i = 0; i < ARRAY; i++)
        {
            text_content[i].clear();
            series[i]->clear();
        }
        index = -1;
        cnt = -1;
        array_signal.clear();
        array_time.clear();
        array_cnt_save.clear();

        array_time.resize(BUFFERSIZE);
        array_cnt_save.resize(BUFFERSIZE);
        array_signal.resize(ARRAY, vector<float>(BUFFERSIZE));

//        //清除计时器
//        ui->lcdNumber->display("00:00");
    }
}

/*************功能：显示可用串口**********************/
void MainWindow::showValidPort()
{
    QStringList newPortStringList;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        newPortStringList += info.portName();
    if(newPortStringList.size() != portStringList.size())
    {
        portStringList = newPortStringList;
        ui->comboVolt1_2->clear();
        ui->comboVolt1_2->addItems(portStringList);
    }
    if(currentCOM != ui->comboVolt1_2->currentText()) //串口突然断开连接了
    {
        currentCOM = ui->comboVolt1_2->currentText();

    }
}
/*************功能：数据保存成txt*********************/
void MainWindow::save_txt()
{
    QString save_fileName = QFileDialog::getSaveFileName(this, "Save",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "text files(*.txt)");

    //对文件进行操作
    QFile file(save_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox::warning(this, "Error", u8"打开失败", QMessageBox::Yes);
        return;
    }
    QTextStream in(&file);

    for (int i = index - 1; i >= 0; i--) //越往上越靠近初始
    {
        in << array_cnt_save[i];
        for(int j = 0; j < ARRAY; j++)
        {
            in << "	" << array_signal[j][i];
        }
        in << "\n";
    }

    file.close();
}



/*************功能：画图（定时器）并显示实时数据******/
/*开六个线程画*/
void MainWindow::Draw_Graph()
{
    int j = 0;
    qDebug() << "draw" << endl;//debug
    for (j = 0; j < THREAD_NUM; j++)
        delete array_graph[j];
    for (j = 0; j < ARRAY; j++) {
        delete series[j];
        series[j] = new QLineSeries;
        delete model_text[j];
    }

    //目前只开2个线程，共6条曲线
    for (j = 0; j < THREAD_NUM; j++) {
        array_graph[j] = new std::thread(&MainWindow::draw_thread, this, j*ARRAY/THREAD_NUM, (j+1)*ARRAY / THREAD_NUM - 1);//创建画图线程
    }

    for (j = 0; j < THREAD_NUM; j++)
        array_graph[j]->join();
    for (j = 0; j < ARRAY; j++) {
        drawing->addSeries(series[j]);
        drawing->setAxisX(axisX, series[j]);
        drawing->setAxisY(axisY, series[j]);

        model_text[j] = new QStringListModel(text_content[j]);
        model_text[j]->setStringList(text_content[j]);
        RT_list[j]->setModel(model_text[j]);
    }
    qDebug() << (index + 1) << endl;
}



/*************功能：发送控温电压**********************/
void MainWindow::Volt_Send()
{
    dataToSend[0] = 1;
    dataToSend[1] = ui->comboVolt1->currentIndex();
    dataToSend[2] = 0;
    qDebug() << dataToSend[1] << endl;
    QByteArray sendData;
    QDataStream stream(&sendData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (int i = 0; i < 3; ++i) {
       stream <<  dataToSend[i];  // 将整数数据以文本形式写入字节数组
    }

    qint64 bytesWritten = global_port.write(sendData);  // 发送数据
    if (bytesWritten == -1) {
        // 发送失败，处理错误
          QMessageBox::warning(this, "失败",  QString::number(dataToSend[1]));
        qDebug() << "error" << endl;
    } else {
         qDebug() << "success" << endl;
          QMessageBox::warning(this, "成功",  QString::number(dataToSend[1]));
    }
  //  global_port.write(volt.toLatin1());        //发电压

}

/*************功能：发送刺激电流**********************/
void MainWindow::Current_Send()
{
    dataToSend[0] = 0;
    dataToSend[1] = ui->lineEdit_4->text().toInt();
    dataToSend[2] = 0;
    qDebug() << dataToSend[1] << endl;
    QByteArray sendData;
    QDataStream stream(&sendData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (int i = 0; i < 3; ++i) {
       stream <<  dataToSend[i];  // 将整数数据以文本形式写入字节数组
    }

    qint64 bytesWritten = global_port.write(sendData);  // 发送数据
    if (bytesWritten == -1) {
        // 发送失败，处理错误
         QMessageBox::warning(this, "失败",  QString::number(dataToSend[1]));
        qDebug() << "error" << endl;
    } else {
         qDebug() << "success" << endl;
          QMessageBox::warning(this, "成功",  QString::number(dataToSend[1]));
    }
  //  global_port.write(volt.toLatin1());        //发电压

}

/*************功能：发送响应界限**********************/
void MainWindow::parameters_send()
{
    dataToSend[0] = 2;//模式2
    dataToSend[1] = ui->lineEdit_5->text().toInt();//参数1
    dataToSend[2] = ui->lineEdit_6->text().toInt();//参数2
    qDebug() << dataToSend[1] << endl;


        QByteArray sendData;
        QDataStream stream(&sendData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        for (int i = 0; i < 3; ++i) {
           stream <<  dataToSend[i];  // 将整数数据以文本形式写入字节数组
        }
        qint64 bytesWritten = global_port.write(sendData);  // 发送数据
        if (bytesWritten == -1) {
            // 发送失败，处理错误
            QMessageBox::warning(this, "失败",  QString::number(dataToSend[1]));
            qDebug() << "error" << endl;
        } else {
             qDebug() << "success" << endl;
             QMessageBox::warning(this, "成功",  QString::number(dataToSend[1]));
        }

}

/*************功能：发送频率**********************/
void MainWindow::Freq_send()
{
    dataToSend[0] = 3;//模式3
    dataToSend[1] = ui->lineEdit_7->text().toInt();//频率设置
    dataToSend[2] = 0;//参数2
    qDebug() << dataToSend[1] << endl;

        if(dataToSend[1] >=0 &&dataToSend[0]>=0)
        {
            QByteArray sendData;
            QDataStream stream(&sendData, QIODevice::WriteOnly);
            stream.setByteOrder(QDataStream::LittleEndian);
            for (int i = 0; i < 3; ++i) {
               stream <<  dataToSend[i];  // 将整数数据以文本形式写入字节数组
            }
            qint64 bytesWritten = global_port.write(sendData);  // 发送数据
            if (bytesWritten == -1) {
                // 发送失败，处理错误
                 QMessageBox::warning(this, "失败",  QString::number(dataToSend[1]));
                qDebug() << "error" << endl;
            } else {
                 qDebug() << "success" << endl;
                  QMessageBox::warning(this, "成功",  QString::number(dataToSend[1]));
            }

        }

}

/*************功能：发送电流持续时间 us**********************/
void MainWindow::timer_up_send()
{
    dataToSend[0] = 4;//模式4
    dataToSend[1] = ui->lineEdit_8->text().toInt();//频率设置
    dataToSend[2] = 0;//参数2
    qDebug() << dataToSend[1] << endl;

        if(dataToSend[1]>=5)
        {
        QByteArray sendData;
        QDataStream stream(&sendData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        for (int i = 0; i < 3; ++i) {
           stream <<  dataToSend[i];  // 将整数数据以文本形式写入字节数组
        }
        qint64 bytesWritten = global_port.write(sendData);  // 发送数据
        if (bytesWritten == -1) {
            // 发送失败，处理错误
            QMessageBox::warning(this, "失败",  QString::number(dataToSend[1]));
            qDebug() << "error" << endl;
        } else {
             QMessageBox::warning(this, "成功",  QString::number(dataToSend[1]));
             qDebug() << "success" << endl;
        }

        }
        else{
        QMessageBox::warning(this, "错误",  QString::number(dataToSend[1]));
        }
}
/*************功能：坐标轴手动设置**********************/
void MainWindow::Set_XY_Range()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        time_end = ui->lineEdit->text().toInt();
        axisX->setRange(-time_end, 0);
    }
    if(!ui->lineEdit_2->text().isEmpty() && !ui->lineEdit_2->text().isEmpty())
    {
        range_min = ui->lineEdit_2->text().toInt();
        range_max = ui->lineEdit_3->text().toInt();
        if(range_max>range_min)
        axisY->setRange(range_min, range_max);
    }
}





