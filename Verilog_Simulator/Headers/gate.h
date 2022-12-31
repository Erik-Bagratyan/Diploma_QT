#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QVector>

#include <memory>

#include "Headers/net.h"

class Net;

enum class EGate_Types
{
    AND,
    NAND,
    OR,
    XOR,
    NOR,
    NOT,
    INVALID
};

class Gate
{
public:
    Gate(QString& name, EGate_Types gate_type, std::shared_ptr<Net> output_net, QVector<std::shared_ptr<Net>> input_nets);

public:
    size_t simulate();

public:
    void set_name(const QString& name);
    void set_delay(const size_t delay);
    void set_type(EGate_Types type);
    void set_value(size_t value);
    QString get_name() const;
    size_t get_delay() const;
    EGate_Types get_type() const;
    size_t get_value() const;
    std::shared_ptr<Net> get_output_net() const;
    QVector<std::shared_ptr<Net>> get_input_nets() const;
    QVector<std::shared_ptr<Net>> get_connected_nets() const;
    bool operator==(const std::shared_ptr<Gate> gate);

private:
    //Operations
    size_t AND_operation() const;
    size_t OR_operation() const;
    size_t XOR_operation() const;
    size_t NOT_operation() const;

private:
    QString m_name;
    EGate_Types m_type;
    std::shared_ptr<Net> m_output;
    QVector<std::shared_ptr<Net>> m_inputs;
    size_t m_delay;
    size_t m_value;

}; // GATE

#endif // GATE_H
