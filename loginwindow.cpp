#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    QPixmap login_ico(":/login.png");
    ui->pixLogin->setPixmap(login_ico.scaled(ui->pixLogin->width(), ui->pixLogin->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblProgramName->setText(progName_def);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::authenticate(QString password) {
    QString rawpasswd = "goodpass";

    if (password == rawpasswd) {
        restoreWindow();
        vw = new ViewWindow(this);
        vw->show();
        connect(vw, SIGNAL(unhideWindow()), this, SLOT(unhideWindow()));
        this->hide();
    }
    else {
        ui->centralWidget->setEnabled(false);
        ui->lblProgramName->setText("Please wait...");
        delay(2);
        ui->lblProgramName->setText(progName_def);

        loginTries_tmp -= 1;
        if (loginTries_tmp == 0) {
            restoreWindow();
            ui->lblProgramName->setText("Login failed");
            delay(2);
            ui->lblProgramName->setText(progName_def);
        }
        else {
            pd = new PasswordDialog(this);
            pd->setModal(true);
            pd->show();
            connect(pd, SIGNAL(authenticate(QString)), this, SLOT(authenticate(QString)));
            connect(pd, SIGNAL(restoreWindow()), this, SLOT(restoreWindow()));
        }
    }
}

void LoginWindow::restoreWindow() {
    loginTries_tmp = loginTries_def;
    ui->centralWidget->setEnabled(true);
}

void LoginWindow::unhideWindow() {
    this->show();
}

void LoginWindow::on_btnAdminLogin_clicked()
{
     pd = new PasswordDialog(this);
     pd->setModal(true);
     pd->show();
     connect(pd, SIGNAL(authenticate(QString)), this, SLOT(authenticate(QString)));
}

void LoginWindow::on_btnUserLogin_clicked()
{
    vw = new ViewWindow(this);
    vw->show();
    connect(this, SIGNAL(disableAdminBtns()), vw, SLOT(disableAdminBtns()));
    emit disableAdminBtns();
    connect(vw, SIGNAL(unhideWindow()), this, SLOT(unhideWindow()));
    this->hide();
}
