#ifndef MAGP_H
#define MAGP_H

#include "character.h"

class MagP : public Character
{
public:
    MagP();
    MagP(const QString& name, const QString& element, int mana, int health, const Armor& armor);

    void craft(QTextStream& out) const override;
    QString getInfo() const override;
    QString getType() const override;
    int getCode() const override;

    QString getElement() const;
    int getMana() const;

private:
    QString m_element;
    int m_mana;
};

#endif // MAGP_H
