#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    checkDB();

    QPixmap login_ico(":/login.png");
    ui->pixLogin->setPixmap(login_ico.scaled(ui->pixLogin->width(), ui->pixLogin->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblProgramName->setText(progName_def);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::checkDB() {
    QString dbpath_def = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString dbpath_def_db = dbpath_def + "/" + "items.db";

    QFileInfo dbpath_def_finfo(dbpath_def);
    QFileInfo dbpath_def_db_finfo(dbpath_def_db);


    if (dbpath_def_finfo.isDir() &&
        dbpath_def_finfo.isReadable() &&
        dbpath_def_finfo.isWritable()) {
        if (dbpath_def_db_finfo.isFile() &&
            dbpath_def_db_finfo.isReadable() &&
            dbpath_def_db_finfo.isWritable()) {
            db.setDatabaseName(dbpath_def_db);
        }
        else {
            QMessageBox::critical(this, "Database Error", "The database file <i>"
                                  + dbpath_def_db + "</i> is not modifiable. Make sure the"
                                                    " database has the right permissions.");

            // close() or QApplication::quit() do not work inside the constructors of inital windows
            // see: https://stackoverflow.com/questions/2356778/closing-a-qmainwindow-on-startup
            QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
        }
    }
    else {
        QMessageBox::critical(this, "Database Error", "The directory for the database file <i>"
                              + dbpath_def + "</i> could not be opened. Make sure the directory"
                                             " has the right permissions.");
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
    }
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
