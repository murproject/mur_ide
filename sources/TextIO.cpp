#include "TextIO.hxx"

#include <QDirIterator>
#include <QFileDialog>
#include <QTextStream>

namespace Ide::IO {

namespace FromDialog {

const QString selectSavFileUrl(const QString &title)
{
    auto home_path = QDir::home().path();
    auto ret = QFileDialog::getSaveFileName(nullptr, title, home_path);
    return ret;
}

const QString selectOpenFileUrl(const QString &filter, const QString &title) {
  return QFileDialog::getOpenFileName(nullptr, title, nullptr, filter);
}
} 

namespace Read {

QJsonObject jsonFromUrl(const QString &url) {
  QFile file(url);

  if (file.open(QIODevice::ReadOnly)) {
    return QJsonDocument::fromJson(file.readAll()).object();
  }

  return QJsonObject();
}

QString textFromUrl(const QString &url)
{
    QString text;
    QFile file(url);

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        text = file.readAll();
        file.close();
    }

    return text;
}
} 

namespace Write {

bool textToFile(const QString &text, const QString &url)
{
    QFile file(url);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(false);
        stream << text.toUtf8();
        file.close();
        return true;
    }
    return false;
}
} 

QStringList fileNamesFromDir(const QString &dir, QStringList filters, FileSuffix option)
{
    QDirIterator it(dir, filters, QDir::Files);
    QStringList names;

    if (option == FileSuffix::On) {
        while (it.hasNext()) {
            it.next();
            names.append(it.fileInfo().fileName());
        }
    } else {
        while (it.hasNext()) {
            it.next();
            names.append(it.fileInfo().baseName());
        }
    }

    return names;
}

QStringList fileUrlsFromDir(const QString &dir, const QStringList &filters, FileSuffix option)
{
    QDirIterator it(dir, filters, QDir::Files);
    QStringList urls;
    QString path = dir + "/";

    if (option == FileSuffix::On) {
        while (it.hasNext()) {
            urls.append(it.next());
        }
    } else {
        while (it.hasNext()) {
            it.next();
            urls.append(path + it.fileInfo().baseName());
        }
    }

    return urls;
}

QString fileNameFromUrl(const QString &url, FileSuffix option)
{
    if (!fileExists(url)) {
        return "";
    }

    QFileInfo file(url);
    if (option == FileSuffix::On) {
        return file.fileName();
    } else {
        return file.baseName();
    }
}

bool fileExists(const QString &url)
{
    QFile file(url);
    return file.exists();
}

bool directoryExists(const QString &url)
{
    QFileInfo dir(url);
    return dir.exists() && dir.isDir();
}

} 
