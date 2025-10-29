#include "TemplateRulesPage.h"

TemplateRulesPage::TemplateRulesPage(std::shared_ptr<RulesManager> rulesManager, QWidget *parent)
    : QWidget(parent)
{
    this->rulesManager = rulesManager;

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
    connect(backButton, &QPushButton::clicked, this, &TemplateRulesPage::onBackClicked);

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
        //rulesManager->selectRule
        rulesManager->selectRule->additional_function = caseFunctionBox->currentText().toStdString();
    }

    rulesManager->selectRule->name = ruleNameEdit->text().toStdString();
    rulesManager->selectRule->function_name = mainFunctionBox->currentText().toStdString();

    rulesManager->addNewRule();

    emit saveClicked();
}

void TemplateRulesPage::setRule()
{
    selectTextLabel->setText(QString::fromStdString(rulesManager->selectRule->text_to_replace));

    if(!rulesManager->selectRule->name.empty()){
        ruleNameEdit->setText(QString::fromStdString(rulesManager->selectRule->name));
        mainFunctionBox->setCurrentText(QString::fromStdString(rulesManager->selectRule->function_name));
        caseFunctionBox->setCurrentText(QString::fromStdString(rulesManager->selectRule->additional_function));
    }
}

void TemplateRulesPage::onBackClicked()
{
    rulesManager->cancelNewRule();
    //rule.reset();
}
