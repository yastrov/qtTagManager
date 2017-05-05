#include "src/settingshelper.h"

namespace SettingsNames {
static const QString MainWindowGroup = "MainWindow";
static const QString MainWindowSize = "size";
static const QString MainWindowPosition = "position";
static const QSize MainWindowSizeDefault = QSize(800, 600);
static const QPoint MainWindowPositionDefault = QPoint(0,0);

static const QString FontGroup = "Font";
static const QString FontSize = "size";
static const QString FontFamily = "family";
static const QString FontFamilyDefault = "Arial";
static const int     FontSizeDefault = 16;

static const QString FileGroup = "Files";
static const QString FileLast = "LastFilename";
}

SettingsHelper::SettingsHelper()
{
#ifdef MYPREFIX_DEBUG
    qDebug()<< "Settings ctor";
#endif
    QString fname;
#if defined(Q_OS_WIN)
    if(QCoreApplication::applicationDirPath().contains("Program Files")) {
        fname = QString("%1/%2.ini").arg(QDir::currentPath()).arg(QCoreApplication::applicationName());
    } else {
        fname = QString("%1/%2.ini").arg(QCoreApplication::applicationDirPath()).arg(QCoreApplication::applicationName());
    }
#elif defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
    fname = QString("%1/.config/%2.ini").arg(QDir::homePath()).arg(QCoreApplication::applicationName());
#endif

#ifdef MYPREFIX_DEBUG
    qDebug()<< "Settings File name: " << fname;
#endif
    createSettings(fname);
}

SettingsHelper::~SettingsHelper()
{
#ifdef MYPREFIX_DEBUG
    qDebug()<< "Settings dtor";
#endif
    _settings->deleteLater();
}

void SettingsHelper::createSettings(const QString &fileName)
{
#ifdef MYPREFIX_DEBUG
    qDebug()<< "SettingsHelper::createSettings";
#endif
//    if(_settings) {
//        _settings->deleteLater();
//    }
    if (!fileName.isEmpty()) {
        _settings = new QSettings(fileName, QSettings::IniFormat);
    } else {
        _settings = new QSettings(QSettings::IniFormat,
                                  QSettings::UserScope,
                                  QCoreApplication::organizationName(),
                                  QCoreApplication::applicationName());
    }
}

void SettingsHelper::saveMainWindowPosition(const QSize &size, const QPoint& pos)
{
    _settings->beginGroup(SettingsNames::MainWindowGroup);
    _settings->setValue(SettingsNames::MainWindowSize, size);
    _settings->setValue(SettingsNames::MainWindowPosition, pos);
    _settings->endGroup();
}

void SettingsHelper::loadMainWindowPosition(QSize &size, QPoint& pos)
{
    _settings->beginGroup(SettingsNames::MainWindowGroup);
    size = _settings->value(SettingsNames::MainWindowSize, SettingsNames::MainWindowSizeDefault).toSize();
    pos = _settings->value(SettingsNames::MainWindowPosition, SettingsNames::MainWindowPositionDefault).toPoint();
    _settings->endGroup();
}

void SettingsHelper::saveFontSize(int size)
{
    _settings->beginGroup(SettingsNames::FontGroup);
    _settings->setValue(SettingsNames::FontSize, size);
    _settings->endGroup();
}

int SettingsHelper::loadFontSize()
{
    _settings->beginGroup(SettingsNames::FontGroup);
    const int size = _settings->value(SettingsNames::FontSize, SettingsNames::FontSizeDefault).toInt();
    _settings->endGroup();
    return size;
}

void SettingsHelper::saveFontFamily(const QString& fontFamily)
{
    _settings->beginGroup(SettingsNames::FontGroup);
    _settings->setValue(SettingsNames::FontFamily, fontFamily);
    _settings->endGroup();
}

QString SettingsHelper::loadFontFamily()
{
    _settings->beginGroup(SettingsNames::FontGroup);
    const QString fontFamily = _settings->value(SettingsNames::FontFamily, SettingsNames::FontFamilyDefault).toString();
    _settings->endGroup();
    return fontFamily;
}

void SettingsHelper::saveFont(int size, const QString& fontFamily)
{
    _settings->beginGroup(SettingsNames::FontGroup);
    _settings->setValue(SettingsNames::FontSize, size);
    _settings->setValue(SettingsNames::FontFamily, fontFamily);
    _settings->endGroup();
}

void SettingsHelper::loadFont(int& size, QString& fontFamily)
{
    _settings->beginGroup(SettingsNames::FontGroup);
    fontFamily = _settings->value(SettingsNames::FontFamily, SettingsNames::FontFamilyDefault).toString();
    size = _settings->value(SettingsNames::FontSize, SettingsNames::FontSizeDefault).toInt();
    _settings->endGroup();
}

QString SettingsHelper::loadFontStyleSheet()
{
#ifdef MYPREFIX_DEBUG
    qDebug()<< "SettingsHelper::loadFontStyleShee";
#endif
    QString fontFamily;
    int fontSize;
    loadFont(fontSize, fontFamily);
    return QString("font-size:%1pt;font-family:%2;").arg(fontSize).arg(fontFamily);
}

void SettingsHelper::saveLastFileName(const QString &fileName)
{
    _settings->beginGroup(SettingsNames::FileGroup);
    _settings->setValue(SettingsNames::FileLast, fileName);
    _settings->endGroup();
}

QString SettingsHelper::loadLastFileName()
{
    QString fname;
    _settings->beginGroup(SettingsNames::FileGroup);
    fname = _settings->value(SettingsNames::FileLast, "").toString();
    _settings->endGroup();
    return fname;
}
