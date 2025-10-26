#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite_modern_cpp.h>

struct TemplateData {
    int id;
    std::string name;
    std::string directory;
};

struct TemplateFile {
    int id;
    std::string filename;
    int template_id;
};

struct TemplateRule {
    int id;
    std::string name;
    std::string function_name;
    std::string additional_function;
    std::string text_to_replace;
    int text_position;
    int template_id;
};

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


