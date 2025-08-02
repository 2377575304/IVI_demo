#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QImage>
#include <QMap>
#include <QStringList>

// 定义歌词映射类型（时间戳 -> 歌词文本）
using LyricMap = QMap<qint64, QString>;

class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = nullptr);

    // 扫描本地音频文件
    QStringList scanAudioFiles(const QString &dirPath);

    // 加载并播放指定音频文件
    void loadAndPlay(const QString &filePath);

    // 获取歌词文本列表
    QStringList getLyricTexts() const;

signals:
    // 音频列表更新
    void audioListUpdated(const QStringList &fileNames);

    // 歌词加载完成
    void lyricLoaded(const QStringList &lyrics);

    // 当前歌词索引变化（用于高亮）
    void currentLyricIndexChanged(int index);

    // 封面加载完成
    void coverLoaded(const QImage &cover);

    // 播放进度变化
    void positionChanged(qint64 currentPos, qint64 totalPos);

public slots:
    // 播放/暂停切换
    void togglePlayPause();

    // 停止播放
    void stop();

    // 设置播放进度
    void setPosition(qint64 position);

private slots:
    // 处理播放进度变化
    void onPositionChanged(qint64 position);

private:
    // 加载歌词文件
    bool loadLyric(const QString &audioFilePath);

    // 解析LRC歌词（Qt6使用QRegularExpression）
    LyricMap parseLRC(const QString &content);

    // 提取音频封面
    QImage extractCover(const QString &filePath);

private:
    QMediaPlayer *m_player = nullptr;  // 媒体播放器
    LyricMap m_lyricMap;               // 歌词时间映射
    QStringList m_lyricTexts;          // 歌词文本列表
};

#endif // AUDIOMANAGER_H
