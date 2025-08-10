#include "videomanager.h"
#include <QFileInfo>
#include <QDebug>

VideoManager::VideoManager(QObject *parent)
    : QObject(parent)
    , player(new QMediaPlayer(this))
    , videoWidget(nullptr)
    , currentIndex(-1)
{
    // 连接信号和槽 - 修复：使用playbackStateChanged而不是stateChanged
    connect(player, &QMediaPlayer::positionChanged, this, &VideoManager::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &VideoManager::onDurationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &VideoManager::onStateChanged);
}

VideoManager::~VideoManager()
{
    if (player) {
        player->stop();
    }
}

QStringList VideoManager::scanVideoFiles(const QString &directory)
{
    QStringList videoFiles;
    QDir dir(directory);
    
    // 支持的视频格式
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.mkv" << "*.mov" << "*.wmv" << "*.flv";
    
    // 获取目录中的视频文件
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);
    
    for (const QString &file : files) {
        videoFiles.append(file);
    }
    
    return videoFiles;
}

void VideoManager::setPlaylist(const QStringList &files, const QString &path)
{
    playlist = files;
    basePath = path;
    currentIndex = -1;
}

void VideoManager::loadAndPlay(const QString &filePath)
{
    QString fullPath;
    
    // 如果是相对路径，添加基础路径
    if (QDir::isRelativePath(filePath)) {
        fullPath = QDir(basePath).filePath(filePath);
    } else {
        fullPath = filePath;
    }
    
    // 查找文件在播放列表中的索引
    for (int i = 0; i < playlist.size(); ++i) {
        if (playlist[i] == filePath || playlist[i] == QFileInfo(fullPath).fileName()) {
            currentIndex = i;
            break;
        }
    }
    
    // 加载并播放视频
    player->setSource(QUrl::fromLocalFile(fullPath));
    player->play();
    
    // 发送视频标题信号
    emit videoTitleChanged(QFileInfo(fullPath).fileName());
}

void VideoManager::play()
{
    player->play();
}

void VideoManager::pause()
{
    player->pause();
}

void VideoManager::stop()
{
    player->stop();
}

void VideoManager::playPrevious()
{
    if (playlist.isEmpty()) return;
    
    if (currentIndex > 0) {
        currentIndex--;
    } else {
        currentIndex = playlist.size() - 1; // 循环到最后一首
    }
    
    loadAndPlay(playlist[currentIndex]);
}

void VideoManager::playNext()
{
    if (playlist.isEmpty()) return;
    
    if (currentIndex < playlist.size() - 1) {
        currentIndex++;
    } else {
        currentIndex = 0; // 循环到第一首
    }
    
    loadAndPlay(playlist[currentIndex]);
}

bool VideoManager::isPlaying() const
{
    return player->playbackState() == QMediaPlayer::PlayingState;
}

qint64 VideoManager::position() const
{
    return player->position();
}

qint64 VideoManager::duration() const
{
    return player->duration();
}

void VideoManager::setPosition(qint64 position)
{
    player->setPosition(position);
}

void VideoManager::setVideoWidget(QVideoWidget *widget)
{
    videoWidget = widget;
    player->setVideoOutput(widget);
}

QString VideoManager::currentFileName() const
{
    if (currentIndex >= 0 && currentIndex < playlist.size()) {
        return playlist[currentIndex];
    }
    return QString();
}

void VideoManager::onPositionChanged(qint64 position)
{
    emit VideoManager::positionChanged(position, player->duration());
}

void VideoManager::onDurationChanged(qint64 duration)
{
    emit VideoManager::positionChanged(player->position(), duration);
}

void VideoManager::onStateChanged(QMediaPlayer::PlaybackState state)  // 修复：使用PlaybackState
{
    emit playbackStateChanged(state == QMediaPlayer::PlayingState);
}