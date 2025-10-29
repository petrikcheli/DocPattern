#include "RulesManager.h"

RulesManager::RulesManager() {}

void RulesManager::addNewRule()
{
    rules.push_back(selectRule);
    selectRule.reset();
}

void RulesManager::createNewRule(std::shared_ptr<TemplateRule> rule)
{
    // selectRule.reset();
    selectRule = rule;
}

void RulesManager::cancelNewRule()
{
    selectRule.reset();
}
