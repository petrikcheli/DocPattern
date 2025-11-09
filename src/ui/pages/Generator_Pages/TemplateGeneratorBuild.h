#ifndef TEMPLATEGENERATORBUILD_H
#define TEMPLATEGENERATORBUILD_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include <vector>

#include "database.h"
// #include "TemplateData.h"
// #include "TemplateFile.h"
// #include "TemplateRule.h"

class TemplateGeneratorBuild : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateGeneratorBuild(std::shared_ptr<DataBase> db, QWidget *parent = nullptr);

public slots:
    void setTemplateId(int idTemplate);
    void onCreateButtonClicked();

signals:
    void backButtonClicked();
    void createButtonClicked();

private:
    void updateListRules();

private:
    QPushButton *createButton;
    QPushButton *backButton;

    QListWidget *listRules;

    std::shared_ptr<DataBase> db;

    std::vector<TemplateRule> rules;
};

#endif // TEMPLATEGENERATORBUILD_H
