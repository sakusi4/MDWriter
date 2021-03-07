#include "previewdialog.h"
#include "ui_previewdialog.h"

PreviewDialog::PreviewDialog(const QString &content, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewDialog)
{
    ui->setupUi(this);

    ui->textEdit_content->setReadOnly(true);
    ui->textEdit_content->setMarkdown(content);
}

PreviewDialog::~PreviewDialog()
{
    delete ui;
}
