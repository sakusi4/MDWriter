#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include "refdialog.h"
#include "setupdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    read_setting_file();

    connect(ui->pushBtn_add_image, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_image_click()));
    connect(ui->pushBtn_add_code, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_code_click()));
    connect(ui->pushBtn_add_ref, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_ref_click()));
    connect(ui->pushBtn_submit, SIGNAL(clicked(bool)), this, SLOT(pushBtn_submit_click()));
    connect(ui->menu_new, SIGNAL(triggered(bool)), this, SLOT(menu_new_click()));
    connect(ui->menu_open, SIGNAL(triggered(bool)), this, SLOT(menu_open_click()));
    connect(ui->menu_setting, SIGNAL(triggered(bool)), this, SLOT(menu_setting_click()));
}

void MainWindow::pushBtn_add_image_click()
{
    //![0](/assets/images/name/0.png)
    QString src_path = QFileDialog::getOpenFileName(this, "Open File", QDir::rootPath(), "Files (*.png *.jpg)");
    if(src_path == "")
        return;

    QFileInfo file(src_path);
    QString img_name = file.fileName();
    QString dst_path = "![" + src_path + "](/assets/images/" + md_only_file_name + "/" + img_name + ")";

    img_datas.push_back(make_tuple(img_name, src_path, dst_path));
    ui->textEdit_content->append(dst_path + "\n");
}

void MainWindow::pushBtn_add_code_click()
{
    QString begin_code = "{% highlight c linenos=table %}\n\n";
    QString end_code = "{% endhighlight %}\n";

    ui->textEdit_content->append(begin_code + end_code);
}

void MainWindow::pushBtn_add_ref_click()
{    
    static int ref_cnt = 0;

    RefDialog ref_dlg;
    int ret = ref_dlg.exec();
    if(ret != QDialog::Accepted)
        return;

    ref_cnt++;
    QString display = "참고 자료: [" + ref_dlg.display + "][" + QString::number(ref_cnt) + "]\n\n";
    QString link = "[" + QString::number(ref_cnt) + "]: " + ref_dlg.link + "\n";

    ui->textEdit_content->append(display + link);
}

void MainWindow::menu_new_click()
{
    SetupDialog dlg;
    int ret = dlg.exec();
    if(ret != QDialog::Accepted)
        return;

    md_only_file_name = dlg.file_name;
    md_file_name = md_only_file_name + ".md";
    set_md_header(dlg.layout, dlg.title, dlg.category, dlg.date);

    is_ready_submit = true;
}

void MainWindow::menu_open_click()
{
    if(is_ready_submit){
        auto ret = QMessageBox::information(this, "info", "This file will be not saved, OK?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(ret != QMessageBox::Yes)
            return;
    }

    QString open_path = QFileDialog::getOpenFileName(this, "Open File", root_path + "/_posts", "Files (*.md)");
    if(open_path == "")
        return;

    QFile file(open_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, "info", "file open fail");
        return;
    }

    QFileInfo file_info(open_path);
    md_file_name = file_info.fileName();
    md_only_file_name = file_info.baseName();

    QTextStream stream(&file);
    QString content = stream.readAll();
    ui->textEdit_content->setText(content);

    is_ready_submit = true;
}

void MainWindow::menu_setting_click()
{
    SettingDialog dlg;
    dlg.exec();
}

void MainWindow::pushBtn_submit_click()
{    
    if(!is_ready_submit){
        QMessageBox::information(this, "info", "No Documents");
        return;
    }

    auto ret = QMessageBox::information(this, "info", "Really?", QMessageBox::Yes | QMessageBox::No);
    if(ret != QMessageBox::Yes)
        return;

    if(!create_md_file()){
        QMessageBox::information(this, "info", "Fail");
        return;
    }

    copy_images();
    QMessageBox::information(this, "info", "Complete");
}

void MainWindow::set_md_header(QString layout, QString title, QString category, QString date)
{
    QString header = "---\n";
    header += "layout: " + layout + "\n";
    header += "title: \"" + title + "\"\n";
    header += "category: " + category + "\n";
    header += "date: " + date + "\n";
    header += "---\n";

    ui->textEdit_content->setText(header);
}

bool MainWindow::create_md_file()
{
    QFile md_file;
    QString content = ui->textEdit_content->toPlainText();
    QString posts_path = root_path + "/_posts/";
    QString file_path = posts_path + md_file_name;

    md_file.setFileName(file_path);
    if(QFile::exists(file_path)){
        auto ret = QMessageBox::information(this, "info", "Do you want to overwrite?", QMessageBox::Yes | QMessageBox::No);
        if(ret != QMessageBox::Yes){
            return false;
        }
    }

    if(!md_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug("File open fail");
        return false;
    }

    QTextStream stream(&md_file);
    stream << content;
    md_file.close();
    return true;
}

void MainWindow::copy_images()
{
    QString content = ui->textEdit_content->toPlainText();
    QString images_path = root_path + "/assets/images/" + md_only_file_name + "/";
    QDir dir;
    dir.mkpath(images_path);

    for(const auto &data : img_datas){
        QString file_name = get<0>(data);
        QString src_path = get<1>(data);
        QString command = get<2>(data);
        if(content.contains(command)){
            QFile::copy(src_path, images_path + file_name);
        }
    }
}

void MainWindow::read_setting_file()
{
    QString setting_path = "./setting.ini";
    QFile file(setting_path);
    if(!QFile::exists(setting_path)){
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
            qDebug("File open fail");
            return;
        }
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("File open fail");
        return;
    }

    QTextStream stream(&file);
    root_path = stream.readLine();

    if(root_path == "")
        QMessageBox::information(this, "info", "Please set root path in setting form");
}

MainWindow::~MainWindow()
{
    delete ui;
}


