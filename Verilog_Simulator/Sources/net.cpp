#include <QDebug>
#include <QString>
#include <QVector>
#include <ctype.h>

#include "Headers/net.h"

Net::Net(QString& name, ENet_Types net_type)
    :   m_value(2)
    ,	m_old_value(2)
{
    set_name(name);
    set_type(net_type);
    set_gates({});
}

Net::Net(QString& name, ENet_Types net_type, const QVector<std::shared_ptr<Gate>> connected_gates) : Net(name, net_type)
{
    set_gates(connected_gates);
}

void Net::set_gates(const QVector<std::shared_ptr<Gate>> gates)
{
    m_connected_gates = std::move(gates);
}

QString Net::get_name() const
{
    return m_name;
}

ENet_Types Net::get_type() const
{
    return m_type;
}

QVector<std::shared_ptr<Gate>> Net::get_connected_gates() const
{
    return m_connected_gates;
}

size_t Net::get_value() const
{
    return m_value;
}

size_t Net::get_old_value() const
{
    return m_old_value;
}

void Net::add_connection(const std::shared_ptr<Gate> gate)
{
   m_connected_gates.push_back(gate);
}

void Net::set_type(ENet_Types type)
{
    if (type == ENet_Types::INVALID)
    {
        //qDebug() << "ERROR: Invalid Net type";
        throw std::runtime_error("ERROR: Invalid Net type");
    }
    m_type = type;
}

void Net::set_name(const QString& name)
{
    if (name.isEmpty() || name[0].isDigit())
    {
        //qDebug() << "ERROR: Invalid Net name";
        throw std::runtime_error("ERROR: Invalid Net name");
    }
    m_name = name;
}

void Net::set_value(size_t value) {
    if (value <= 1) {
        m_old_value = m_value;
        m_value = value;
    }
    else {
        //qDebug() << "ERROR: ...";
        throw std::runtime_error("ERROR: ...");
    }
}
