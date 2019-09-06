#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();

signals:
    void authenticate(QString);

    void restoreWindow();

private slots:
    void on_btxOKCancel_accepted();

    void on_btxOKCancel_rejected();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
