#include "carddialog.h"
#include "ui_carddialog.h"
#include "character.h"
#include "magp.h"
#include "vragp.h"
#include <QFile>
#include <QTextStream>
#include <QPixmap>

CardDialog::CardDialog(Character* character, int row, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::CardDialog)
    , m_character(character)
    , m_row(row)
{
    ui->setupUi(this);
    ui->imageLabel->setStyleSheet(
        "QLabel {"
        "    border: 2px solid black;"
        "    border-radius: 10px;"
        "    background-color: #f0f0f0;"
        "}"
        );
    setWindowTitle("Карточка персонажа");

    loadImage();
    updateInfo();

    connect(ui->printButton, &QPushButton::clicked, this, &CardDialog::onPrintClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &CardDialog::onCancelClicked);
}

CardDialog::~CardDialog()
{
    delete ui;
}

void CardDialog::loadImage()
{
    QString path;

    if (m_character->getCode() == 1) {
        path = "images/mag.png";
    } else {
        path = "images/vrag.png";
    }

    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        ui->imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->imageLabel->setText(m_character->getCode() == 1 ? "🧙" : "👹");
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setStyleSheet("font-size: 100px; border: 1px solid gray;");
    }
}

void CardDialog::updateInfo()
{
    QString info;
    QTextStream stream(&info);

    m_character->craft(stream);

    ui->infoText->setText(info);
    ui->infoText->setReadOnly(true);
}

void CardDialog::onPrintClicked()
{
    QFile file("output.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        m_character->craft(out);
        file.close();
    }

    emit characterDeleted(m_row);
    accept();
}

void CardDialog::onCancelClicked()
{
    reject();
}
