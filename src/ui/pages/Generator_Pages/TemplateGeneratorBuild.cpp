#include "TemplateGeneratorBuild.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QAbstractItemView>

TemplateGeneratorBuild::TemplateGeneratorBuild(std::shared_ptr<DataBase> db, QWidget *parent)
    : QWidget{parent}
{
    this->db = db;

    createButton = new QPushButton("Создать", this);
    backButton = new QPushButton("Назад", this);
    listRules = new QListWidget(this);
    listRules->setSelectionMode(QAbstractItemView::NoSelection);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(createButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(listRules);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    connect(backButton, &QPushButton::clicked, this, &TemplateGeneratorBuild::backButtonClicked);
    connect(createButton, &QPushButton::clicked, this, &TemplateGeneratorBuild::onCreateButtonClicked);
}

void TemplateGeneratorBuild::setTemplateId(int idTemplate)
{
    rules.clear();
    rules = db->getTemplateRules(idTemplate);
    updateListRules();
}

void TemplateGeneratorBuild::onCreateButtonClicked()
{
    // тут должна быть логика того, как мы обрабатываем то что ввел пользователь
    // сначала логика проверки того, что пользовал что-то ввел
    // Если все окей то нужно обработать функцию
    // Если это функция fio то нужно воспользоваться python, чтобы он сделал падеж
    // Как все готово необходимо в docx найти это место которое сначала нужно удалить а потом вставить туда наши результаты
}

void TemplateGeneratorBuild::updateListRules()
{
    listRules->clear();
    for(int i = 0; i < rules.size(); ++i){
        QWidget *itemWidget = new QWidget();

        QLabel *nameRule = new QLabel(QString::fromStdString(rules.at(i).name));
        QTextEdit *inputText = new QTextEdit();
        inputText->setPlaceholderText("Введите текст");
        inputText->setFixedHeight(40);

        QVBoxLayout *layout = new QVBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);
        layout->addWidget(nameRule);
        layout->addWidget(inputText);

        QListWidgetItem *item = new QListWidgetItem(listRules);

        itemWidget->setFixedHeight(80);
        item->setSizeHint(QSize(0, itemWidget->size().height()));

        listRules->addItem(item);
        listRules->setItemWidget(item, itemWidget);

    }
}
