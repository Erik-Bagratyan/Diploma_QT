#ifndef NETLIST_H
#define NETLIST_H

#include <QMap>
#include <QFile>
#include <QVector>
#include <QString>
#include <QTextStream>

#include <memory>

#include "Headers/net.h"
#include "Headers/gate.h"

extern QMap<EGate_Types, QString> GateType_to_Str;

enum class EObj_Types
{
    GATE,
    NET,
    INVALID
};

class Netlist
{
public:
    Netlist(const QString& netlist_path);

public:
    void read();
    void add_net(const std::shared_ptr<Net> net);
    void add_gate(const std::shared_ptr<Gate> gate);
    void remove_comment(QString& line);
    void strip(QString& str);
    std::shared_ptr<QFile> exists(const QString& fileName);

public:
    QString get_netlist_name() const;
    QString get_netlist_path() const;
    QMap<QString, QVector<std::shared_ptr<Net>>> get_nets() const;
    QMap<QString, std::shared_ptr<Net>> get_primary_input_nets();
    QMap<QString, std::shared_ptr<Net>> get_primary_output_nets();
    QMap<QString, QVector<std::shared_ptr<Gate>>> get_gates() const;
    std::shared_ptr<Net> get_net_by_name(QString& name);

private:
    QString get_path_basename(const QString& path) const;
    QVector<QString> get_nets_from_line(QString& line);
    void create_gate_obj(QString& line, EGate_Types gate_type);
    EObj_Types get_obj_type(QString& temp);
    QString& getline(QTextStream& text_stream, QString& text);

private:
    QString m_file_name;
    QString m_file_path;
    QMap<QString, QVector<std::shared_ptr<Gate>>> m_gates;
    QMap<QString, QVector<std::shared_ptr<Net>>> m_nets;
}; // Netlist

#endif // NETLIST_H
