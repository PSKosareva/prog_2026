#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "character.h"
#include "magp.h"
#include "vragp.h"
#include "carddialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Персонажи");

    m_table = ui->tableWidget;
    m_table->setColumnCount(5);
    QStringList headers = {"Имя", "Стихия", "ХП", "Мана", "Броня"};
    m_table->setHorizontalHeaderLabels(headers);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);
    m_table->setStyleSheet(
        "QTableWidget { gridline-color: transparent; }"
        "QTableWidget::item { border: none; }"
        "QTableWidget::item:selected { color: black; }"
        "QTableWidget::item:selected:active { background-color: #a0a0a0; color: black; }"
        );
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);

    connect(m_table, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClicked);

    m_loadButton = ui->loadButton;
    connect(m_loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text files (*.txt)");
    if (filename.isEmpty()) {
        filename = "characters.txt";
    }
    loadFromFile(filename);
}

void MainWindow::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл: " + filename);
        return;
    }

    clearTable();
    m_characters.clear();

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 5) continue;

        bool ok;
        int code = parts[0].toInt(&ok);
        if (!ok) continue;

        QString name = parts[1];

        QString armorStr = parts[parts.size() - 1];
        armorStr.remove('<').remove('>');
        QStringList armorParts = armorStr.split('/');

        Armor armor;
        if (armorParts.size() >= 3) {
            armor.helmet = armorParts[0].toInt();
            armor.cuirass = armorParts[1].toInt();
            armor.boots = armorParts[2].toInt();
        }

        if (code == 1 && parts.size() >= 5) {
            QString element = parts[2];
            int mana = parts[3].toInt();
            int health = parts[4].toInt();
            auto mag = std::make_unique<MagP>(name, element, mana, health, armor);
            addToTable(mag.get());
            m_characters.push_back(std::move(mag));
        }
        else if (code == 2 && parts.size() >= 5) {
            QString rarity = parts[2];
            int damage = parts[3].toInt();
            int health = parts[4].toInt();
            auto vrag = std::make_unique<VragP>(name, rarity, damage, health, armor);
            addToTable(vrag.get());
            m_characters.push_back(std::move(vrag));
        }
    }

    file.close();
    QMessageBox::information(this, "Загрузка", QString("Загружено %1 персонажей").arg(m_characters.size()));
}

void MainWindow::addToTable(Character* character)
{
    int row = m_table->rowCount();
    m_table->insertRow(row);

    if (character->getCode() == 1) {
        MagP* mag = dynamic_cast<MagP*>(character);
        if (mag) {
            m_table->setItem(row, 0, new QTableWidgetItem(mag->getName()));
            m_table->setItem(row, 1, new QTableWidgetItem(mag->getElement()));
            m_table->setItem(row, 2, new QTableWidgetItem(QString::number(mag->getHealth())));
            m_table->setItem(row, 3, new QTableWidgetItem(QString::number(mag->getMana())));
            m_table->setItem(row, 4, new QTableWidgetItem(
                                         QString("<%1/%2/%3>").arg(mag->getArmor().helmet)
                                             .arg(mag->getArmor().cuirass)
                                             .arg(mag->getArmor().boots)));

            m_table->item(row, 2)->setForeground(Qt::green);
            m_table->item(row, 3)->setForeground(Qt::blue);
        }
    } else {
        VragP* vrag = dynamic_cast<VragP*>(character);
        if (vrag) {
            m_table->setItem(row, 0, new QTableWidgetItem(vrag->getName()));
            m_table->setItem(row, 1, new QTableWidgetItem(vrag->getRarity()));
            m_table->setItem(row, 2, new QTableWidgetItem(QString::number(vrag->getHealth())));
            m_table->setItem(row, 3, new QTableWidgetItem(QString::number(vrag->getDamage())));
            m_table->setItem(row, 4, new QTableWidgetItem(
                                         QString("<%1/%2/%3>").arg(vrag->getArmor().helmet)
                                             .arg(vrag->getArmor().cuirass)
                                             .arg(vrag->getArmor().boots)));

            m_table->item(row, 2)->setForeground(Qt::red);
        }
    }
}

void MainWindow::clearTable()
{
    m_table->setRowCount(0);
}

void MainWindow::onTableDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= static_cast<int>(m_characters.size())) return;

    CardDialog* dialog = new CardDialog(m_characters[row].get(), row, this);
    connect(dialog, &CardDialog::characterDeleted, this, &MainWindow::onCharacterDeleted);
    dialog->exec();
    delete dialog;
}

void MainWindow::onCharacterDeleted(int row)
{
    if (row >= 0 && row < static_cast<int>(m_characters.size())) {
        m_characters.erase(m_characters.begin() + row);
        clearTable();
        for (auto& ch : m_characters) {
            addToTable(ch.get());
        }
    }
}
