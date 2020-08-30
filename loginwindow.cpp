#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // Override project defaults with our fork's new name.
    setApplicationName();

    // Check the database for anomalies.
    // TODO: make this function more readable / extensible
    checkDB();

    // Set our bundled login icon.
    // TODO: make this function more readable
    QString login_icon_path = ":/login.png";
    QPixmap login_icon(login_icon_path);
    setLoginIcon(login_icon);

    // Set our pretty name as the default status text.
    QString default_status = QCoreApplication::organizationName();
    setNewStatusText(default_status);
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
    // Hardcode the admin password for now (INSECURE!).
    // TODO: Implement hashed password authentication with salting.
    QString raw_password = "goodpass";
    // Use our pretty name as the default status text.
    QString default_status = QCoreApplication::organizationName();
    // Declare status text for failed and waiting.
    QString failed_status = "Login failed";
    QString waiting_status = "Please wait...";

    // Basic plain-text authentication.
    if (password == raw_password) {
        launch_ViewWindow();
    }
    else {
        // Disable window, and update status for incorrect passwords.
        disableWindow();
        setNewStatusText(waiting_status);

        // Try to deter brute-force attacks by delaying execution.
        delay_secs(2);

        // Retry password authentication if the number of tries
        // has not exceeded 3.
        if (login_tries < 3) {
            launch_PasswordDialog();
        }
        else {
            // If the number of allowed tries has been exceeded,
            // re-enable the window, and set a 'failed' status message.
            enableWindow();
            setNewStatusText(failed_status);

            // Reset status message after some time.
            delay_secs(2);
            setNewStatusText(default_status);
        }
    }
}

void LoginWindow::restoreWindow() {
    enableWindow();
}

void LoginWindow::unhideWindow() {
    showWindow();
}

void LoginWindow::on_btnAdminLogin_clicked()
{
    // Always reset the number of login attempts whenever trying
    // to authenticate.
    login_tries = 0;
    launch_PasswordDialog();
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

void LoginWindow::setApplicationName() {
    QCoreApplication::setApplicationName("qlibmgsys");
    QCoreApplication::setOrganizationName("Qt Library Management System");
    QCoreApplication::setOrganizationDomain("qlibmgsys.org");
}

void LoginWindow::setNewStatusText(QString new_status) {
    ui->lblProgramName->setText(new_status);
}

void LoginWindow::setLoginIcon(QPixmap login_icon) {
    ui->pixLogin->
    setPixmap(login_icon.scaled(ui->pixLogin->width(),      // Make sure to use the image QObject's width.
                                ui->pixLogin->height(),     // Make sure to use the image QObject's height.
                                Qt::KeepAspectRatio,        // Do not stretch the image in weird ways.
                                Qt::SmoothTransformation)); // Anti-alias the image to avoid jarred resizing.
}

void LoginWindow::launch_ViewWindow() {
    // Instanstiate our View window and show it.
    vw = new ViewWindow(this);
    vw->show();

    // Connect to ViewWindow in case it wants us to appear again later on.
    connect(vw, SIGNAL(unhideWindow()), this, SLOT(unhideWindow()));

    // Just hide ourselves while we wait for the application to either exit,
    // or make us re-appear.
    hideWindow();

    // Make sure to re-enable our window after switching to the new one.
    enableWindow();
}

void LoginWindow::launch_PasswordDialog() {
    // Record our login attempt.
    login_tries += 1;

    // Instantiate our Password dialog as a modal and show it.
    pd = new PasswordDialog(this);
    pd->setModal(true);
    pd->show();

    // Connect to PasswordDialog for when it:
    // a) wants us to authenticate the given password; or
    connect(pd, SIGNAL(authenticate(QString)), this, SLOT(authenticate(QString)));
    // b) wants us to restore our window immediately (due to auth cancellation).
    connect(pd, SIGNAL(authInfoCancelled()), this, SLOT(restoreWindow()));
}

void LoginWindow::enableWindow() {
    ui->centralWidget->setEnabled(true);
}

void LoginWindow::disableWindow() {
    ui->centralWidget->setDisabled(true);
}

void LoginWindow::showWindow() {
    this->show();
}

void LoginWindow::hideWindow() {
    this->hide();
}
