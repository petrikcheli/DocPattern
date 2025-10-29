#ifndef TEMPLATERULE_H
#define TEMPLATERULE_H

#include <string>

struct TemplateRule {
    int id;
    std::string name;
    std::string function_name;
    std::string additional_function;
    std::string text_to_replace;
    int text_position;
    int template_id;
};

#endif // TEMPLATERULE_H
