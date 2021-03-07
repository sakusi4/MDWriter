#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    Ui::SettingDialog *ui;

    QString get_directory_path();

public slots:
    void pushBtn_open_root_click();
    void pushBtn_save_click();
    void pushBtn_cancel_click();
};

#endif // SETTINGDIALOG_H
