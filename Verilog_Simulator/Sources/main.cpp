//#include "Headers/mainwindow.h"
#include "Headers/netlist.h"
#include "Headers/simulation.h"
#include <QApplication>

#include <QDebug>
#include <QMap>

#include <memory>

void print_test_branch(QVector<QMap<QString, size_t>> primaryInputs);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QString path = "Verilog/simple.txt";
    QString path = "Verilog/example.txt";
    std::shared_ptr<Netlist> netlist = std::make_shared<Netlist>(path);
    netlist->read();
    srand(static_cast<unsigned int>(time(nullptr)));
    auto inputNets = netlist->get_primary_input_nets();
    QVector<QMap<QString, size_t>> primaryInputs {};
    primaryInputs.resize(5);
    for(int i = 0; i < 5; ++i) {
        for(auto el: inputNets) {
            primaryInputs[i][el->get_name()] = rand() % 2;
        }
    }
    Simulation simulation(netlist);
    simulation.event_driven_simulation(primaryInputs);
    for (auto elem : *simulation.getOutput())
    {
        qDebug() << elem;
    }
    return a.exec();
}
