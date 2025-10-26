#ifndef TEMPLATEEDITPAGE_H
#define TEMPLATEEDITPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QListWidget>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include "PythonWorker.h"

namespace fs = std::filesystem;

class TemplateEditPage : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateEditPage(QWidget *parent = nullptr);
    void setFilePath(const QString &path);



private:
    QString convertDocxToHtml();
    // создается папка TemplatesFio/{{fileName}}
    // потом можно переименовать в название шаблона
    // еще нужно сохранять данные можно в бд допустим
    void copyFileToNewDir();


signals:

private:
    std::unique_ptr<PythonWorker> pythonWorker;

    QPushButton *createRuleButton;
    QPushButton *saveButton;
    QTextBrowser *textBrowserFile;
    QListWidget *listRules;

    const std::string dirTemplates = "TemplatesFiles";
    QString filePath;
    QString fileName;
    fs::path subDir;
};

#endif // TEMPLATEEDITPAGE_H
