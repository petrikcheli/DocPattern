#include "database.h"

DataBase::DataBase(const std::string &filename) : db_(filename)
{
    init_tables();
}

int DataBase::addTemplate(const std::string &name, const std::string &directory)
{
    db_ << "INSERT INTO templates (name, directory) VALUES (?, ?);" << name << directory;
    int id = 0;
    db_ << "SELECT last_insert_rowid();" >> id;
    return id;
}

void DataBase::addTemplateFile(const std::string &filename, int template_id)
{
    db_ << "INSERT INTO template_files (filename, template_id) VALUES (?, ?);"
        << filename << template_id;
}

void DataBase::addRule(const TemplateRule &rule)
{
    db_ << "INSERT INTO template_rules "
           "(name, function_name, additional_function, text_to_replace, text_position, template_id) "
           "VALUES (?, ?, ?, ?, ?, ?);"
        << rule.name
        << rule.function_name
        << rule.additional_function
        << rule.text_to_replace
        << rule.text_position
        << rule.template_id;
}

std::vector<TemplateData> DataBase::getTemplates()
{
    std::vector<TemplateData> result;
    db_ << "SELECT id, name, directory FROM templates;"
        >> [&](int id, std::string name, std::string dir) {
              result.push_back({id, name, dir});
          };
    return result;
}

std::vector<TemplateFile> DataBase::getTemplateFiles(int template_id)
{
    std::vector<TemplateFile> result;
    db_ << "SELECT id, filename, template_id FROM template_files WHERE template_id = ?;"
        << template_id
        >> [&](int id, std::string filename, int tid) {
              result.push_back({id, filename, tid});
          };
    return result;
}

std::vector<TemplateRule> DataBase::getTemplateRules(int template_id)
{
    std::vector<TemplateRule> result;
    db_ << "SELECT id, name, function_name, additional_function, text_to_replace, text_position, template_id "
           "FROM template_rules WHERE template_id = ?;"
        << template_id
        >> [&](int id, std::string name, std::string fname, std::string addfun,
               std::string text, int pos, int tid) {
              result.push_back({id, name, fname, addfun, text, pos, tid});
          };
    return result;
}



void DataBase::init_tables()
{
    db_ << "CREATE TABLE IF NOT EXISTS templates ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "name TEXT NOT NULL, "
           "directory TEXT NOT NULL"
           ");";

    db_ << "CREATE TABLE IF NOT EXISTS template_files ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "filename TEXT NOT NULL, "
           "template_id INTEGER NOT NULL, "
           "FOREIGN KEY(template_id) REFERENCES templates(id)"
           ");";

    db_ << "CREATE TABLE IF NOT EXISTS template_rules ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "name TEXT NOT NULL, "
           "function_name TEXT NOT NULL, "
           "additional_function TEXT, "
           "text_to_replace TEXT NOT NULL, "
           "text_position INTEGER NOT NULL, "
           "template_id INTEGER NOT NULL, "
           "FOREIGN KEY(template_id) REFERENCES templates(id)"
           ");";
}
