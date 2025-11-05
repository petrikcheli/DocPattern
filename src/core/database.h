#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite_modern_cpp.h>
#include "TemplateRule.h"
#include "TemplateData.h"
#include "TemplateFile.h"

class DataBase
{
public:
    DataBase(const std::string& filename);

    int addTemplate(const std::string &name, const std::string &directory);
    void addTemplateFile(const std::string &filename, int template_id);
    void addRule(const TemplateRule &rule);

    std::vector<TemplateData> getTemplates();
    std::vector<TemplateFile> getTemplateFiles(int template_id);
    std::vector<TemplateRule> getTemplateRules(int template_id);

private:
    sqlite::database db_;
    void init_tables();
};


