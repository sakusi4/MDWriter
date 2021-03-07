#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class PreviewDialog;
}

class PreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewDialog(const QString &content, QWidget *parent = nullptr);
    ~PreviewDialog();   

    QString content;

private:
    Ui::PreviewDialog *ui;
};

#endif // PREVIEWDIALOG_H
