#include <QDebug>
#include <QString>
#include <QVector>
//#include <ctype.h>

#include "Headers/gate.h"

Gate::Gate(QString& name, EGate_Types gate_type,
           std::shared_ptr<Net> output_net, QVector<std::shared_ptr<Net>> input_nets)
{
    set_type(gate_type);
    set_name(name);
    m_output = output_net;
    m_inputs = input_nets;
    m_delay = 1;
    m_value = 0;
}

size_t Gate::simulate()
{
    size_t out = 0;
    switch (m_type)
    {
        case EGate_Types::AND:
        {
            out = AND_operation();
            m_output->set_value(out);
            set_value(out);
            return out;
        }
        case EGate_Types::NAND:
        {
            out = (AND_operation() == 1) ? 0 : 1;
            m_output->set_value(out);
            set_value(out);
            return out;
        }
        case EGate_Types::OR:
        {
            out = OR_operation();
            m_output->set_value(out);
            set_value(out);
            return out;
        }
        case EGate_Types::NOR:
        {
            out = (OR_operation() == 1) ? 0 : 1;
            m_output->set_value(out);
            set_value(out);
            return out;
        }
        case EGate_Types::XOR:
        {
            out = XOR_operation();
            m_output->set_value(out);
            set_value(out);
            return out;
        }
        case EGate_Types::NOT:
        {
            out = NOT_operation();
            m_output->set_value(out);
            set_value(out);
            return out;
        }
    }
    return out;
}

void Gate::set_name(const QString& name)
{
    if (name.isEmpty() || name[0].isDigit())
    {
        //qDebug() << "ERROR: Invalid Gate name";
        throw std::runtime_error("ERROR: Invalid Gate name");
    }
    m_name = name;
}

void Gate::set_delay(const size_t delay)
{
    m_delay = delay;
}

void Gate::set_type(EGate_Types type)
{
    if (type == EGate_Types::INVALID)
    {
        //qDebug() << "ERROR: Invalid Gate type";
        throw std::runtime_error("ERROR: Invalid Gate type");
    }
    m_type = type;
}

void Gate::set_value(size_t value)
{
    if (value != 0 && value != 1)
    {
        //qDebug() << "ERROR: Invalid Value";
        throw std::runtime_error("ERROR: Invalid Value");
    }
    m_value = value;
}

QString Gate::get_name() const
{
    return m_name;
}

size_t Gate::get_delay() const
{
    return m_delay;
}

EGate_Types Gate::get_type() const
{
    return m_type;
}

size_t Gate::get_value() const
{
    return m_value;
}

std::shared_ptr<Net> Gate::get_output_net() const
{
    return m_output;
}

QVector<std::shared_ptr<Net>> Gate::get_input_nets() const
{
    return m_inputs;
}

QVector<std::shared_ptr<Net>> Gate::get_connected_nets() const
{
    QVector<std::shared_ptr<Net>> temp = get_input_nets();
    temp.push_back(get_output_net());
    return temp;
}

bool Gate::operator==(const std::shared_ptr<Gate> gate) {
    return m_name == gate->get_name();
}

size_t Gate::AND_operation() const
{
    for (int i = 0; i < m_inputs.size(); ++i)
    {
        if (m_inputs[i]->get_value() == 0)
            return 0;
    }
    return 1;
}

size_t Gate::OR_operation() const
{
    for (std::shared_ptr<Net> elem : m_inputs)
    {
        if (elem->get_value() == 1)
            return 1;
    }
    return 0;
}

size_t Gate::XOR_operation() const
{
    size_t coutn_of_1 = 0;
    for (auto elem : m_inputs)
    {
        if (elem->get_value() == 1)
            ++coutn_of_1;
    }
    return (coutn_of_1 % 2 == 1) ? 1 : 0;
}

size_t Gate::NOT_operation() const
{
    return (m_inputs[0]->get_value() == 0) ? 1 : 0;
}
