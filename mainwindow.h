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
    
    void on_last_clicked();
    
    void on_pushButton_stop_clicked();
    
    void on_next_clicked();
    
    // 处理播放状态变化
    void onPlaybackStateChanged(bool isPlaying);
    
    // 处理播放进度变化
    void onPositionChanged(qint64 position, qint64 duration);
    
    // 处理进度条拖动 有bug为解决
    //void on_slider_progress_sliderMoved(int position);

private:
    // 格式化时间（毫秒转为mm:ss格式）
    QString formatTime(qint64 timeInMs);
    
    Ui::MainWindow *ui;
    AudioManager *audioMgr;
    
    
};
#endif // MAINWINDOW_H
