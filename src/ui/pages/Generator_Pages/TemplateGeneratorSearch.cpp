#include "TemplateGeneratorSearch.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

TemplateGeneratorSearch::TemplateGeneratorSearch(std::shared_ptr<DataBase> db,
                                                 std::shared_ptr<PythonWorker> pythonWorker,
                                                 QWidget *parent)
    : QWidget{parent}
{
    this->db = db;
    this->pythonWorker = pythonWorker;

    searchLine = new QTextEdit(this);
    searchButton = new QPushButton("Найти", this);
    listTemplate = new QListWidget(this);

    //Layout в который входит панель поиска
    auto topLayout = new QHBoxLayout();
    searchLine->setMaximumHeight(30);
    searchButton->setMaximumHeight(30);
    topLayout->addWidget(searchLine, 3);
    topLayout->addWidget(searchButton, 1);

    //Главынй layout
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(listTemplate);

    vecTemplateData = this->db->getTemplates();

    updateTemplates(vecTemplateData);

    setLayout(mainLayout);

    connect(searchLine, &QTextEdit::textChanged, this, [this]() {
        auto filtered = filterByName(this->vecTemplateData, searchLine->toPlainText());
        updateTemplates(filtered);
    });
}

void TemplateGeneratorSearch::updateTemplates(const std::vector<TemplateData> &vecTemplateData)
{
    listTemplate->clear();
    for (int i = 0; i < vecTemplateData.size(); ++i){
        QWidget *itemWidget = new QWidget();

        QLabel *idTemplate = new QLabel(QString::number(vecTemplateData.at(i).id), itemWidget);
        idTemplate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        QLabel *nameTemplate = new QLabel(QString::fromStdString(vecTemplateData.at(i).name), itemWidget);
        QPushButton *selectButton = new QPushButton("Выбрать", itemWidget);

        QHBoxLayout *mainWidgetLayout = new QHBoxLayout(itemWidget);
        mainWidgetLayout->addWidget(idTemplate, 1);
        mainWidgetLayout->addWidget(nameTemplate, 7);
        mainWidgetLayout->addWidget(selectButton, 2);

        QListWidgetItem *item = new QListWidgetItem(listTemplate);
        item->setSizeHint(itemWidget->sizeHint()); // <-- Важно

        listTemplate->addItem(item);
        listTemplate->setItemWidget(item, itemWidget);

        int idTemplateLambda = vecTemplateData.at(i).id;

        connect(selectButton, &QPushButton::clicked, this, [this, idTemplateLambda](){
            emit selectTemplate(idTemplateLambda);
        });
    }
}

std::vector<TemplateData> TemplateGeneratorSearch::filterByName(const std::vector<TemplateData> &data, const QString &query)
{
    std::vector<TemplateData> result;
    if (query.isEmpty())
        return data;

    std::string q = query.toStdString();
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    for (const auto &item : data)
    {
        std::string name = item.name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name.find(q) != std::string::npos)
            result.push_back(item);
    }
    return result;
}

