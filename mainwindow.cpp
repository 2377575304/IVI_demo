#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>
#include <QFileInfo>
#include <qdir.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioMgr = new AudioManager(this);
    // 连接播放状态变化信号
    connect(audioMgr, &AudioManager::playbackStateChanged,
            this, &MainWindow::onPlaybackStateChanged);
    connect(audioMgr,&AudioManager::positionChanged,this,
        &MainWindow::onPositionChanged);
    connect(audioMgr, &AudioManager::lyricLoaded, this, &MainWindow::updateLyrics);
    connect(audioMgr, &AudioManager::songTitleChanged, this, &MainWindow::onSongTitleChanged);




//链接地图信号



}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_music_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->musiclist->clear();//读取前先清除列表
    QString audiopath = "D:/desktop/anything/Qt/IVI_demo";
    QStringList files = audioMgr->scanAudioFiles(audiopath);
    
    // 设置播放列表
    audioMgr->setPlaylist(files, audiopath);
    
    // 添加到UI列表
    for (const QString &file : files) {
        ui->musiclist->addItem(file);
    }
}

void MainWindow::on_pushButton_vedio_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_map_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_ctl_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_musiclist_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    QString filePath = "D:/desktop/anything/Qt/IVI_demo/" + fileName; // 根据实际情况改路径
    audioMgr->loadAndPlay(filePath);
}


void MainWindow::on_last_clicked()
{
    // 播放上一首歌曲
    audioMgr->playPrevious();
}


void MainWindow::on_pushButton_stop_clicked()
{
    // 切换播放/暂停状态
    audioMgr->togglePlayPause();
}


void MainWindow::on_next_clicked()
{
    // 播放下一首歌曲
    audioMgr->playNext();
}

// 处理播放状态变化
void MainWindow::onPlaybackStateChanged(bool isPlaying)
{
    // 根据播放状态更新按钮文本
    if (isPlaying) {
        ui->pushButton_stop->setText("||"); // 暂停符号
    } else {
        ui->pushButton_stop->setText(">"); // 播放符号
    }
}


// 处理播放进度变化
void MainWindow::onPositionChanged(qint64 position, qint64 duration)
{
    // 避免除以零错误
    if (duration <= 0) {
        ui->slider_progress->setValue(0);
        ui->label_currentTime->setText("00:00");
        ui->label_totalTime->setText("00:00");
        return;
    }
    // 更新进度条位置（转换为千分比）
    int sliderPosition = static_cast<int>((position * 1000) / duration);
    ui->slider_progress->setValue(sliderPosition);
    // 更新时间标签
    ui->label_currentTime->setText(formatTime(position));
    ui->label_totalTime->setText(formatTime(duration));

    // 高亮当前歌词
    highlightCurrentLyric(position);
}

// 处理进度条拖动
/*void MainWindow::on_slider_progress_sliderMoved(int position)
{
    // 获取当前总时长
    qint64 duration = audioMgr->duration();
    
    // 计算新的播放位置
    qint64 newPosition = (duration * position) / 1000;
    
    // 设置新的播放位置
    audioMgr->setPosition(newPosition);
}*/

// 格式化时间（毫秒转为mm:ss格式）
QString MainWindow::formatTime(qint64 timeInMs)
{
    int seconds = static_cast<int>(timeInMs / 1000) % 60;
    int minutes = static_cast<int>(timeInMs / 60000);
    
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void MainWindow::updateLyrics(const QStringList &lyrics) {
    ui->lyriclist->clear();
    for (const QString &lyric : lyrics) {
        ui->lyriclist->addItem(lyric);
    }
    currentLyricIndex = -1;
}


void MainWindow::highlightCurrentLyric(qint64 position) {
    // 需要从 AudioManager 获取当前歌词索引（稍后添加）
    int newIndex = audioMgr->getCurrentLyricIndex(position);
    if (newIndex != currentLyricIndex && newIndex >= 0 && newIndex < ui->lyriclist->count()) {
        // 取消上一行高亮
        if (currentLyricIndex >= 0) {
            ui->lyriclist->item(currentLyricIndex)->setForeground(Qt::black);
        }
        // 高亮当前行
        ui->lyriclist->item(newIndex)->setForeground(Qt::red);
        // 滚动到当前行
        ui->lyriclist->scrollToItem(ui->lyriclist->item(newIndex), QAbstractItemView::PositionAtCenter);
        currentLyricIndex = newIndex;
    }
}

void MainWindow::onSongTitleChanged(const QString &title) {
    ui->label_title->setText(title);
}


