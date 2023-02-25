#include "dialog.h"
#include "ui_dialog.h"
#include <algorithm>
#include <stdexcept>

#include <QDataStream>
#include <iostream>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->btnOne, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnTwo, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnThree, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnFour, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnFive, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnSix, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnSeven, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnEigth, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnNine, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnZero, &QPushButton::clicked, this, &Dialog::append);

    connect(ui->btnDivide, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnMultiply, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnPlus, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnMinus, &QPushButton::clicked, this, &Dialog::append);
    connect(ui->btnPoint, &QPushButton::clicked, this, &Dialog::append);

    connect(ui->btnClear, &QPushButton::clicked, this, &Dialog::clearEquation);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::append()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    equation.append(btn->text());
    if (equation.endsWith("/") || equation.endsWith("*") || equation.endsWith("-") || equation.endsWith("+")){
        setBtnDisabled();
    } else {
        setBtnEnabled();
    }
    ui->lblEquation->setText("Equation: " + equation);
}

void Dialog::clearEquation()
{
    equation.clear();
    ui->lblEquation->setText("Equation: " + equation);
}


void Dialog::on_btnEqual_clicked()
{
    spaced_equation = toDivideEquation(equation);
    equation.clear();
    QString answer = makeSolution(spaced_equation);
    ui->lcdAnswer->display(answer);
}

void cleanVector(std::vector<QString>& strings){
    while (count(strings.begin(), strings.end(), "_") != 0){
        QString temp = "_";
        auto it = std::find(strings.begin(), strings.end(), temp);
        strings.erase(it);
    }
}

void makeEquation(std::vector<QString>& strings, int i){
    QString rhd_point = strings[i+1];
    strings[i+1] = "_";
    QString lhd_point = strings[i-1];
    strings[i-1] = "_";
    //qInfo() << lhd_point << " : " << rhd_point;
    float rhd = rhd_point.toFloat();
    float lhd = lhd_point.toFloat();
    //qInfo() << lhd << " : " << rhd;
    if (strings[i] == "/"){
        if (std::abs(rhd) < 0.00000001){
            throw std::invalid_argument("You can't divide by zero");
        }
        QString num;
        strings[i] = num.setNum(lhd / rhd);
    } else if (strings[i] == "*"){
        QString num;
        strings[i] = num.setNum(lhd * rhd);
    } else if (strings[i] == "-"){
        QString num;
        strings[i] = num.setNum(lhd - rhd);
    } else if (strings[i] == "+"){
        QString num;
        strings[i] = num.setNum(lhd + rhd);
    }
}

void doEquations(std::vector<QString>& strings, QString str){
    if (strings.size() == 1) return;
    //qInfo() << "end of iteration";
    while (count(strings.begin(), strings.end(), str) != 0){
        auto it = std::find(strings.begin(), strings.end(), str);
        int i = std::distance(strings.begin(), it);
        makeEquation(strings, i);
        cleanVector(strings);
    }
}

QString Dialog::makeSolution(std::vector<QString>& strings)
{
    doEquations(strings, "/");
    doEquations(strings, "*");
    doEquations(strings, "-");
    doEquations(strings, "+");
    if (strings.size() > 1){
        throw std::logic_error("There more numbers at the end of calculations, then expected");
    }
    return strings.at(0);
}

// Provereno
std::vector<QString> Dialog::toDivideEquation(const QString str)
{
    std::vector<QString> answer;
    QString temp_str;
    for (int i=0; i < str.size(); ++i){
        if (str.at(i) == "/" || str.at(i) == "*" || str.at(i) == "+" || str.at(i) == "-" || str.at(i) == "(" || str.at(i) == ")"){
            answer.push_back(temp_str);
            QString ch = QString(str.at(i));
            answer.push_back(ch);
            temp_str.clear();
        } else {
            temp_str.append(str.at(i));
        }
    }
    answer.push_back(temp_str);
    return answer;
}

void Dialog::setBtnDisabled(){
    ui->btnPlus->setEnabled(false);
    ui->btnMinus->setEnabled(false);
    ui->btnMultiply->setEnabled(false);
    ui->btnDivide->setEnabled(false);
}

void Dialog::setBtnEnabled(){
    ui->btnPlus->setEnabled(true);
    ui->btnMinus->setEnabled(true);
    ui->btnMultiply->setEnabled(true);
    ui->btnDivide->setEnabled(true);
}


