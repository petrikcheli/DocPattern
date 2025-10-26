#pragma once
#include <QString>
#include <QStringList>

class PythonWorker
{
public:
    explicit PythonWorker(const QString& pythonDir);

    bool runScript(const QString& scriptName, const QStringList& args);
    bool runFirstScript(const QStringList& args);
    bool runSecondScript(const QStringList& args);

    // Новый универсальный метод: возвращает stdout как строку
    QString runScriptAndGetOutput(const QString& scriptName, const QStringList& args);

    // Метод для скрипта с Petrovich
    QString runFioDeclensionScript(const QString& fio, const QString& grammaticalCase);

private:
    QString pythonDir_;
};
