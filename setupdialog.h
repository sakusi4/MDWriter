#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog();

    QString file_name;
    QString layout;
    QString title;
    QString category;
    QString date;

private:
    Ui::SetupDialog *ui;

public slots:
    void pushBtn_ok_click();
    void pushBtn_cancel_click();
};

#endif // SETUPDIALOG_H
