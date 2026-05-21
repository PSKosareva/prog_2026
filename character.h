#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <QTextStream>
#include "armor.h"

class Character
{
public:
    Character();
    Character(const QString& name, int health, const Armor& armor);
    virtual ~Character();

    virtual void craft(QTextStream& out) const = 0;
    virtual QString getInfo() const;
    virtual QString getType() const = 0;
    virtual int getCode() const = 0;

    QString getName() const { return m_name; }
    int getHealth() const { return m_health; }
    Armor getArmor() const { return m_armor; }

protected:
    QString m_name;
    int m_health;
    Armor m_armor;
};

#endif // CHARACTER_H
