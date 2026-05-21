#include "magp.h"

MagP::MagP() : Character(), m_element(""), m_mana(0) {}

MagP::MagP(const QString& name, const QString& element, int mana, int health, const Armor& armor)
    : Character(name, health, armor), m_element(element), m_mana(mana) {}

void MagP::craft(QTextStream& out) const
{
    QString elementColor = "black";
    QString el = m_element.toLower();
    if (el == "огонь") elementColor = "red";
    else if (el == "вода") elementColor = "blue";
    else if (el == "земля") elementColor = "brown";

    out << "<div align='center' style='margin-bottom: 15px;'>" << m_name << "</div>";
    out << "<div align='center' style='margin-bottom: 15px;'><font color='" << elementColor << "'>" << m_element << "</font></div>";
    out << "<div align='center'><font color='green'>" << m_health << "</font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='blue'>" << m_mana << "</font></div>";
}

QString MagP::getInfo() const
{
    return QString("Имя: %1\nТип: Маг\nСтихия: %2\nМана: %3\nЗдоровье: %4\nБроня: %5/%6/%7")
        .arg(m_name)
        .arg(m_element)
        .arg(m_mana)
        .arg(m_health)
        .arg(m_armor.helmet)
        .arg(m_armor.cuirass)
        .arg(m_armor.boots);
}

QString MagP::getType() const
{
    return "Маг";
}

int MagP::getCode() const
{
    return 1;
}

QString MagP::getElement() const
{
    return m_element;
}

int MagP::getMana() const
{
    return m_mana;
}
