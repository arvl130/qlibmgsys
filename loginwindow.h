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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "db_conn");

    int login_tries = 0;

    void delay_secs(int seconds)
    {
        QTime dieTime= QTime::currentTime().addSecs(seconds);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    void setApplicationName();
    void setNewStatusText(QString new_status);
    void setLoginIcon(QPixmap login_icon);

    void launch_ViewWindow();
    void launch_PasswordDialog();

    void enableWindow();
    void disableWindow();

    void hideWindow();
    void showWindow();
};

#endif // LOGINWINDOW_H
