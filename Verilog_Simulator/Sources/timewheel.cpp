#include "Headers/timewheel.h"

#include <QVector>

#include <algorithm>

void TimeWheel::insert(QVector<std::shared_ptr<Event>> items) {
    if (items.size() > 0) {
        auto time = items[0]->getTime();
        extend_capacity(time + 1);
        std::for_each(std::begin(items), std::end(items), [&, this](auto item) { insert(item); });
    }
}

void TimeWheel::insert(std::shared_ptr<Event> item) {
    auto time = item->getTime();
    extend_capacity(time);
    m_events[time].push_back(item);
}

std::shared_ptr<Gate> TimeWheel::find(std::shared_ptr<Gate> gate) const {
    for (const auto& eventList : m_events) {
        for (const auto& event : eventList) {
            if (event->getGate() == gate) {
                return event->getGate();
            }
        }
    }
    return nullptr;
}

QVector<std::shared_ptr<Event>>& TimeWheel::operator[](size_t idx) {
    return m_events[idx];
}

void TimeWheel::extend_capacity(size_t size) {
    if (size > m_events.capacity()) {
        m_events.resize(size + 1);
    }
}

size_t TimeWheel::length() const
{
    return m_events.size();
}
