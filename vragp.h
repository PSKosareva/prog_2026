#ifndef VRAGP_H
#define VRAGP_H

#include "character.h"

class VragP : public Character
{
public:
    VragP();
    VragP(const QString& name, const QString& rarity, int damage, int health, const Armor& armor);

    void craft(QTextStream& out) const override;
    QString getInfo() const override;
    QString getType() const override;
    int getCode() const override;

    QString getRarity() const;
    int getDamage() const;

private:
    QString m_rarity;
    int m_damage;
};

#endif // VRAGP_H
