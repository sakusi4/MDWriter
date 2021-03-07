#ifndef REFDIALOG_H
#define REFDIALOG_H

#include <QDialog>

namespace Ui {
class RefDialog;
}

class RefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RefDialog(QWidget *parent = nullptr);
    ~RefDialog();

    QString display;
    QString link;

private:
    Ui::RefDialog *ui;

public slots:
    void pushBtn_ok_click();
    void pushBtn_cancel_click();
};

#endif // REFDIALOG_H
