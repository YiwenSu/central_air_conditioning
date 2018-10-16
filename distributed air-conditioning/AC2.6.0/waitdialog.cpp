#include "waitdialog.h"
#include "ui_waitdialog.h"

waitDialog::waitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    QMovie *movie = new QMovie(":/wait.gif");
    ui->label->setMovie(movie);
    movie->start();

}

waitDialog::~waitDialog()
{
    delete ui;
}
