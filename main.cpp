#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrl>
#include <QDebug>

void translateText(const QString &text, const QString &targetLang, QTextEdit *outputField){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url("http://127.0.0.1:5000/convert");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["text"] = text;
    json["target_lang"] = targetLang;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    QObject::connect(reply, &QNetworkReply::finished, [reply,outputField](){

        if(reply->error() == QNetworkReply::NoError){
            QByteArray respondData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(respondData);
            QJsonObject jsonObject = jsonDoc.object();

            QString translatedText = jsonObject["translated_text"].toString();
            outputField->setPlainText(translatedText);
        }
        else{
            qDebug() << "Translation error:" <<reply->errorString();
        }

        reply->deleteLater();
    });

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("Language Translator App");
    mainWindow.resize(800,600);

    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *titleLabel = new QLabel("Language Translator App");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; color:red; font-family: Bell MT");
    layout->addWidget(titleLabel);

    //Create a layout for dropdowns and text fields
    QVBoxLayout *formLayout = new QVBoxLayout();

    QComboBox *sourceLanguageComboBox = new QComboBox();
    sourceLanguageComboBox->addItem("English","en");
    sourceLanguageComboBox->addItem("Spanish","es");
    sourceLanguageComboBox->addItem("French","fr");
    sourceLanguageComboBox->addItem("Chinese","zh");

    sourceLanguageComboBox->setCurrentIndex(0);
    sourceLanguageComboBox->setStyleSheet("font-size: 20px; padding: 5px");
    formLayout->addWidget(sourceLanguageComboBox,0,Qt::AlignCenter);

    QTextEdit *textInput = new QTextEdit();
    textInput->setFixedSize(400,150);
    textInput->setStyleSheet("background-color:lightgray; color: black; font-size:20px");
    textInput->setAcceptRichText(false);
    formLayout->addWidget(textInput,0,Qt::AlignCenter);

    QComboBox *targetLanguageComboBox = new QComboBox();
    targetLanguageComboBox->addItem("English","en");
    targetLanguageComboBox->addItem("Spanish","es");
    targetLanguageComboBox->addItem("French","fr");
    targetLanguageComboBox->addItem("Chinese","zh");

    targetLanguageComboBox->setCurrentIndex(0);
    targetLanguageComboBox->setStyleSheet("font-size: 20px; padding: 5px");
    formLayout->addWidget(targetLanguageComboBox,0,Qt::AlignCenter);

    QTextEdit *translationOutput = new QTextEdit();
    translationOutput->setFixedSize(400,150);
    translationOutput->setStyleSheet("background-color:lightgray; color: black; font-size:20px");
    translationOutput->setAcceptRichText(false);
    translationOutput->setReadOnly(true);
    formLayout->addWidget(translationOutput,0,Qt::AlignCenter);

    QPushButton *translateButton = new QPushButton("Translate");
    translateButton->setFixedSize(150,60);
    translateButton->setStyleSheet("background-color: green; color: white; font-size:20px");
    formLayout->addWidget(translateButton,0, Qt::AlignCenter);

    QObject::connect(translateButton, &QPushButton::clicked, [&](){
        QString text = textInput->toPlainText();
        QString targetLang = targetLanguageComboBox->currentData().toString();
        translateText(text,targetLang,translationOutput);
    });

    layout->addLayout(formLayout);

    mainWindow.show();

    return app.exec();
}
