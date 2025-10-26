#include "templateeditpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>




TemplateEditPage::TemplateEditPage(QWidget *parent)
    : QWidget{parent}
{
    pythonWorker = std::make_unique<PythonWorker>(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("python"));

    createRuleButton = new QPushButton("Добавить правило", this);
    saveButton = new QPushButton("Сохранить", this);
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
