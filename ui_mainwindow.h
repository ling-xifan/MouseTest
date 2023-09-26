/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_txt;
    QAction *action_excel;
    QAction *action_graph;
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_14;
    QLabel *label_2;
    QPushButton *pushStart;
    QPushButton *pushClear;
    QLineEdit *lineEdit_3;
    QListView *list_ch1;
    QLabel *label_4;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushStop;
    QLabel *label_13;
    QComboBox *comboVolt1;
    QFrame *line;
    QPushButton *pushVoltage;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLabel *label_12;
    QPushButton *pushVoltage_2;
    QLabel *label_7;
    QLineEdit *lineEdit_4;
    QPushButton *pushOpen;
    QPushButton *pushClose;
    QComboBox *comboVolt1_2;
    QLabel *label_temperature;
    QLabel *label_5;
    QPushButton *pushVoltage_3;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLabel *label_6;
    QLabel *label_8;
    QPushButton *pushVoltage_4;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1261, 665);
        action_txt = new QAction(MainWindow);
        action_txt->setObjectName(QString::fromUtf8("action_txt"));
        action_excel = new QAction(MainWindow);
        action_excel->setObjectName(QString::fromUtf8("action_excel"));
        action_graph = new QAction(MainWindow);
        action_graph->setObjectName(QString::fromUtf8("action_graph"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(320, 10, 71, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        label->setFont(font);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(100, 20, 41, 21));
        label_14->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(490, 10, 31, 21));
        label_2->setFont(font);
        pushStart = new QPushButton(centralWidget);
        pushStart->setObjectName(QString::fromUtf8("pushStart"));
        pushStart->setGeometry(QRect(1090, 300, 111, 61));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        pushStart->setFont(font1);
        pushClear = new QPushButton(centralWidget);
        pushClear->setObjectName(QString::fromUtf8("pushClear"));
        pushClear->setGeometry(QRect(1090, 490, 111, 61));
        pushClear->setFont(font1);
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(630, 10, 61, 21));
        lineEdit_3->setFont(font);
        list_ch1 = new QListView(centralWidget);
        list_ch1->setObjectName(QString::fromUtf8("list_ch1"));
        list_ch1->setGeometry(QRect(350, 60, 91, 31));
        QFont font2;
        font2.setPointSize(11);
        list_ch1->setFont(font2);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(550, 10, 71, 21));
        label_4->setFont(font);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(60, 110, 951, 481));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        pushStop = new QPushButton(centralWidget);
        pushStop->setObjectName(QString::fromUtf8("pushStop"));
        pushStop->setGeometry(QRect(1090, 390, 111, 61));
        pushStop->setFont(font1);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(60, 20, 41, 21));
        label_13->setFont(font);
        comboVolt1 = new QComboBox(centralWidget);
        comboVolt1->addItem(QString());
        comboVolt1->addItem(QString());
        comboVolt1->addItem(QString());
        comboVolt1->addItem(QString());
        comboVolt1->addItem(QString());
        comboVolt1->addItem(QString());
        comboVolt1->setObjectName(QString::fromUtf8("comboVolt1"));
        comboVolt1->setGeometry(QRect(60, 50, 81, 31));
        comboVolt1->setFont(font);
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(1040, 0, 20, 601));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        pushVoltage = new QPushButton(centralWidget);
        pushVoltage->setObjectName(QString::fromUtf8("pushVoltage"));
        pushVoltage->setGeometry(QRect(160, 50, 51, 31));
        pushVoltage->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(710, 10, 71, 21));
        label_3->setFont(font);
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(790, 10, 61, 20));
        lineEdit_2->setFont(font);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(410, 10, 61, 21));
        lineEdit->setFont(font);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(860, 10, 21, 21));
        label_12->setFont(font);
        pushVoltage_2 = new QPushButton(centralWidget);
        pushVoltage_2->setObjectName(QString::fromUtf8("pushVoltage_2"));
        pushVoltage_2->setGeometry(QRect(620, 60, 51, 31));
        pushVoltage_2->setFont(font);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(230, 60, 111, 31));
        label_7->setFont(font);
        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(550, 60, 61, 31));
        lineEdit_4->setFont(font);
        pushOpen = new QPushButton(centralWidget);
        pushOpen->setObjectName(QString::fromUtf8("pushOpen"));
        pushOpen->setGeometry(QRect(1090, 130, 111, 61));
        pushOpen->setFont(font1);
        pushClose = new QPushButton(centralWidget);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));
        pushClose->setGeometry(QRect(1090, 210, 111, 61));
        pushClose->setFont(font1);
        comboVolt1_2 = new QComboBox(centralWidget);
        comboVolt1_2->addItem(QString());
        comboVolt1_2->setObjectName(QString::fromUtf8("comboVolt1_2"));
        comboVolt1_2->setGeometry(QRect(1060, 60, 111, 31));
        comboVolt1_2->setFont(font);
        label_temperature = new QLabel(centralWidget);
        label_temperature->setObjectName(QString::fromUtf8("label_temperature"));
        label_temperature->setGeometry(QRect(1190, 60, 71, 31));
        label_temperature->setFont(font);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(470, 60, 71, 31));
        label_5->setFont(font);
        pushVoltage_3 = new QPushButton(centralWidget);
        pushVoltage_3->setObjectName(QString::fromUtf8("pushVoltage_3"));
        pushVoltage_3->setGeometry(QRect(900, 0, 51, 31));
        pushVoltage_3->setFont(font);
        lineEdit_5 = new QLineEdit(centralWidget);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(750, 60, 61, 31));
        lineEdit_5->setFont(font);
        lineEdit_6 = new QLineEdit(centralWidget);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(870, 60, 61, 31));
        lineEdit_6->setFont(font);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(700, 60, 41, 31));
        label_6->setFont(font);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(830, 60, 31, 31));
        label_8->setFont(font);
        pushVoltage_4 = new QPushButton(centralWidget);
        pushVoltage_4->setObjectName(QString::fromUtf8("pushVoltage_4"));
        pushVoltage_4->setGeometry(QRect(940, 60, 51, 31));
        pushVoltage_4->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1261, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(action_txt);
        menu->addAction(action_excel);
        menu->addAction(action_graph);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_txt->setText(QCoreApplication::translate("MainWindow", "save as txt", nullptr));
        action_excel->setText(QCoreApplication::translate("MainWindow", "save as excel", nullptr));
        action_graph->setText(QCoreApplication::translate("MainWindow", "save as graph", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\345\260\272\345\272\246", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\357\274\210V\357\274\211", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "s", nullptr));
        pushStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        pushClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\225\260\346\215\256", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "2000", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Y\346\234\200\345\244\247\345\200\274", nullptr));
        pushStop->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\351\207\207\351\233\206", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\345\212\240\347\203\255", nullptr));
        comboVolt1->setItemText(0, QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251", nullptr));
        comboVolt1->setItemText(1, QCoreApplication::translate("MainWindow", "1", nullptr));
        comboVolt1->setItemText(2, QCoreApplication::translate("MainWindow", "2", nullptr));
        comboVolt1->setItemText(3, QCoreApplication::translate("MainWindow", "3", nullptr));
        comboVolt1->setItemText(4, QCoreApplication::translate("MainWindow", "4", nullptr));
        comboVolt1->setItemText(5, QCoreApplication::translate("MainWindow", "5", nullptr));

        pushVoltage->setText(QCoreApplication::translate("MainWindow", "\347\241\256\350\256\244", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Y\346\234\200\345\260\217\345\200\274", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "150", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "k\316\251", nullptr));
        pushVoltage_2->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\344\274\240\346\204\237\345\231\250\347\224\265\351\230\273(K\316\251)", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("MainWindow", "20", nullptr));
        pushOpen->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        pushClose->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\344\270\262\345\217\243", nullptr));
        comboVolt1_2->setItemText(0, QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251", nullptr));

        label_temperature->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\347\224\265\346\265\201(uA)", nullptr));
        pushVoltage_3->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Max", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Min", nullptr));
        pushVoltage_4->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
