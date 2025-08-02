#include "audiomanager.h"
#include <QDir>
#include <QFile>
#include <QRegularExpression>  // Qt6推荐使用的正则类
#include <QMediaMetaData>
#include <QImage>
#include <QDebug>

AudioManager::AudioManager(QObject *parent) : QObject(parent)
{
    m_player = new QMediaPlayer(this);

    // 关联播放器信号（Qt6中信号参数需要显式声明）
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &AudioManager::onPositionChanged);

    connect(m_player, &QMediaPlayer::durationChanged,
            this, [=](qint64 duration) {
                emit positionChanged(m_player->position(), duration);
            });
}

// 扫描本地音频文件
QStringList AudioManager::scanAudioFiles(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        qWarning() << "目录不存在:" << dirPath;
        return {};
    }

    // 支持的音频格式
    const QStringList filters = {"*.mp3", "*.wav", "*.flac", "*.m4a", "*.ogg", "*.aac"};
    dir.setNameFilters(filters);

    // 获取文件列表（仅文件名，用于显示）
    const QStringList fileNames = dir.entryList(QDir::Files | QDir::Readable);
    emit audioListUpdated(fileNames);

    return fileNames;
}

// 加载并播放音频文件
void AudioManager::loadAndPlay(const QString &filePath)
{
    if (filePath.isEmpty()) return;

    // 设置媒体源（Qt6中QUrl::fromLocalFile必须显式调用）
    m_player->setSource(QUrl::fromLocalFile(filePath));
    qDebug()<<filePath;
    m_player->play();

    // 加载歌词
   // loadLyric(filePath);
   qDebug() << "loadAndPlay called";
    // 提取封面
    QImage cover = extractCover(filePath);
    if (!cover.isNull()) {
        emit coverLoaded(cover);
    }
}

// 加载歌词
bool AudioManager::loadLyric(const QString &audioFilePath)
{
    // 生成歌词文件路径（与音频同目录、同名，后缀为.lrc）
    QString lyricPath = audioFilePath;
    const QRegularExpression regex("\\.(mp3|wav|flac|m4a|ogg|aac)$", QRegularExpression::CaseInsensitiveOption);
    lyricPath.replace(regex, ".lrc");

    QFile file(lyricPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "未找到歌词文件:" << lyricPath;
        m_lyricMap.clear();
        m_lyricTexts.clear();
        emit lyricLoaded({});
        return false;
    }

    // 解析LRC内容
    const QString content = QString::fromUtf8(file.readAll());  // Qt6推荐使用fromUtf8
    m_lyricMap = parseLRC(content);
    m_lyricTexts = m_lyricMap.values();  // 提取歌词文本列表
    emit lyricLoaded(m_lyricTexts);

    return true;
}

// 解析LRC歌词（Qt6使用QRegularExpression）
LyricMap AudioManager::parseLRC(const QString &content)
{
    LyricMap lyricMap;

    // 匹配LRC时间标签的正则表达式（格式：[mm:ss.zz]）
    const QRegularExpression regex("\\[(\\d+):(\\d+\\.\\d+)\\](.*)");
    const QStringList lines = content.split("\n", Qt::SkipEmptyParts);  // Qt6使用Qt::SkipEmptyParts

    for (const QString &line : lines) {
        const QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch()) {
            // 提取分钟、秒和歌词
            const int minute = match.captured(1).toInt();
            const double second = match.captured(2).toDouble();
            const QString lyric = match.captured(3).trimmed();

            if (lyric.isEmpty()) continue;

            // 转换为毫秒时间戳
            const qint64 totalMs = static_cast<qint64>(minute * 60 * 1000 + second * 1000);
            lyricMap.insert(totalMs, lyric);
        }
    }

    return lyricMap;
}

// 提取音频封面（Qt6版本）
QImage AudioManager::extractCover(const QString &filePath)
{
    Q_UNUSED(filePath);  // 实际项目中需替换为真实提取逻辑

    // 注意：Qt6的QMediaPlayer提取封面元数据需要配合QMediaMetaDataReader
    // 这里简化处理，返回一个默认封面（实际使用时建议用TagLib库）
    QImage cover(300, 300, QImage::Format_RGB32);
    cover.fill(Qt::darkGray);  // 灰色占位封面
    return cover;
}

// 播放/暂停切换
void AudioManager::togglePlayPause()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else if (m_player->playbackState() == QMediaPlayer::PausedState) {
        m_player->play();
    }
}

// 停止播放
void AudioManager::stop()
{
    m_player->stop();
}

// 设置播放进度
void AudioManager::setPosition(qint64 position)
{
    if (position >= 0 && position <= m_player->duration()) {
        m_player->setPosition(position);
    }
}

// 处理播放进度变化（同步歌词）
void AudioManager::onPositionChanged(qint64 position)
{
    // 发送进度信号给UI
    emit positionChanged(position, m_player->duration());

    // 查找当前歌词索引
    if (m_lyricMap.isEmpty()) return;

    // Qt6中QMap的upperBound用法不变
    auto it = m_lyricMap.upperBound(position);
    if (it != m_lyricMap.begin()) {
        --it;
        const int index = m_lyricTexts.indexOf(it.value());
        if (index != -1) {
            emit currentLyricIndexChanged(index);
        }
    }
}

// 获取歌词文本列表
QStringList AudioManager::getLyricTexts() const
{
    return m_lyricTexts;
}
