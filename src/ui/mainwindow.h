#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "templateuploadpage.h"
#include "templateeditpage.h"
#include "templaterulespage.h"
#include "RulesManager.h"

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

    std::shared_ptr<RulesManager> rulesManager;

};
#endif // MAINWINDOW_H
