#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void append();
    void clearEquation();
    void on_btnEqual_clicked();

private:
    Ui::Dialog *ui;
    QString temp;
    QString equation;
    std::vector<QString> spaced_equation;

    QString makeSolution(std::vector<QString>& strings);
    std::vector<QString> toDivideEquation(const QString str);
    void setBtnDisabled();
    void setBtnEnabled();
};
#endif // DIALOG_H
