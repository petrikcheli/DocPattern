#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite_modern_cpp.h>
#include "TemplateRule.h"
#include "TemplateData.h"
#include "TemplateFile.h"

class database
{
public:
    database(const std::string& filename);

    int addTemplate(const std::string &name, const std::string &directory);
    void addTemplateFile(const std::string &filename, int template_id);
    void addRule(const TemplateRule &rule);

    std::vector<TemplateData> getTemplates();

private:
    sqlite::database db_;
    void init_tables();
};


