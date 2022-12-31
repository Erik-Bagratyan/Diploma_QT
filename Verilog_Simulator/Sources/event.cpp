#include "Headers/event.h"

Event::Event(size_t time, std::shared_ptr<Gate> gate)
    :	m_time(time)
    ,	m_gate(gate)
{}

Event::Event(size_t new_value, size_t time, std::shared_ptr<Gate> gate)
    :	m_new_value(new_value)
    ,	m_time(time)
    ,	m_gate(gate)
{}

std::shared_ptr<Gate> Event::getGate()
{
    return m_gate;
}

size_t Event::getTime() const
{
    return m_time;
}

size_t Event::getNewValue() const
{
    return m_new_value;
}
