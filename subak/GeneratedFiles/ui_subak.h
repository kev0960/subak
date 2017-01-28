/********************************************************************************
** Form generated from reading UI file 'subak.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBAK_H
#define UI_SUBAK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_subakClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTableWidget *tableWidget2;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *subakClass)
    {
        if (subakClass->objectName().isEmpty())
            subakClass->setObjectName(QStringLiteral("subakClass"));
        subakClass->resize(893, 784);
        centralWidget = new QWidget(subakClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 2, 0, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 0, 1, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);
        textEdit->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(textEdit, 0, 0, 1, 1);

        tableWidget2 = new QTableWidget(centralWidget);
        tableWidget2->setObjectName(QStringLiteral("tableWidget2"));

        gridLayout->addWidget(tableWidget2, 1, 0, 1, 1);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 4);
        gridLayout->setRowStretch(2, 5);

        verticalLayout_2->addLayout(gridLayout);

        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        verticalLayout_2->addWidget(plainTextEdit);

        subakClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(subakClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 893, 26));
        subakClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(subakClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        subakClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(subakClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        subakClass->setStatusBar(statusBar);

        retranslateUi(subakClass);

        QMetaObject::connectSlotsByName(subakClass);
    } // setupUi

    void retranslateUi(QMainWindow *subakClass)
    {
        subakClass->setWindowTitle(QApplication::translate("subakClass", "subak", 0));
        pushButton->setText(QApplication::translate("subakClass", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class subakClass: public Ui_subakClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBAK_H
