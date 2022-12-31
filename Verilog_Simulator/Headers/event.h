#ifndef EVENT_H
#define EVENT_H

#include <memory>

#include "Headers/gate.h"

class Event
{
public:
    Event() = default;
    Event(size_t time, std::shared_ptr<Gate> gate);
    Event(size_t newValue, size_t time, std::shared_ptr<Gate> gate);

public:
    std::shared_ptr<Gate> getGate();
    size_t getTime() const;
    size_t getNewValue() const;

public:
    size_t m_new_value;
    size_t m_time;
    std::shared_ptr<Gate> m_gate;
}; // Event


#endif // EVENT_H
