#include "vragp.h"

VragP::VragP() : Character(), m_rarity(""), m_damage(0) {}

VragP::VragP(const QString& name, const QString& rarity, int damage, int health, const Armor& armor)
    : Character(name, health, armor), m_rarity(rarity), m_damage(damage) {}

void VragP::craft(QTextStream& out) const
{
    QString nameColor = "black";
    QString r = m_rarity.toLower();
    if (r == "редкий") nameColor = "#FFD700";
    else if (r == "обычный") nameColor = "gray";
    else if (r == "легенда") nameColor = "purple";

    out << "<div align='center' style='margin-bottom: 15px;'><font color='" << nameColor << "'>" << m_name << "</font></div>";
    out << "<div align='center' style='margin-bottom: 15px;'><font color='red'>" << m_health << "</font></div>";
    out << "<div align='center'>&lt;" << m_armor.helmet << "/"
        << m_armor.cuirass << "/"
        << m_armor.boots << "&gt;</div>";
}

QString VragP::getInfo() const
{
    return QString("Имя: %1\nТип: Враг\nРедкость: %2\nУрон: %3\nЗдоровье: %4\nБроня: %5/%6/%7")
        .arg(m_name)
        .arg(m_rarity)
        .arg(m_damage)
        .arg(m_health)
        .arg(m_armor.helmet)
        .arg(m_armor.cuirass)
        .arg(m_armor.boots);
}

QString VragP::getType() const
{
    return "Враг";
}

int VragP::getCode() const
{
    return 2;
}

QString VragP::getRarity() const
{
    return m_rarity;
}

int VragP::getDamage() const
{
    return m_damage;
}
