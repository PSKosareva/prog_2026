#include "character.h"

Character::Character() : m_name(""), m_health(0), m_armor(0,0,0) {}

Character::Character(const QString& name, int health, const Armor& armor)
    : m_name(name), m_health(health), m_armor(armor) {}

Character::~Character() {}

QString Character::getInfo() const
{
    return QString("Имя: %1\nЗдоровье: %2\nБроня: %3/%4/%5")
        .arg(m_name)
        .arg(m_health)
        .arg(m_armor.helmet)
        .arg(m_armor.cuirass)
        .arg(m_armor.boots);
}
