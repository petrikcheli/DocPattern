#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    central = new QWidget(this);

    setCentralWidget(central);

    rulesManager = std::make_shared<RulesManager>();

    layout = new QHBoxLayout(central);
    sideMenu = new QListWidget(this);
    stack = new QStackedWidget(this);
    uploadPage = new TemplateUploadPage(this);
    editPage = new TemplateEditPage(rulesManager, this);
    rulePage = new TemplateRulesPage(rulesManager, this);

    sideMenu->setViewMode(QListWidget::IconMode);
    sideMenu->setIconSize(QSize(40,40));
    sideMenu->setMovement(QListWidget::Static);
    sideMenu->setMaximumWidth(70);
    sideMenu->setSpacing(10);

    sideMenu->addItem(new QListWidgetItem(QIcon("icons/начать.png"), ""));
    sideMenu->addItem(new QListWidgetItem(QIcon("icons/создать.png"), ""));
    sideMenu->addItem(new QListWidgetItem(QIcon("icons/изменить.png"), ""));
    sideMenu->addItem(new QListWidgetItem(QIcon("icons/удалить.png"), ""));

    stack->addWidget(new QLabel("Добро пожаловать"));
    stack->addWidget(uploadPage);
    stack->addWidget(new QLabel("Изменить"));
    stack->addWidget(new QLabel("Удалить"));
    stack->addWidget(editPage);
    stack->addWidget(rulePage);

    layout->addWidget(sideMenu);
    layout->addWidget(stack);

    connect(uploadPage, &TemplateUploadPage::fileLoaded, this, [=](const QString &path){
        editPage->setFilePath(path);
        stack->setCurrentWidget(editPage);
    });

    connect(editPage, &TemplateEditPage::createRuleButtonClicked, this, [=](){
        rulePage->setRule();
        stack->setCurrentWidget(rulePage);
    });

    // c
    connect(editPage, &TemplateEditPage::backButtonClicked, this, [=]{
        stack->setCurrentWidget(uploadPage);
    });

    connect(rulePage, &TemplateRulesPage::backClicked, this, [=]{
        stack->setCurrentWidget(editPage);
    });

    connect(rulePage, &TemplateRulesPage::saveClicked, this, [=]{
        editPage->updateRuleList();
        stack->setCurrentWidget(editPage);
    });

    connect(sideMenu, &QListWidget::currentRowChanged,
            stack, &QStackedWidget::setCurrentIndex);

    // connect(editPage, &TemplateEditPage::ruleSelected,
    //         rulePage, &TemplateRulesPage::setRule);
    sideMenu->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
