#include "templateeditpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QSplitter>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

TemplateEditPage::TemplateEditPage(std::shared_ptr<RulesManager> rulesManager,
                                   std::shared_ptr<DataBase> db,
                                   QWidget *parent)
    : QWidget{parent}
{
    pythonWorker = std::make_unique<PythonWorker>(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("python"));
    this->rulesManager = rulesManager;
    this->db = db;

    createRuleButton = new QPushButton("Добавить правило", this);
    saveButton = new QPushButton("Сохранить", this);
    backButton = new QPushButton("Назад", this);
    textBrowserFile = new QTextBrowser(this);
    listRules = new QListWidget(this);

    // === Левая часть: текстовое окно (занимает всё место) ===
    auto *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(textBrowserFile);

    // === Правая часть: список правил (узкий столбец) ===
    auto *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(listRules);

    // Можно задать фиксированную ширину
    listRules->setMaximumWidth(150);

    // // === Верхняя часть (левая и правая колонка) ===
    // auto *topLayout = new QHBoxLayout();
    // topLayout->addLayout(leftLayout, 3);   // вес 3 → занимает больше места
    // topLayout->addLayout(rightLayout, 1);  // вес 1 → уже

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(textBrowserFile);
    splitter->addWidget(listRules);

    // Настраиваем начальные пропорции (например, 3 к 1)
    QList<int> sizes;
    sizes << 600 << 200; // ширина каждой панели в пикселях
    splitter->setSizes(sizes);


    // === Нижняя часть: две кнопки по горизонтали ===
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch(); // чтобы кнопки прижались вправо
    bottomLayout->addWidget(createRuleButton);
    bottomLayout->addWidget(saveButton);

    // === Главный layout ===
    // auto *mainLayout = new QVBoxLayout(this);
    // mainLayout->addLayout(topLayout);
    // mainLayout->addLayout(bottomLayout);
    // mainLayout->setStretch(0, 1); // верхняя часть растягивается
    // mainLayout->setStretch(1, 0); // нижняя — фиксированная

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    setLayout(mainLayout);

    // когда все правила подготовленны сохраняет все в базу данных и выходит на домашнюю станицу
    connect(saveButton, &QPushButton::clicked, this, &TemplateEditPage::saveSelectedText);

    // ведет на страницу с созданием правила
    connect(createRuleButton, &QPushButton::clicked, this, &TemplateEditPage::createRuleButtonSlot);

    // будет создавать черновик с недоделанной работой (пока ничего не делает)
    connect(backButton, &QPushButton::clicked, this, &TemplateEditPage::cancelCreation);

    //QTextEdit *editor = new QTextEdit(this);
    //Сначала нужно скопировать куда-то File
    //Потом мне нужно преобразовать его в html
    //и потом выводим

    // copyFileToNewDir();
    // convertDocxToHtml();


    // QFile file(fileName);
    // file.open(QIODevice::ReadOnly);
    // textBrowserFile->setHtml(file.readAll());
}

void TemplateEditPage::setFilePath(const QString &path)
{
    filePath = path;
    qDebug() << "filePath = " << path;
    fileName = filePath.split("/").back();

    copyFileToNewDir();
    QString dirHtml = convertDocxToHtml();


    QFile file(dirHtml);
    file.open(QIODevice::ReadOnly);
    textBrowserFile->setHtml(file.readAll());
}

QString TemplateEditPage::convertDocxToHtml()
{
    //Тут нужно запустить python скрипт
    QString dirDocx = QString::fromStdString(dirTemplates) + "/" + fileName.split('.').first() + "/" + fileName;
    QString dirHtml = QString::fromStdString(dirTemplates) + "/" + fileName.split('.').first() + "/" + fileName.split('.').first() + ".html";

    pythonWorker->runFirstScript({dirDocx, dirHtml});

    return dirHtml;
}

void TemplateEditPage::copyFileToNewDir()
{
    try {
        fs::path baseDir = dirTemplates;
        subDir = baseDir / fileName.split('.').begin()->toStdString();

        fs::create_directories(subDir);

        qDebug() << "Папка создана";

        fs::path source = filePath.toStdString();
        fs::path destination = subDir / fileName.toStdString();

        // qDebug() << "source = " << source.string();
        // qDebug() << "destination = " << destination.string();

        try {
            fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
            qDebug() << "файл скопирован";
        } catch(const fs::filesystem_error &e){
            qDebug() << "Ошибка при создании файла: " << e.what();
        }

    } catch (const fs::filesystem_error &e) {
        qDebug() << "Ошибка при создании папки: " << e.what();
    }

}

void TemplateEditPage::updateUi(){
    // Очистка текстового поля
    textBrowserFile->clear();

    // Очистка списка правил в UI
    listRules->clear();

    // Очистка правил в менеджере
    if (rulesManager) {
        rulesManager->clearRule();  // если этот метод очищает vector<shared_ptr<TemplateRule>>
    }

    // Очистка выделенного текста
    selectText.clear();
    startPosSelectText = 0;
    endPosSelectText = 0;

    // Очистка имени файла и пути
    filePath.clear();
    fileName.clear();
    subDir.clear();

    qDebug() << "UI и данные TemplateEditPage сброшены.";
}

void TemplateEditPage::saveSelectedText()
{
    // тут нужно сохранить все правила и добавить id шаблона
    if (!db) {
        QMessageBox::critical(this, "Ошибка", "База данных не инициализирована.");
        return;
    }

    bool ok;
    QString templateName = QInputDialog::getText(this,
                                                 tr("Создание шаблона"),
                                                 tr("Введите имя шаблона:"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &ok);

    if (!ok || templateName.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Имя шаблона не может быть пустым."));
        return;
    }

    // 2. Создание TemplateData
    TemplateData newTemplate;
    newTemplate.name = templateName.toStdString();
    newTemplate.directory = dirTemplates; // можешь указать реальный путь
    int templateId = db->addTemplate(newTemplate.name, newTemplate.directory);

    if (templateId <= 0) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить шаблон в базу данных.");
        return;
    }

    newTemplate.id = templateId;

    QString dirDocx = QString::fromStdString(dirTemplates) + "/" + fileName.split('.').first() + "/" + fileName;
    QString dirHtml = QString::fromStdString(dirTemplates) + "/" + fileName.split('.').first() + "/" + fileName.split('.').first() + ".html";

    // 3. Сохранение файлов шаблона (если есть)
    db->addTemplateFile(dirDocx.toStdString(), templateId);
    db->addTemplateFile(dirHtml.toStdString(), templateId);
    // for (auto &file : templateFiles) {
    //     file.template_id = templateId;
    //     db->addTemplateFile(file.filename, templateId);
    // }

    // 4. Сохранение правил (из rulesManager)
    for (const auto &rule : rulesManager->geRules()) {
        if (!rule) continue; // на случай, если указатель пустой
        TemplateRule newRule = *rule;
        newRule.template_id = templateId;
        db->addRule(newRule);
    }

    QMessageBox::information(this, "Сохранено", "Шаблон, файлы и правила успешно сохранены.");

    updateUi();
    emit saveButtonClicked();
}

void TemplateEditPage::createRuleButtonSlot()
{
    QTextCursor cursor = textBrowserFile->textCursor();
    selectText = cursor.selectedText();

    startPosSelectText = cursor.selectionStart();
    endPosSelectText = cursor.selectionEnd();

    if(selectText.isEmpty()) {
        qDebug() << "Текст не выделен";
        return;
    }
    qDebug() << "Текст - " << selectText;
    qDebug() << "Начало: " << startPosSelectText << " Конец: " << endPosSelectText;
    auto newRule = std::make_shared<TemplateRule>();
    newRule->text_to_replace = selectText.toStdString();
    newRule->text_position = startPosSelectText;
    rulesManager->createNewRule(newRule);
    //rules.push_back(newRule);

    //emit ruleSelected(newRule);
    emit createRuleButtonClicked();
}

void TemplateEditPage::cancelCreation()
{
    //TODO: тут будет логика создания черновика
    //(сейчас я не буду это реализовывать, потому что при одинаковом файле может удалиться папка которая нужна)
    //поэтому тут обязательно нужно добавить логику с тем, что есть файл называется одинакого и то добавить номер к этому файлу
    updateUi();
    emit backButtonClicked();
    return;
}

void TemplateEditPage::updateRuleList()
{
    listRules->clear();

    for (int i = 0; i < rulesManager->rules.size(); ++i) {
        auto rule = rulesManager->rules[i];

        // === Контейнер для одной строки ===
        QWidget *itemWidget = new QWidget();

        // === Название правила ===
        QLabel *ruleLabel = new QLabel(QString::fromStdString(rule->name), itemWidget);
        ruleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        // === Кнопки ===
        QPushButton *editButton = new QPushButton("✏️", itemWidget);
        QPushButton *deleteButton = new QPushButton("🗑️", itemWidget);
        editButton->setFixedWidth(32);
        deleteButton->setFixedWidth(32);

        // === Горизонтальный layout для строки ===
        QVBoxLayout *layout = new QVBoxLayout(itemWidget);
        QHBoxLayout *layoutButton = new QHBoxLayout();
        layout->addWidget(ruleLabel);
        layoutButton->addWidget(editButton);
        layoutButton->addWidget(deleteButton);
        layout->addLayout(layoutButton);
        //layout->setContentsMargins(4, 2, 4, 2);

        // === Создаём элемент списка ===
        QListWidgetItem *item = new QListWidgetItem(listRules);
        item->setSizeHint(itemWidget->sizeHint());
        itemWidget->setFixedWidth(150);
        listRules->addItem(item);
        listRules->setItemWidget(item, itemWidget);

        // === Подключаем сигналы кнопок ===
        connect(editButton, &QPushButton::clicked, this, [this, i]() {
            rulesManager->updateRule(rulesManager->rules[i]);
            emit editRuleRequested(rulesManager->rules[i]);
        });

        connect(deleteButton, &QPushButton::clicked, this, [this, i]() {
            rulesManager->rules.erase(rulesManager->rules.begin() + i);
            updateRuleList();
        });
    }
}

