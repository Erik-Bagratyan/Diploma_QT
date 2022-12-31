#include <iostream>

#include "Headers/simulation.h"
#include "Headers/timewheel.h"
#include "Headers/netlist.h"
#include "Headers/net.h"
#include "Headers/event.h"

#include <QDebug>
Simulation::Simulation(std::shared_ptr<Netlist> netlist)
    : m_netlist(netlist)
    , m_output(new QVector<QString>())
{}

void Simulation::event_driven_simulation(QVector<QMap<QString, size_t>> primary_inputs)
{
    setupDumpedNets();

    auto primaryInputNets = m_netlist->get_primary_input_nets();
    TimeWheel mTimeWheel;
    size_t currentTime;
    for (int i = 0; i < primary_inputs.size(); ++i) {

        QString inputVectror{};
        for (auto input = primary_inputs[i].begin(); input != primary_inputs[i].end(); ++input) {
            inputVectror += input.key() + " : " + QString::number(input.value()); // + "/n"
        }
        m_output->push_back("Input vector");
        m_output->push_back(inputVectror);
        m_output->push_back("Events list:");

        for (auto input = primary_inputs[i].begin(); input != primary_inputs[i].end(); ++input) {
            auto net = primaryInputNets.find(input.key());
            if (net != primaryInputNets.end()) {
                net.value()->set_value(input.value());
            }
        }

        QVector<std::shared_ptr<Event>> initialEvents;
        const size_t startTime = 0;
        for (auto net : primaryInputNets) {
            auto gates = net->get_connected_gates();
            for (auto gate : gates) {
                if (!isOnEventList(initialEvents, gate)) {
                    initialEvents.push_back(std::make_shared<Event>(startTime, gate));
                }
            }
        }

        mTimeWheel.insert(initialEvents);

        currentTime = startTime;

        for (; currentTime < mTimeWheel.length(); ++currentTime) {
            auto currentEvents = mTimeWheel[currentTime];
            auto futureEvents = QVector<std::shared_ptr<Event>>{};
            for (auto event : currentEvents) {
                auto currentGate = event->getGate();
                auto oldValue = currentGate->get_value();
                auto newValue = currentGate->simulate();

                if (oldValue != newValue) {
                    m_output->push_back("time: " + QString::number(currentTime) +
                        " gate: " + currentGate->get_name() + "<" + GateType_to_Str[(currentGate->get_type())] + "> "
                        " old value: " + QString::number(static_cast<int>(oldValue)) +
                        " new value: " + QString::number(static_cast<int>(newValue)));

                    auto gateOutputNet = currentGate->get_output_net();
                    if (gateOutputNet) {
                        gateOutputNet->set_value(newValue);
                        auto gates = gateOutputNet->get_connected_gates();
                        for (auto gate : gates) {
                            futureEvents.push_back(std::make_shared<Event>(
                                currentTime + currentGate->get_delay(),
                                gate));
                        }
                    }
                }
                mTimeWheel.insert(futureEvents);
            }
        }

        netScanner();
        m_output->push_back("______________________________________________________________________\n");
    }
}

void Simulation::set_netlist(std::shared_ptr<Netlist> netlist)
{
    if (netlist != nullptr)
    {
        m_netlist = netlist;
        return;
    }
    throw std::runtime_error("Empty Netlist!");
}

std::shared_ptr<QVector<QString>> Simulation::getOutput() const
{
    return m_output; ;
}

QMap<QString, QVector<size_t>> Simulation::getDumpedData() const
{
    return m_dumped_data;;
}

bool Simulation::isOnEventList(QVector<std::shared_ptr<Event>>& events, std::shared_ptr<Gate> gate) const
{
    for (const auto& event : events) {
        if (event->getGate() == gate) {
            return true;
        }
    }
    return false;
}

void Simulation::setupDumpedNets()
{
    auto primaryInputNets = m_netlist->get_primary_input_nets();
    for (auto net : primaryInputNets) {
        m_dumped_data[net->get_name()] = QVector<size_t>();
    }

    auto primaryOutputs = m_netlist->get_primary_output_nets();
    for (auto net : primaryOutputs) {
        m_dumped_data[net->get_name()] = QVector<size_t>();
    }
}

void Simulation::netScanner()
{
    auto primaryInputNets = m_netlist->get_primary_input_nets();
    auto primaryOutputs = m_netlist->get_primary_output_nets();

    for (auto net = primaryInputNets.begin(); net != primaryInputNets.end(); ++net) {
        size_t signal = net.value()->get_value();
        m_dumped_data[net.key()].push_back(signal);
    }

    for (auto net = primaryOutputs.begin(); net != primaryOutputs.end(); ++net) {
        size_t signal = net.value()->get_value();
        m_dumped_data[net.key()].push_back(signal);
    }
}
