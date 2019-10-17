#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = nullptr);
    ~UpdateDialog();

private slots:
    void toggleOKBtn();
    void loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString);

    void on_btnOKCancel_accepted();

    void on_btnCoverPhoto_clicked();

signals:
    void updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::UpdateDialog *ui;

    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


};

#endif // UPDATEDIALOG_H
