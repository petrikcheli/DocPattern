#ifndef TEMPLATEEDITPAGE_H
#define TEMPLATEEDITPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QListWidget>

class TemplateEditPage : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateEditPage(QWidget *parent = nullptr);
    void setFilePath(const QString &path){ filePath = path; }



private:
    void convertDocxToHtml();
    // создается папка TemplatesFio/{{fileName}}
    // потом можно переименовать в название шаблона
    // еще нужно сохранять данные можно в бд допустим
    void copyFileToNewDir();

signals:

private:
    QPushButton *createRuleButton;
    QPushButton *saveButton;
    QTextBrowser *textBrowserFile;
    QListWidget *listRules;

    QString filePath;
};

#endif // TEMPLATEEDITPAGE_H
