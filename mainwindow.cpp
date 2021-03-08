#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include "refdialog.h"
#include "setupdialog.h"
#include "aboutdialog.h"
#include "previewdialog.h"
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    read_setting_file();
    init();

    connect(ui->pushBtn_add_image, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_image_click()));
    connect(ui->pushBtn_add_code, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_code_click()));
    connect(ui->pushBtn_add_ref, SIGNAL(clicked(bool)), this, SLOT(pushBtn_add_ref_click()));
    connect(ui->pushBtn_submit, SIGNAL(clicked(bool)), this, SLOT(pushBtn_submit_click()));
    connect(ui->pushBtn_preview, SIGNAL(clicked(bool)), this, SLOT(pushBtn_preview_click()));
    connect(ui->pushBtn_toggle_list, SIGNAL(clicked(bool)), this, SLOT(pushBtn_toggle_list_click()));
    connect(ui->pushBtn_refresh_posts, SIGNAL(clicked(bool)), this, SLOT(pushBtn_refresh_posts_click()));
    connect(ui->menu_new, SIGNAL(triggered(bool)), this, SLOT(menu_new_click()));
    connect(ui->menu_open, SIGNAL(triggered(bool)), this, SLOT(menu_open_click()));
    connect(ui->menu_setting, SIGNAL(triggered(bool)), this, SLOT(menu_setting_click()));
    connect(ui->menu_about, SIGNAL(triggered(bool)), this, SLOT(menu_about_click()));
    connect(ui->listWidget_posts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(list_item_click(QListWidgetItem*)));
    connect(ui->listWidget_posts, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_context_menu(const QPoint&)));
    connect(ui->textEdit_content, SIGNAL(textChanged()), this, SLOT(textEdit_content_changed()));
}

void MainWindow::init()
{
    ui->textEdit_content->clear();
    md_file_name = "";
    md_only_file_name = "";
    is_changed = false;
    read_post_files();
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
    ui->textEdit_content->insertPlainText(dst_path + "\n");
}

void MainWindow::pushBtn_add_code_click()
{
    QString begin_code = "{% highlight c linenos=table %}\n\n";
    QString end_code = "{% endhighlight %}\n";

    ui->textEdit_content->insertPlainText(begin_code + end_code);
}

void MainWindow::pushBtn_add_ref_click()
{    
    static int ref_cnt = 0;

    RefDialog ref_dlg;
    int ret = ref_dlg.exec();
    if(ret != QDialog::Accepted)
        return;

    ref_cnt++;
    QString display = "Ref: [" + ref_dlg.display + "][" + QString::number(ref_cnt) + "]\n\n";
    QString link = "[" + QString::number(ref_cnt) + "]: " + ref_dlg.link + "\n";

    ui->textEdit_content->insertPlainText(display + link);
}

void MainWindow::pushBtn_toggle_list_click()
{
    QString str[2] = {"Show List", "Hide List"};
    bool visible = ui->listWidget_posts->isVisible();

    ui->listWidget_posts->setVisible(visible ^ true);
    ui->pushBtn_refresh_posts->setVisible(visible ^ true);
    ui->pushBtn_toggle_list->setText(str[visible ^ true]);
}

void MainWindow::pushBtn_refresh_posts_click()
{
    read_post_files();
}

void MainWindow::menu_new_click()
{
    save_md_file();

    SetupDialog dlg;
    int ret = dlg.exec();
    if(ret != QDialog::Accepted)
        return;

    md_only_file_name = dlg.file_name;
    md_file_name = md_only_file_name + ".md";
    set_md_header(dlg.layout, dlg.title, dlg.category, dlg.date);   
}

void MainWindow::menu_open_click()
{
    save_md_file();

    QString open_path = QFileDialog::getOpenFileName(this, "Open File", root_path + "/_posts", "Files (*.md)");
    if(open_path == "")
        return;

    load_md_file(open_path);
}

void MainWindow::menu_setting_click()
{
    SettingDialog dlg;
    int ret = dlg.exec();
    if(ret != QDialog::Accepted)
        return;

    read_setting_file();
    init();
}

void MainWindow::menu_about_click()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::pushBtn_submit_click()
{    
    save_md_file();
}

void MainWindow::pushBtn_preview_click()
{
    PreviewDialog dlg(ui->textEdit_content->toPlainText());
    dlg.exec();
}

void MainWindow::list_item_click(QListWidgetItem *item)
{    
    if(item == nullptr)
        return;

    QString name = item->text();

    save_md_file();
    load_md_file(root_path + "/_posts/" + name);
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

void MainWindow::load_md_file(const QString &file_name)
{
    QFileInfo file_info(file_name);
    md_file_name = file_info.fileName();
    md_only_file_name = file_info.baseName();

    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this, "info", "load_md_file: fail");
        return;
    }

    QTextStream stream(&file);
    QString content = stream.readAll();
    ui->textEdit_content->setText(content);

    file.close();

    is_changed = false;
}

void MainWindow::save_md_file()
{
    if(!is_changed){
        return;
    }else{
        auto ret = QMessageBox::information(this, "info", "Do you want to save this file?", QMessageBox::Yes | QMessageBox::No);
        if(ret != QMessageBox::Yes)
            return;
    }

    if(!create_md_file()){
        QMessageBox::information(this, "info", "save_md_file: fail");
        return;
    }

    copy_images();
    read_post_files();
}

bool MainWindow::create_md_file()
{
    QFile md_file;
    QString content = ui->textEdit_content->toPlainText();
    QString posts_path = root_path + "/_posts/";
    QString file_path = posts_path + md_file_name;

    md_file.setFileName(file_path);
    if(!md_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        QMessageBox::information(this, "info", "create_md_file: fail");
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
            QMessageBox::information(this, "info", "read_setting_file: fail");
            return;
        }
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, "info", "read_setting_file: fail");
        return;
    }

    QTextStream stream(&file);
    root_path = stream.readLine();

    if(root_path == ""){
        QMessageBox::information(this, "info", "Please set root path in setting form");
        return;
    }

    file.close();    
}

void MainWindow::read_post_files()
{
    ui->listWidget_posts->clear();

    QString posts_path = root_path + "/_posts";
    QDir dir(posts_path);
    QStringList list = dir.entryList(QStringList() << "*.md", QDir::Files);

    for(const QString &name : list){
        QListWidgetItem *item = new QListWidgetItem();
        QIcon *icon = new QIcon("./resources/list_item.png");
        item->setText(name);
        item->setIcon(*icon);
        ui->listWidget_posts->insertItem(ui->listWidget_posts->count(), item);
    }
}

void MainWindow::show_context_menu(const QPoint &pos)
{
    QPoint global_pos = ui->listWidget_posts->mapToGlobal(pos);
    QListWidgetItem *item =  ui->listWidget_posts->itemAt(pos);

    if(item == nullptr)
        return;

    QMenu menu(ui->listWidget_posts);
    QAction *action = menu.addAction("Delete");
    QAction *ret = menu.exec(global_pos);
    if(ret == action){        
        QString file_name = root_path + "/_posts/" + item->text();
        QFile file(file_name);
        if(file.remove()){
            init();
            QMessageBox::information(this, "info", "Delete Successed");
        }
    }
}

void MainWindow::textEdit_content_changed()
{
    is_changed = true;
}


MainWindow::~MainWindow()
{
    delete ui;
}
