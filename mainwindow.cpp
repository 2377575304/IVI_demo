#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    audioMgr = new AudioManager(this);
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

