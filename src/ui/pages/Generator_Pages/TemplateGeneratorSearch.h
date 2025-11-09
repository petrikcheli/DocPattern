#ifndef TEMPLATEGENERATORSEARCH_H
#define TEMPLATEGENERATORSEARCH_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QDebug>
#include <QLabel>

#include "database.h"
#include "PythonWorker.h"
#include "TemplateData.h"
#include "TemplateFile.h"
#include "TemplateRule.h"


class TemplateGeneratorSearch : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateGeneratorSearch(std::shared_ptr<DataBase> db,
                                     std::shared_ptr<PythonWorker> pythonWorker,
                                     QWidget *parent = nullptr);

signals:
    void selectTemplate(int idTemplate);

private:
    void updateTemplates(const std::vector<TemplateData> &vecTemplateData);
    std::vector<TemplateData> filterByName(const std::vector<TemplateData> &data,
                                           const QString &query);

private:
    QTextEdit *searchLine;
    QPushButton *searchButton;
    QListWidget *listTemplate;

    std::shared_ptr<DataBase> db;
    std::shared_ptr<PythonWorker> pythonWorker;

    std::vector<TemplateData> vecTemplateData;
};

#endif // TEMPLATEGENERATORSEARCH_H
