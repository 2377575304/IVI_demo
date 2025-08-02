#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include "audiomanager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_main_clicked();

    void on_pushButton_music_clicked();

    void on_pushButton_vedio_clicked();

    void on_pushButton_map_clicked();

    void on_pushButton_ctl_clicked();

    void on_musiclist_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    AudioManager *audioMgr;
};
#endif // MAINWINDOW_H
