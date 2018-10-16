/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QLabel *title;
    QLineEdit *inputID;
    QLineEdit *inputPWD;
    QPushButton *button;
    QPushButton *forget;
    QLabel *label;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(421, 390);
        Login->setStyleSheet(QStringLiteral("border-image: url(:/login/loginback.png);"));
        title = new QLabel(Login);
        title->setObjectName(QStringLiteral("title"));
        title->setGeometry(QRect(90, 110, 251, 21));
        title->setStyleSheet(QStringLiteral("font: 775 15pt \"Lato\";"));
        inputID = new QLineEdit(Login);
        inputID->setObjectName(QStringLiteral("inputID"));
        inputID->setGeometry(QRect(40, 150, 341, 51));
        inputID->setStyleSheet(QLatin1String("border-image: url(:/login/inputline.png);\n"
"font: 9pt \"Lato\";"));
        inputPWD = new QLineEdit(Login);
        inputPWD->setObjectName(QStringLiteral("inputPWD"));
        inputPWD->setGeometry(QRect(40, 230, 341, 51));
        inputPWD->setStyleSheet(QLatin1String("border-image: url(:/login/inputline.png);\n"
"font: 9pt \"Lato\";"));
        inputPWD->setEchoMode(QLineEdit::Normal);
        button = new QPushButton(Login);
        button->setObjectName(QStringLiteral("button"));
        button->setGeometry(QRect(270, 310, 101, 41));
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setStyleSheet(QStringLiteral("border-image: url(:/login/button.png);"));
        forget = new QPushButton(Login);
        forget->setObjectName(QStringLiteral("forget"));
        forget->setGeometry(QRect(40, 300, 211, 21));
        forget->setCursor(QCursor(Qt::PointingHandCursor));
        forget->setStyleSheet(QLatin1String("font: 775 11pt \"Lato\";\n"
"color: rgb(0, 161, 133);"));
        label = new QLabel(Login);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 10, 331, 81));
        label->setStyleSheet(QStringLiteral("border-image: url(:/holiday.png);"));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Form", Q_NULLPTR));
        title->setText(QApplication::translate("Login", "Welcome back home!", Q_NULLPTR));
        inputID->setInputMask(QString());
        inputID->setText(QString());
        inputID->setPlaceholderText(QApplication::translate("Login", "User/ID:", Q_NULLPTR));
        inputPWD->setInputMask(QString());
        inputPWD->setText(QString());
        inputPWD->setPlaceholderText(QApplication::translate("Login", "roomID", Q_NULLPTR));
        button->setText(QString());
        forget->setText(QApplication::translate("Login", ">>Forget your password?", Q_NULLPTR));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
