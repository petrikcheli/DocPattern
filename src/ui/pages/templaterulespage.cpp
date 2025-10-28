#include "TemplateRulesPage.h"

TemplateRulesPage::TemplateRulesPage(QWidget *parent)
    : QWidget(parent)
{
    headLabel = new QLabel("Что хотите сделать с этим выделенным текстом?");
    selectTextLabel = new QLabel("Тест");

    ruleNameEdit = new QLineEdit(this);
    ruleNameEdit->setPlaceholderText("Введите название правила");

    mainFunctionBox = new QComboBox();
    mainFunctionBox->addItems({"Функция 1", "Функция 2", "Функция 3", "Функция 4"});

    caseLabel = new QLabel("Выберите падеж:");
    caseFunctionBox = new QComboBox();
    caseFunctionBox->addItems({"Именительный", "Родительный", "Дательный", "Винительный", "Творительный", "Предложный"});

    // Скрываем выбор падежа по умолчанию
    caseLabel->setVisible(true);
    caseFunctionBox->setVisible(true);

    connect(mainFunctionBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TemplateRulesPage::onMainFunctionChanged);

    saveButton = new QPushButton("Сохранить");
    backButton = new QPushButton("Назад");

    connect(saveButton, &QPushButton::clicked, this, &TemplateRulesPage::onSaveClicked);
    connect(backButton, &QPushButton::clicked, this, &TemplateRulesPage::backClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headLabel);
    mainLayout->addWidget(selectTextLabel);
    mainLayout->addWidget(ruleNameEdit);
    mainLayout->addWidget(mainFunctionBox);
    mainLayout->addWidget(caseLabel);
    mainLayout->addWidget(caseFunctionBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);

    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void TemplateRulesPage::onMainFunctionChanged(int index)
{
    // Например: если выбрана первая функция — показать выбор падежа
    bool needCase = (index == 0); // допустим, первая функция требует падеж
    caseLabel->setVisible(needCase);
    caseFunctionBox->setVisible(needCase);
}

void TemplateRulesPage::onSaveClicked()
{
    // тут нужно сохрантить
    // int id;
    // std::string name; +
    // std::string function_name; +
    // std::string additional_function; +
    // std::string text_to_replace;
    // int text_position;
    // int template_id;
    if(ruleNameEdit->text().isEmpty()){
        qDebug() << "Не ввел имя";
        return;
    }
    if(mainFunctionBox->currentText().isEmpty()){
        qDebug() << "Пользователь не выбрал функцию";
        return;
    }
    if(mainFunctionBox->currentIndex() == 0){
        if(caseFunctionBox->currentText().isEmpty()){
            qDebug() << "Пользователь не выбрал падеж";
            return;
        }
        rule->additional_function = caseFunctionBox->currentText().toStdString();
    }

    rule->name = ruleNameEdit->text().toStdString();
    rule->function_name = mainFunctionBox->currentText().toStdString();


    emit saveClicked();
}

void TemplateRulesPage::setRule(std::shared_ptr<TemplateRule> r)
{
    rule = r;
    selectTextLabel->setText(QString::fromStdString(rule->text_to_replace));

    if(!rule->name.empty()){
        ruleNameEdit->setText(QString::fromStdString(rule->name));
        mainFunctionBox->setCurrentText(QString::fromStdString(rule->function_name));
        caseFunctionBox->setCurrentText(QString::fromStdString(rule->additional_function));
    }
}
