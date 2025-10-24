#include "templateeditpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>


TemplateEditPage::TemplateEditPage(QWidget *parent)
    : QWidget{parent}
{
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
    QFile file("output.html");
    file.open(QIODevice::ReadOnly);
    textBrowserFile->setHtml(file.readAll());
}
