#include "lyrics.h"

Lyrics::Lyrics(const QString &m_data) : Block(m_data)
{

}

QString Lyrics::toHtml()
{
    return "<p>" + htmlCheckBIU(m_data) + "</p>";
}

QString Lyrics::toFountain()
{
    return "~" + m_data;
}

void Lyrics::toTreeWidgetItem(QTreeWidgetItem *parent)
{
    parent->addChild(new QTreeWidgetItem(QStringList() << m_data));
}