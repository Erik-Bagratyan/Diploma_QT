#ifndef NET_H
#define NET_H

#include <QString>
#include <QVector>

#include <memory>

#include "Headers/gate.h"

class Gate;

enum class ENet_Types
{
    WIRE,
    INPUT,
    OUTPUT,
    INVALID
};

class Net
{
public:
    Net() = default;
    Net(QString& name, ENet_Types net_type);
    Net(QString& name, ENet_Types net_type, QVector<std::shared_ptr<Gate>> connected_gates);

public:
    void add_connection(const std::shared_ptr<Gate> gate);
    void set_type(ENet_Types type);
    void set_name(const QString& name);
    void set_value(size_t value);
    void set_gates(const QVector<std::shared_ptr<Gate>> gates);

    QString get_name() const;
    ENet_Types get_type() const;
    QVector<std::shared_ptr<Gate>> get_connected_gates() const;
    size_t get_value() const;
    size_t get_old_value() const;

private:
    QString m_name;
    ENet_Types m_type;
    QVector<std::shared_ptr<Gate>> m_connected_gates;
    size_t m_value;
    size_t m_old_value;

}; // Net

#endif // NET_H
