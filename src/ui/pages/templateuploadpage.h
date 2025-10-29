#ifndef TEMPLATEUPLOADPAGE_H
#define TEMPLATEUPLOADPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPushButton>

class TemplateUploadPage : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateUploadPage(QWidget *parent = nullptr);

private slots:
    void selectButtonClicked();

signals:
    //void fileLoaded(const QString &filePath);
    void fileLoaded(const QString &filePath);

protected:
    // 👇 Объявляем функции обработки drag & drop
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *label;
    QLabel *fileNameLabel;
    QPushButton *selectButton;

    QString filePath;
};

#endif // TEMPLATEUPLOADPAGE_H
