#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

#include <QtTest>
#include <QDebug>

#include <passworddialog.h>
#include <viewwindow.h>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_btnAdminLogin_clicked();

    void authenticate(QString);

    void restoreWindow();

private:
    Ui::LoginWindow *ui;

    PasswordDialog *pd;

    ViewWindow vw;

    QString progName_def = "SQLite Library System";

    int loginTries_def = 3;
    int loginTries_tmp = loginTries_def;

    void delay(int secs)
    {
        QTime dieTime= QTime::currentTime().addSecs(secs);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
};

#endif // LOGINWINDOW_H
