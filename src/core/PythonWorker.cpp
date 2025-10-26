#include "PythonWorker.h"
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <stdexcept>

PythonWorker::PythonWorker(const QString& pythonDir)
    : pythonDir_(pythonDir)
{
    QString pythonExe = QDir(pythonDir_).absoluteFilePath("python.exe");
    if (!QFile::exists(pythonExe)) {
        throw std::runtime_error("Python executable not found: " + pythonExe.toStdString());
    }
}

bool PythonWorker::runScript(const QString& scriptName, const QStringList& args)
{
    QString pythonExe = QDir(pythonDir_).absoluteFilePath("python.exe");
    QString scriptPath = QDir(pythonDir_).absoluteFilePath(scriptName);

    if (!QFile::exists(pythonExe) || !QFile::exists(scriptPath)) {
        qWarning() << "Python or script not found!";
        return false;
    }

    QProcess process;
    process.setProgram(pythonExe);

    QStringList fullArgs;
    fullArgs << scriptPath;
    fullArgs << args;
    process.setArguments(fullArgs);
    process.setWorkingDirectory(QDir::currentPath());

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONHOME", pythonDir_);
    env.insert("PYTHONPATH", QDir(pythonDir_).absoluteFilePath("Lib"));
    process.setProcessEnvironment(env);

    process.start();
    if (!process.waitForStarted()) {
        qWarning() << "Failed to start Python process!";
        return false;
    }

    process.waitForFinished(-1);

    QString output = QString::fromUtf8(process.readAllStandardOutput());
    QString error  = QString::fromUtf8(process.readAllStandardError());

    if (!error.isEmpty())
        qWarning() << "Python error:" << error;

    if (!output.isEmpty())
        qDebug() << "Python output:" << output;

    return process.exitCode() == 0;
}

bool PythonWorker::runFirstScript(const QStringList& args) {
    return runScript("main.py", args);
}

bool PythonWorker::runSecondScript(const QStringList& args) {
    return runScript("second_script.py", args);
}

// Универсальный метод для получения вывода
QString PythonWorker::runScriptAndGetOutput(const QString& scriptName, const QStringList& args)
{
    QString pythonExe = QDir(pythonDir_).absoluteFilePath("python.exe");
    QString scriptPath = QDir(pythonDir_).absoluteFilePath(scriptName);

    if (!QFile::exists(pythonExe) || !QFile::exists(scriptPath)) {
        qWarning() << "Python or script not found!";
        return {};
    }

    QProcess process;
    process.setProgram(pythonExe);

    QStringList fullArgs;
    fullArgs << scriptPath;
    fullArgs << args;
    process.setArguments(fullArgs);
    process.setWorkingDirectory(QDir::currentPath());

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONHOME", pythonDir_);
    env.insert("PYTHONPATH", QDir(pythonDir_).absoluteFilePath("Lib"));
    process.setProcessEnvironment(env);

    process.start();
    if (!process.waitForStarted()) {
        qWarning() << "Failed to start Python process!";
        return {};
    }

    process.waitForFinished(-1);
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    QString error = QString::fromUtf8(process.readAllStandardError());

    if (!error.isEmpty())
        qWarning() << "Python stderr:" << error;

    return output.trimmed();
}

// Удобный метод для вызова fio_declension.py
QString PythonWorker::runFioDeclensionScript(const QString& fio, const QString& grammaticalCase)
{
    return runScriptAndGetOutput("fio_declension.py", {fio, grammaticalCase});
}
