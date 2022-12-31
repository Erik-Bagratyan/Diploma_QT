#ifndef SIMULATION_H
#define SIMULATION_H

#include "Headers/timewheel.h"

#include <QVector>
#include <QMap>
#include <QString>

#include <memory>

class Netlist;

class Simulation
{
public:
    Simulation() = default;
    Simulation(std::shared_ptr<Netlist> netlist);
public:
    void event_driven_simulation(QVector<QMap<QString, size_t>> primary_inputs);
public:
    void set_netlist(std::shared_ptr<Netlist> netlist);
public:
    std::shared_ptr<QVector<QString>> getOutput() const;
public:
    QMap<QString, QVector<size_t>> getDumpedData() const;
public:
    bool isOnEventList(QVector<std::shared_ptr<Event>>& events, std::shared_ptr<Gate> gate) const;
public:
    void setupDumpedNets();
    void netScanner();
private:
    std::shared_ptr<Netlist> m_netlist;
    QMap<QString, QVector<size_t>> m_dumped_data;
    std::shared_ptr<QVector<QString>> m_output;
}; // Simulation

#endif // SIMULATION_H
