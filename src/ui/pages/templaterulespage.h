#ifndef TEMPLATERULESPAGE_H
#define TEMPLATERULESPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

#include "RulesManager.h"


class TemplateRulesPage : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateRulesPage(std::shared_ptr<RulesManager> rulesManager, QWidget *parent = nullptr);


signals:
    void backClicked();
    void saveClicked();

public slots:
    void onMainFunctionChanged(int index);
    void onSaveClicked();
    //void onBackButtonClicked();
    void setRule();
    void onBackClicked();

private:
    QLabel *headLabel;
    QLabel *selectTextLabel;
    QLineEdit *ruleNameEdit;

    QComboBox *mainFunctionBox;      // Основная функция (4 варианта)
    QComboBox *caseFunctionBox;      // Падеж (появляется при определённой функции)
    QLabel *caseLabel;               // "Выберите падеж:"

    QPushButton *saveButton;
    QPushButton *backButton;

    QString selectText;

    //std::shared_ptr<TemplateRule> rule;
    std::shared_ptr<RulesManager> rulesManager;
};

#endif // TEMPLATERULESPAGE_H
