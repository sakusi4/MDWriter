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

    QString md_date;
    QString md_title;
    QString md_category;
    QString md_file_name;
    QString md_only_file_name;
    QString content;

    // file_name, src_path, command
    vector<tuple<QString, QString, QString>> img_datas;

    void create_md_file();
    void copy_images();
    void set_md_header();
public slots:    
    void pushBtn_submit_click();
    void pushBtn_set_header_click();    
    void pushBtn_clear_contents_click();
    void pushBtn_add_image_click();
    void pushBtn_add_code_click();
    void pushBtn_add_ref_click();
    void menu_setting_click();
};
#endif // MAINWINDOW_H
