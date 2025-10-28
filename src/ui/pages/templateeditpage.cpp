#include "templateeditpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextCursor>
#include <QTextDocumentFragment>

TemplateEditPage::TemplateEditPage(QWidget *parent)
    : QWidget{parent}
{
    pythonWorker = std::make_unique<PythonWorker>(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("python"));

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
    listRules->setMaximumWidth(100);

    // === Верхняя часть (левая и правая колонка) ===
    auto *topLayout = new QHBoxLayout();
    topLayout->addLayout(leftLayout, 3);   // вес 3 → занимает больше места
    topLayout->addLayout(rightLayout, 1);  // вес 1 → уже

    // === Нижняя часть: две кнопки по горизонтали ===
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch(); // чтобы кнопки прижались вправо
    bottomLayout->addWidget(createRuleButton);
    bottomLayout->addWidget(saveButton);

    // === Главный layout ===
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setStretch(0, 1); // верхняя часть растягивается
    mainLayout->setStretch(1, 0); // нижняя — фиксированная

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
    // Пример 1 — обычный скрипт
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

void TemplateEditPage::saveSelectedText()
{
    return;
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
    rules.push_back(newRule);

    emit ruleSelected(newRule);
    emit createRuleButtonClicked();
}

void TemplateEditPage::cancelCreation()
{
    //TODO: тут будет логика создания черновика
    //(сейчас я не буду это реализовывать, потому что при одинаковом файле может удалиться папка которая нужна)
    emit backButtonClicked();
    return;
}

void TemplateEditPage::updateRuleList()
{
    listRules->clear();

    for (int i = 0; i < rules.size(); ++i) {
        auto rule = rules[i];

        // === Контейнер для одной строки ===
        QWidget *itemWidget = new QWidget();

        // === Название правила ===
        QLabel *ruleLabel = new QLabel(QString::fromStdString(rule->text_to_replace), itemWidget);
        ruleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        // === Кнопки ===
        QPushButton *editButton = new QPushButton("✏️", itemWidget);
        QPushButton *deleteButton = new QPushButton("🗑️", itemWidget);
        editButton->setFixedWidth(32);
        deleteButton->setFixedWidth(32);

        // === Горизонтальный layout для строки ===
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->addWidget(ruleLabel);
        layout->addWidget(editButton);
        layout->addWidget(deleteButton);
        layout->setContentsMargins(4, 2, 4, 2);

        // === Создаём элемент списка ===
        QListWidgetItem *item = new QListWidgetItem(listRules);
        item->setSizeHint(itemWidget->sizeHint());
        listRules->addItem(item);
        listRules->setItemWidget(item, itemWidget);

        // === Подключаем сигналы кнопок ===
        connect(editButton, &QPushButton::clicked, this, [this, i]() {
            emit editRuleRequested(rules[i]);
        });

        connect(deleteButton, &QPushButton::clicked, this, [this, i]() {
            rules.erase(rules.begin() + i);
            updateRuleList();
        });
    }
}

