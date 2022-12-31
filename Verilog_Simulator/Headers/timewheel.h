#ifndef TIMEWHEEL_H
#define TIMEWHEEL_H

#include <QVector>

#include <memory>

#include "event.h"
#include "gate.h"

//class Event;

class TimeWheel
{
public:
    TimeWheel() = default;
public:
    void insert(QVector<std::shared_ptr<Event>> items);
    void insert(std::shared_ptr<Event> item);
public:
    size_t length() const;
    std::shared_ptr<Gate> find(std::shared_ptr<Gate> gate) const;
public:
    QVector<std::shared_ptr<Event>>& operator[](size_t idx);
private:
    void extend_capacity(size_t size);
private:
    QVector<QVector<std::shared_ptr<Event>>> m_events;
};

#endif // TIMEWHEEL_H
