#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStringList>
#include <QDir>

class VideoManager : public QObject
{
    Q_OBJECT

public:
    explicit VideoManager(QObject *parent = nullptr);
    ~VideoManager();
    
    // 扫描视频文件
    QStringList scanVideoFiles(const QString &directory);
    
    // 设置播放列表
    void setPlaylist(const QStringList &files, const QString &basePath);
    
    // 播放控制
    void loadAndPlay(const QString &filePath);
    void play();
    void pause();
    void stop();
    void playNext();
    void playPrevious();
    
    // 获取播放状态
    bool isPlaying() const;
    qint64 position() const;
    qint64 duration() const;
    void setPosition(qint64 position);
    
    // 设置视频输出部件
    void setVideoWidget(QVideoWidget *videoWidget);
    
    // 获取当前播放文件名
    QString currentFileName() const;

signals:
    void playbackStateChanged(bool isPlaying);
    void positionChanged(qint64 position, qint64 duration);
    void videoTitleChanged(const QString &title);

private slots:
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onStateChanged(QMediaPlayer::PlaybackState state);  // 修复：使用PlaybackState

private:
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QStringList playlist;
    QString basePath;
    int currentIndex;
};

#endif // VIDEOMANAGER_H
