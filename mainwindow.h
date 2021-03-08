#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>
#include <QDate>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <vector>
#include <tuple>
#include <QListWidget>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern QString root_path;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool is_changed;
    QString md_file_name;
    QString md_only_file_name;

    // file_name, src_path, command
    vector<tuple<QString, QString, QString>> img_datas;

    void init();
    bool create_md_file();
    void copy_images();
    void set_md_header(QString layout, QString title, QString category, QString date);
    void read_setting_file();
    void load_md_file(const QString &file_name);
    void save_md_file();
    void read_post_files();

public slots:    
    void pushBtn_preview_click();
    void pushBtn_submit_click();
    void pushBtn_add_image_click();
    void pushBtn_add_code_click();
    void pushBtn_add_ref_click();
    void pushBtn_toggle_list_click();
    void pushBtn_refresh_posts_click();
    void menu_new_click();
    void menu_open_click();
    void menu_setting_click();
    void menu_about_click();
    void list_item_click(QListWidgetItem*);
    void show_context_menu(const QPoint& pos);
    void textEdit_content_changed();
};
#endif // MAINWINDOW_H
