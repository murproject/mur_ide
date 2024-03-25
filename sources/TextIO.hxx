#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

namespace Ide::IO {

enum class FileSuffix { On, Off };

namespace FromDialog {
const QString selectSavFileUrl(const QString &title = "Save File");
const QString selectOpenFileUrl(const QString &filter, const QString &title = "Open File");
} 

namespace Read {
QJsonObject jsonFromUrl(const QString &);
QString textFromUrl(const QString &);
} 

namespace Write {
bool textToFile(const QString &text, const QString &url);
}

QStringList fileUrlsFromDir(const QString &dir, const QStringList &filters, FileSuffix);
QStringList fileNamesFromDir(const QString &dir, QStringList filters, FileSuffix);
QString fileNameFromUrl(const QString &, FileSuffix);
bool fileExists(const QString &);
bool directoryExists(const QString &);
} 
