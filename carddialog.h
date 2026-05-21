#ifndef CARDDIALOG_H
#define CARDDIALOG_H

#include <QDialog>

class Character;

namespace Ui {
class CardDialog;
}

class CardDialog : public QDialog
{
    Q_OBJECT

public:
    CardDialog(Character* character, int row, QWidget* parent = nullptr);
    ~CardDialog();

signals:
    void characterDeleted(int row);

private slots:
    void onPrintClicked();
    void onCancelClicked();

private:
    void loadImage();
    void updateInfo();

    Ui::CardDialog* ui;
    Character* m_character;
    int m_row;
};

#endif // CARDDIALOG_H
