#include "templateuploadpage.h"

TemplateUploadPage::TemplateUploadPage(QWidget *parent)
    : QWidget{parent}{

    setAcceptDrops(true);

    auto *layout = new QVBoxLayout(this);
    auto *downLayout = new QHBoxLayout(this);

    label = new QLabel("Перетащите сюда DOCX файл", this);
    fileNameLabel = new QLabel("Файл - файл не выбран", this);

    selectButton = new QPushButton("Выбрать", this);
    selectButton->setDisabled(true);

    label->setAlignment(Qt::AlignCenter);

    downLayout->addWidget(fileNameLabel);
    downLayout->addWidget(selectButton);

    layout->addWidget(label);
    layout->addLayout(downLayout);


    connect(selectButton, &QPushButton::clicked, this, &TemplateUploadPage::selectButtonClicked);
}

void TemplateUploadPage::selectButtonClicked()
{
    emit fileLoaded(filePath);
}

void TemplateUploadPage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void TemplateUploadPage::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        filePath = urls.first().toLocalFile();
        QString fileName = filePath.split('/').back();
        fileNameLabel->setText(fileName);
        selectButton->setDisabled(false);
        qDebug() << "Файл загружен:" << filePath;
    }
}
