#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#ifdef MYPREFIX_DEBUG
#include <QDebug>
#endif
#include <QApplication>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPoint>

class SettingsHelper
{
public:
    explicit SettingsHelper();
    virtual ~SettingsHelper();
    void sync() { _settings->sync(); }
    // Main Window
    void saveMainWindowPosition(const QSize &size, const QPoint& pos);
    void loadMainWindowPosition(QSize &size, QPoint& pos);
    // last filename
    void saveLastFileName(const QString &fileName);
    QString loadLastFileName();
    // Font
    void saveFontSize(int size);
    int loadFontSize();
    void saveFontFamily(const QString& fontFamily);
    QString loadFontFamily();
    void saveFont(int size, const QString& fontFamily);
    void loadFont(int& size, QString& fontFamily);
    // Helper
    QString loadFontStyleSheet();

private:
    void createSettings(const QString &fileName);
    QSettings* _settings;
};

#endif // SETTINGSHELPER_H
