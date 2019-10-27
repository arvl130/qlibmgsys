#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

#include <QtTest>
#include <QDebug>
#include <QtSql>

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
    void checkDB();

    void authenticate(QString);

    void restoreWindow();

    void unhideWindow();

    void on_btnAdminLogin_clicked();

    void on_btnUserLogin_clicked();

signals:
    void disableAdminBtns();

private:
    Ui::LoginWindow *ui;
    PasswordDialog *pd;
    ViewWindow *vw;

    QString progName_def = "SQLite Library System";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "db_conn");

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
