#include "database.h"

database::database(const std::string &filename) : db_(filename)
{
    init_tables();
}

int database::addTemplate(const std::string &name, const std::string &directory)
{
    db_ << "INSERT INTO templates (name, directory) VALUES (?, ?);" << name << directory;
    int id = 0;
    db_ << "SELECT last_insert_rowid();" >> id;
    return id;
}

void database::addTemplateFile(const std::string &filename, int template_id)
{
    db_ << "INSERT INTO template_files (filename, template_id) VALUES (?, ?);"
        << filename << template_id;
}

void database::addRule(const TemplateRule &rule)
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

std::vector<TemplateData> database::getTemplates()
{
    std::vector<TemplateData> result;
    db_ << "SELECT id, name, directory FROM templates;"
        >> [&](int id, std::string name, std::string dir) {
              result.push_back({id, name, dir});
          };
    return result;
}

void database::init_tables()
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
