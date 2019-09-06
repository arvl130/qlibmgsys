#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_btxOKCancel_accepted()
{
    if (!ui->txtPassword->text().isEmpty()) {
        emit authenticate(ui->txtPassword->text());
    }
}

void PasswordDialog::on_btxOKCancel_rejected()
{
    emit restoreWindow();
}
