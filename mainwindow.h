#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include <memory>

class Character;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadButtonClicked();
    void onTableDoubleClicked(int row, int column);
    void onCharacterDeleted(int row);

private:
    void loadFromFile(const QString& filename);
    void addToTable(Character* character);
    void clearTable();

    Ui::MainWindow* ui;
    QTableWidget* m_table;
    QPushButton* m_loadButton;
    std::vector<std::unique_ptr<Character>> m_characters;
};

#endif // MAINWINDOW_H
