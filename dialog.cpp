#include "dialog.h"
#include "./ui_dialog.h"

typedef enum
{
    nonePressed,
    addPressed,
    subPressed,
    mulPressed,
    divPressed,
    modPressed
}OPERATION;

OPERATION operation = nonePressed;

double calcVal = 0;



Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Calculator");

    QPushButton* NumBtns[10]{nullptr};
    for(int i =0 ; i <10 ; i++)
    {
        QString btnName = "btnNum"+QString::number(i);
       NumBtns[i] = this->findChild<QPushButton*>(btnName);
        connect(NumBtns[i],QPushButton::clicked ,this,this->numBtnClicked);
    }

    connect(ui->addBtn,QPushButton::clicked,this,this->numBtnClicked);
    connect(ui->subBtn,QPushButton::clicked,this,this->numBtnClicked);
    connect(ui->mulBtn,QPushButton::clicked,this,this->numBtnClicked);
    connect(ui->divBtn,QPushButton::clicked,this,this->numBtnClicked);
    connect(ui->modulusBtn,QPushButton::clicked,this,this->numBtnClicked);
    connect(ui->equalBtn,QPushButton::clicked,this,this->equalBtnClicked);
    connect(ui->clearBtn ,QPushButton::clicked , this ,this->clearBtnClicked);
    connect(ui->signBtn , QPushButton::clicked , this,this->signBtnClicked);


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::numBtnClicked()
{

    QPushButton* pressed = qobject_cast<QPushButton*>( sender());
    QString pressedStr = pressed->text();
    QString currentVal = ui->lineEdit->text();

    QString newValStr;


    if((currentVal =="" || currentVal == "0") && pressedStr != "⁒" && pressedStr!="÷" && pressedStr!="x" &&  pressedStr!="+")
    {

        ui->lineEdit->setText(pressedStr);
    }
    else if(currentVal !="" )
    {
        if(currentVal.back().isDigit())
        {
            newValStr = currentVal+pressedStr;
            ui->lineEdit->setText(newValStr);
        }

        else
        {
            if(pressedStr.back().isDigit())
            {
            newValStr = currentVal+pressedStr;
            ui->lineEdit->setText(newValStr);
            }
        }
    }






}



void Dialog::equalBtnClicked()
{

    if(ui->lineEdit->text().back().isDigit())
    {
    qInfo()<<"before : "<<ui->lineEdit->text();
    ui->lineEdit->setText(QString::number(calc(ui->lineEdit->text())));

    }


}

void Dialog::clearBtnClicked()
{
    calcVal = 0.0;
    operation =OPERATION::nonePressed;
    ui->lineEdit->setText("");

}

void Dialog::signBtnClicked()
{
    QString currentValStr = ui->lineEdit->text();
    QRegularExpression reg("[-]?[0-9.]*");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(reg,this));
    if(ui->lineEdit->hasAcceptableInput())
    {
        double newVal =  -1 * currentValStr.toDouble();
        ui->lineEdit->setText(QString::number(newVal));

    }

}

double Dialog::calc(QString line)
{


    double f;

        qsizetype x = line.indexOf("+");

        if(x!=-1)
        {
            return calc(line.sliced(0,x)) + calc(line.sliced(x+1));
        }

        x = line.indexOf("-",1);
        if(x!= -1 && x!=0)
        {
            qInfo()<<"now"<<line.sliced(x);
            return calc(line.sliced(0,x)) + calc(line.sliced(x));
        }

        x = line.indexOf("x");
        if(x!= -1 )
        {
            return  calc(line.sliced(0,x)) * calc(line.sliced(x+1));
        }

        x = line.indexOf("÷");
        if(x!= -1 )
        {
            if (line.at(x+1)=='0'){

                QMessageBox::information(this,"Error","Can't divid by zero");
            }

            return calc(line.sliced(0,x)) / calc(line.sliced(x+1));
        }
        x = line.indexOf("⁒");
        if(x!= -1 )
        {

            return fmod( calc(line.sliced(0,x)) , calc(line.sliced(x+1)));
        }

        double d = line.toDouble();
        qInfo()<<"d : "<<d;
        return d;











}

void Dialog::on_undoBtn_clicked()
{
    QString currentStr = ui->lineEdit->text();
    currentStr = currentStr.removeLast();
    ui->lineEdit->setText(currentStr);
}

