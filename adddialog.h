#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();

private slots:
    void toggleOKBtn();

    void on_btnOKCancel_accepted();

    void on_btnCoverPhoto_clicked();

signals:
    void addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::AddDialog *ui;

    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
};

#endif // ADDDIALOG_H
