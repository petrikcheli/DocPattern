#ifndef RULESMANAGER_H
#define RULESMANAGER_H

#include <string>
#include <memory>
#include <vector>
#include "TemplateRule.h"

class RulesManager
{
public:
    RulesManager();
    void addRule(std::shared_ptr<TemplateRule> rule);
    void addNewRule();
    void deleteRule(std::shared_ptr<TemplateRule> rule);
    void createNewRule(std::shared_ptr<TemplateRule> rule);
    void cancelNewRule();
    std::vector<std::shared_ptr<TemplateRule>>& geRule(int index);

public:
    std::shared_ptr<TemplateRule> selectRule;
    // \n Хранит структуры правил, чтобы при нажатии кнопки сохранить занести все правила в базу данных
    // \n не забыть еще дать id шаблона кажлому правилу
    std::vector<std::shared_ptr<TemplateRule>> rules;

};

#endif // RULESMANAGER_H
