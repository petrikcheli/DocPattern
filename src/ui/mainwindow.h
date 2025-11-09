#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

//страницы для создания шаблонов
#include "templateuploadpage.h"
#include "templateeditpage.h"
#include "templaterulespage.h"

//страницы для генерации по шаблонам
#include "TemplateGeneratorSearch.h"
#include "TemplateGeneratorBuild.h"

//вспомогательные классы
#include "RulesManager.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *central;
    QHBoxLayout *layout;
    QListWidget *sideMenu;
    QStackedWidget *stack;

    TemplateUploadPage *uploadPage;
    TemplateEditPage *editPage;
    TemplateRulesPage *rulePage;

    TemplateGeneratorSearch *generatorSearchPage;
    TemplateGeneratorBuild *generatorBuildPage;

    std::shared_ptr<RulesManager> rulesManager;

    std::shared_ptr<DataBase> db;

    std::shared_ptr<PythonWorker> pythonWorker;
};
#endif // MAINWINDOW_H
