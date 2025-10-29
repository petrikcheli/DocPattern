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
#include <QLabel>

#include <filesystem>
#include <vector>
#include "PythonWorker.h"
#include "RulesManager.h"


namespace fs = std::filesystem;

class TemplateEditPage : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateEditPage(std::shared_ptr<RulesManager> rulesManager, QWidget *parent = nullptr);
    void setFilePath(const QString &path);
    //std::vector<std::shared_ptr<TemplateRule>>& getRules(){ return rules; };


private:
    QString convertDocxToHtml();
    /// создается папка TemplatesFio/{{fileName}}
    // потом можно переименовать в название шаблона
    // еще нужно сохранять данные можно в бд допустим
    void copyFileToNewDir();

public slots:
    /// сохраняет текст который выбрал пользователь
    // Есть проблема с тем, что нужно занести в базу данных сам Template а потом еще и ввести id во все Struct TemplateRules
    void saveSelectedText();
    /// переход на страницу создания правила
    void createRuleButtonSlot();
    /// удаляется папка директория /{{fileName}}
    void cancelCreation();
    /// обновляет список правил которые создал пользователь
    void updateRuleList();

signals:
    void createRuleButtonClicked();
    /// сигнал когда нажата кнопка назад
    // нужна для того, чтобы удалить данные которые не нужно сохранять а также возвращает на страницу выбора файла
    void backButtonClicked();
    // /// Сигнализирует о том, что выбрано новое правило
    // void ruleSelected(std::shared_ptr<TemplateRule> rule);
    /// сигнал для обновления списка
    void editRuleRequested(std::shared_ptr<TemplateRule> rule);

private:
    std::unique_ptr<PythonWorker> pythonWorker;


    QPushButton *saveButton;
    QPushButton *createRuleButton;
    //TODO: сделать черновик при выходе
    QPushButton *backButton;
    QTextBrowser *textBrowserFile;
    QListWidget *listRules;

    const std::string dirTemplates = "TemplatesFiles";
    QString filePath;
    QString fileName;
    fs::path subDir;

    QString selectText;
    int startPosSelectText;
    int endPosSelectText;

    std::shared_ptr<RulesManager> rulesManager;
};

#endif // TEMPLATEEDITPAGE_H
