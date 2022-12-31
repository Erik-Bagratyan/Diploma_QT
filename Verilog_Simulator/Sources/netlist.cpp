#include <QDebug>
#include <QVector>
#include <QMap>
#include <QString>

#include <algorithm>

#include "Headers/netlist.h"

QMap<EGate_Types, QString> GateType_to_Str = { std::make_pair(EGate_Types::AND, "AND"),
                                               std::make_pair(EGate_Types::NAND, "NAND"),
                                               std::make_pair(EGate_Types::OR, "OR"),
                                               std::make_pair(EGate_Types::XOR, "XOR"),
                                               std::make_pair(EGate_Types::NOR, "NOR"),
                                               std::make_pair(EGate_Types::NOT, "NOT") };

QMap<ENet_Types, QString> NetType_to_Str = { std::make_pair(ENet_Types::WIRE, "WIRE"),
                                                     std::make_pair(ENet_Types::INPUT, "INPUT"),
                                                     std::make_pair(ENet_Types::OUTPUT, "OUTPUT") };

QMap<QString, EGate_Types> Gates = { std::make_pair("AND", EGate_Types::AND),
                                                std::make_pair("NAND", EGate_Types::NAND),
                                                std::make_pair("OR", EGate_Types::OR),
                                                std::make_pair("XOR", EGate_Types::XOR),
                                                std::make_pair("NOR", EGate_Types::NOR),
                                                std::make_pair("NOT", EGate_Types::NOT) };

QMap<QString, ENet_Types> Nets = { std::make_pair("WIRE", ENet_Types::WIRE),
                                            std::make_pair("INPUT", ENet_Types::INPUT),
                                            std::make_pair("OUTPUT", ENet_Types::OUTPUT) };


Netlist::Netlist(const QString& netlist_path)
    :	m_file_name(get_path_basename(netlist_path))
    ,	m_file_path(netlist_path)
    ,	m_gates(QMap<QString, QVector<std::shared_ptr<Gate>>>{})
    ,	m_nets(QMap<QString, QVector<std::shared_ptr<Net>>>{})
{}

std::shared_ptr<QFile> Netlist::exists(const QString& fileName)
{
    std::shared_ptr<QFile> m_file = std::make_shared<QFile>();
    m_file->setFileName(fileName);
    if (m_file->exists())
        return m_file;
    throw std::runtime_error("ERROR: The given file path doesn't exists");
}

void Netlist::strip(QString& str) {
    int lindex = 0;
    std::string temp = str.toStdString();
    while (lindex < temp.length())
    {
        if (temp[lindex] == ' ' || temp[lindex] == '\n')
            lindex++;
        else
            break;
    }
    int rindex = temp.length();
    while (rindex > lindex)
    {
        --rindex;
        if (temp[rindex] != ' ' && temp[rindex] != '\n')
            break;
    }
    rindex++;
    temp = temp.substr(lindex, rindex - lindex);
    str = QString::fromStdString(temp);
}

void Netlist::remove_comment(QString& line)
{
    std::string temp = line.toStdString();
    size_t pos = temp.find("//");
    if (pos != std::string::npos)
        temp.replace(pos, temp.size() - 1, "");
    line = QString::fromStdString(temp);
}

EObj_Types Netlist::get_obj_type(QString& temp)
{
    //std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    temp = temp.toUpper();
    if (Gates.find(temp) != Gates.end())
        return EObj_Types::GATE;
    else if (Nets.find(temp) != Nets.end())
        return EObj_Types::NET;
    return EObj_Types::INVALID;
}

QVector<QString> Netlist::get_nets_from_line(QString& temp)
{
    QVector<QString> nets;
    bool continue_find = true;
    int start, curr_index;
    start = curr_index = 0;
    std::string line = temp.toStdString();
    while (continue_find)
    {
        curr_index = line.find(',', start);
        if (curr_index != std::string::npos)
        {
            QString temp_str = QString::fromStdString(line.substr(start, curr_index - start));
            start = curr_index + 1;
            strip(temp_str);
            nets.push_back(temp_str);
        }
        else
            continue_find = false;
    }
    QString temp_str = QString::fromStdString(line.substr(start, line.length() - start));
    strip(temp_str);
    nets.push_back(temp_str);

    return nets;
}

void Netlist::create_gate_obj(QString& gate_line, EGate_Types gate_type)
{
    strip(gate_line);
    std::string line = gate_line.toStdString();
    size_t ind = line.find('(');
    QString gate_name = QString::fromStdString(line.substr(0, ind++));
    /// This is output part
    char sep = ',';
    int next_ind = line.find(sep, ind);
    QString net_name = QString::fromStdString(line.substr(ind, next_ind - ind));
    ind = next_ind + 1;
    strip(net_name);
    std::shared_ptr<Net> output = get_net_by_name(net_name);
    /// This is inputs part
    QVector<std::shared_ptr<Net>> in_nets{};
    next_ind = line.find(sep, ind);
    while (next_ind != -1)
    {
        net_name = QString::fromStdString(line.substr(ind, next_ind - ind));
        strip(net_name);
        in_nets.push_back(get_net_by_name(net_name));
        ind = next_ind + 1;
        next_ind = line.find(sep, ind);
    }
    next_ind = line.find(')', ind);
    net_name = QString::fromStdString(line.substr(ind, next_ind - ind));
    strip(net_name);
    in_nets.push_back(get_net_by_name(net_name));
    if (in_nets.size() > 0)
    {
        m_gates[gate_name] = QVector<std::shared_ptr<Gate>>();
        std::shared_ptr<Gate> gate = std::make_shared<Gate>(gate_name, gate_type, output, in_nets);
        m_gates[gate_name].push_back(gate);
        for (auto elem : in_nets)
        {
            elem->add_connection(gate);
        }
    }
}

QString& Netlist::getline(QTextStream& text_stream, QString& text)
{
    text = "";
    QChar ch;
    while (!text_stream.atEnd())
    {
        text_stream >>(ch);
        if (ch == ';')
        {
            break;
        }
        text = text + ch;
    }
    return text;
}

void Netlist::read()
{
        std::shared_ptr<QFile> file_ = exists(m_file_path);
        file_->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inputFile(&*file_);
        QString line;

        inputFile >> line;
        while (line != "module") inputFile >> line;
        line = inputFile.readLine();
        int openBracketIndex = line.toStdString().find("(");
        QString moduleName = line.mid(1, openBracketIndex - 1);
        while (!getline(inputFile, line).isEmpty())
        {
            remove_comment(line);
            strip(line);
            if (line.isEmpty())
                continue;
            if (line == "endmodule")
                return;	// This is a point of end parsing
            size_t first_space_index = line.toStdString().find(' ');
            QString obj_type_str = line.mid(0, first_space_index);
            EObj_Types obj_type = get_obj_type(obj_type_str);
            if (obj_type == EObj_Types::INVALID)
            {
                throw std::runtime_error("ERROR: Invalid object type");
            }
            line = line.replace(0, first_space_index, "");
            if (line.isEmpty())
            {
                std::runtime_error("ERROR: Invalid line");
            }
            switch (obj_type)
            {
            case EObj_Types::GATE:
            {
                EGate_Types gate_type = Gates[obj_type_str];
                create_gate_obj(line, gate_type);
                break;
            }
            case EObj_Types::NET:
            {
                ENet_Types net_type = Nets[obj_type_str];
                QVector<QString> nets = get_nets_from_line(line);
                for (QString& elem : nets)
                {
                    std::shared_ptr<Net> net = std::make_shared<Net>(elem, net_type);
                    add_net(net);
                }
                break;
            }
            }
        }
}

void Netlist::add_net(const std::shared_ptr<Net> net)
{
    m_nets[NetType_to_Str[net->get_type()]].push_back(net);
}

void Netlist::add_gate(const std::shared_ptr<Gate> gate)
{
    m_gates[GateType_to_Str[gate->get_type()]].push_back(gate);
    QVector<std::shared_ptr<Net>> connected_nets = gate->get_connected_nets();
    for (std::shared_ptr<Net> elem : connected_nets)
    {
        elem->add_connection(gate);
    }
}

QString Netlist::get_netlist_name() const
{
    return m_file_name;
}

QString Netlist::get_netlist_path() const
{
    return m_file_path;
}

QMap<QString, QVector<std::shared_ptr<Net>>> Netlist::get_nets() const
{
    return m_nets;
}

QMap<QString, std::shared_ptr<Net>> Netlist::get_primary_input_nets()
{
    QMap<QString, std::shared_ptr<Net>> primary_inputs;
    for (auto elem : m_nets["INPUT"])
    {
        primary_inputs[elem->get_name()] = std::move(elem);
    }
    return primary_inputs;
}

QMap<QString, std::shared_ptr<Net>> Netlist::get_primary_output_nets()
{
    QMap<QString, std::shared_ptr<Net>> primary_outputs;
    for (auto elem : m_nets["OUTPUT"])
    {
        primary_outputs[elem->get_name()] = std::move(elem);
    }
    return primary_outputs;
}

QMap<QString, QVector<std::shared_ptr<Gate>>> Netlist::get_gates() const
{
    return m_gates;
}

std::shared_ptr<Net> Netlist::get_net_by_name(QString& name)
{
    QVector<std::shared_ptr<Net>> nets = m_nets["OUTPUT"];
    for (auto& elem : m_nets["OUTPUT"])
    {
        if (elem->get_name() == name)
            return elem;
    }
    for (auto& elem : m_nets["INPUT"])
    {
        if (elem->get_name() == name)
            return elem;
    }
    for (auto& elem : m_nets["WIRE"])
    {
        if (elem->get_name() == name)
            return elem;
    }
    //qDebug() << "ERORR :  Not found " << name << " net";
    throw std::runtime_error("ERORR :  Not found {name} net");
}

QString Netlist::get_path_basename(const QString& path) const
{
    std::string temp = path.toStdString();
    char sep = '/';
    size_t i = temp.rfind(sep, temp.length());
    if (i != std::string::npos) {
        return QString::fromStdString(temp.substr(i + 1, temp.length() - i));
    }

    return path;
}
