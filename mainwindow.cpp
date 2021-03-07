#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include "refdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect(ui->pushBtn_set_header, SIGNAL(clicked(bool)), this, SLOT(pushBtn_set_header_click()));
    connect(ui->pushBtn_clear_contents, SIGNAL(clicked(bool)), this, SLOT(pushBtn_clear_contents_click()));
    connect(ui->pushBtn_add_image, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_image_click()));
    connect(ui->pushBtn_add_code, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_code_click()));
    connect(ui->pushBtn_add_ref, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_ref_click()));
    connect(ui->pushBtn_submit, SIGNAL(clicked(bool)), this, SLOT(pushBtn_submit_click()));
    connect(ui->menu_setting, SIGNAL(triggered(bool)), this, SLOT(menu_setting_click()));

    //QString time_format = "yyyy-MM-dd  HH:mm:ss";
    QString time_format = "yyyy-MM-dd";
    QDateTime cur_time = QDateTime::currentDateTime();
    QString md_date = cur_time.toString(time_format);

    ui->lineEdit_file_name->setText(md_date + "-");
    ui->lineEdit_date->setText(md_date);
}

void MainWindow::pushBtn_set_header_click()
{
    if(ui->textEdit_content->toPlainText().length() > 0){
        QMessageBox::information(this, "info", "Contents is not empty", QMessageBox::Ok);
        return;
    }

    md_only_file_name = ui->lineEdit_file_name->text();
    md_file_name = md_only_file_name + ".md";
    if(md_file_name.length() <= 14){
        qDebug("no file name");
        return;
    }

    md_title = ui->lineEdit_title->text();
    md_category = ui->lineEdit_category->text();
    md_date = ui->lineEdit_date->text();

    set_md_header();
}

void MainWindow::pushBtn_clear_contents_click()
{
    QMessageBox::StandardButton ret =  QMessageBox::question(this, "info", "Really?", QMessageBox::Yes | QMessageBox::No);
    if(ret != QMessageBox::Yes)
        return;

    ui->textEdit_content->clear();
}

void MainWindow::set_md_header()
{
    QString header = "---\n";
    header += "layout: post\n";
    header += "title: \"" + md_title + "\"\n";
    header += "category: " + md_category + "\n";
    header += "date: " + md_date + "\n";
    header += "---\n";

    ui->textEdit_content->setText(header);
}

void MainWindow::pushBtn_add_image_click()
{
    //![0](/assets/images/name/0.png)
    QString src_path = QFileDialog::getOpenFileName(this, "Open File", QDir::rootPath(), "Files (*.png *.jpg)");
    if(src_path == "")
        return;

    QFileInfo file(src_path);
    QString img_name = file.fileName();
    QString dst_path = "![0](/assets/images/" + md_only_file_name + "/" + img_name + ")";

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
    QString display = "참고 자료: [" + ref_dlg.display + "][" + QString::number(ref_cnt) + "]\n";
    QString link = "[" + QString::number(ref_cnt) + "]: " + ref_dlg.link + "\n";

    ui->textEdit_content->append(display + link);
}

void MainWindow::menu_setting_click()
{
    SettingDialog dlg;
    dlg.exec();
}

void MainWindow::pushBtn_submit_click()
{    
    create_md_file();
    copy_images();
    QMessageBox::information(this, "info", "Complete");   
}

void MainWindow::create_md_file()
{
    QFile md_file;
    QString content = ui->textEdit_content->toPlainText();
    QString posts_path = root_path + "/_posts/";
    QString file_path = posts_path + md_file_name;

    md_file.setFileName(file_path);
    if(QFile::exists(file_path)){
        qDebug("this file is already exists");
        return;
    }

    if(!md_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug("File open fail");
        return;
    }

    QTextStream stream(&md_file);
    stream << content;
    md_file.close();
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

MainWindow::~MainWindow()
{
    delete ui;
}


