#include "Headers/mainwindow.h"
#include "Headers/netlist.h"
#include "Headers/simulation.h"
#include "Headers/netlist.h"

#include <QTabBar>
#include <QApplication>
#include <QScreen>
#include <QTextEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupTabBar();
}

MainWindow::~MainWindow() {

}

void MainWindow::setupTabBar() {
    QWidget *centralWidget = new QWidget(this);

    QScreen* screen = QApplication::screens().at(0);
    QSize screenSize = screen->availableSize();
    setFixedSize(QSize(screenSize.width()/2, screenSize.height()/2));

    mTabs = new QTabWidget(centralWidget);
    mTabs->setFixedSize(QSize(screenSize.width()/2, screenSize.height()/2));


    setupFileDialogTab();
    setupTextEditTab();
    setupSimulationTab();
    setCentralWidget(centralWidget);
    show();
}

void MainWindow::setupSimulationTab() {

    QWidget* timingAndSimulation = new QWidget(mTabs);
    QVBoxLayout* timingLayout = new QVBoxLayout(mTabs);

    QPushButton* startSimulationButton = new QPushButton("Strat simulation", timingAndSimulation);
    QObject::connect(startSimulationButton, &QPushButton::pressed, this, &MainWindow::startSimulation);

    mSimulationOutputTextEdit = new QTextEdit(mTabs);
    QObject::connect(this, &MainWindow::simulationOutput, this, &MainWindow::dumpSimulationOutput);


    mChartView = new QChartView();

    mChartView->setRenderHint(QPainter::Antialiasing);

    timingLayout->addWidget(startSimulationButton);
    timingLayout->addWidget(mChartView);
    timingLayout->addWidget(mSimulationOutputTextEdit);
    timingAndSimulation->setLayout(timingLayout);

    mTabs->addTab(timingAndSimulation,"Simulation");
}

void MainWindow::setupTextEditTab() {
    mTextEdit = new QTextEdit(mTabs);

    mTabs->addTab(mTextEdit,"Editor");
}

void MainWindow::setupFileDialogTab() {
    QWidget* fileDialogWidget = new QWidget(mTabs);
    QPushButton* fileDialogButton = new QPushButton("open file", fileDialogWidget);

    QObject::connect(fileDialogButton, &QPushButton::pressed, this, &MainWindow::openFile);

    mTabs->addTab(fileDialogWidget, "Select file");
}

void MainWindow::setupChartValues() {
    QChart *chart = new QChart();
    size_t distance = 2;
    auto data = mSimulation->getDumpedData();

    QCategoryAxis *axisY = new QCategoryAxis;
    axisY->setMin(2);
    axisY->setMax(data.size()*distance + 2);

    QCategoryAxis *axisX = new QCategoryAxis;
    axisX->setMin(0);
    axisX->setMax(data.begin().value().size());
    axisX->setGridLineColor(QColor("red"));

    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axisX, Qt::AlignBottom);

    for(auto el = data.begin(); el != data.end(); ++el) {
        QLineSeries* series = new QLineSeries();
        auto inputVector = el.value();

        axisY->append(el.key(), distance + 2);

        for(int i = 0; i < inputVector.size(); ++i) {
            if(i == 0 && inputVector[i] == 0) {
                series->append(i, inputVector[i] + distance);
            } else if(i == 0 && inputVector[i] == 1) {
                series->append(i, inputVector[i] + distance);
            } else if((inputVector[i] != inputVector[i - 1]) && inputVector[i] == 1) {
                series->append(i, inputVector[i - 1] + distance);
                series->append(i, inputVector[i] + distance);
            } else if((inputVector[i] != inputVector[i - 1]) && inputVector[i] == 0) {
                series->append(i, inputVector[i - 1] + distance);
                series->append(i, inputVector[i] + distance);
            } else if(inputVector[i] == 1) {
                series->append(i, inputVector[i] + distance);
            } else {
                series->append(i, inputVector[i] + distance);
            }
        }
        distance += 2;
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    chart->legend()->hide();

    mChartView->setChart(chart);
}

void MainWindow::openFile() {
    mFilePath =  QFileDialog::getOpenFileName(
                mTabs,
                "Open verilog file",
                ":/verilog_files",
                "Document files (*.v)");

    if(mFilePath.isNull() ) {
        throw std::runtime_error("File dont selected");
    }

    setEditor();
}

void MainWindow::startSimulation() {

    std::shared_ptr<Netlist> netlist = std::make_shared<Netlist>(mFilePath);
    netlist->read();

    mSimulation = std::make_shared<Simulation>(netlist);

    srand(static_cast<unsigned int>(time(nullptr)));

    auto inputNets = netlist->get_primary_input_nets();

    QVector<QMap<QString, size_t>> primaryInputs {};
    primaryInputs.resize(8);
    for(int i = 0; i < 8; ++i) {
        // generate random primary inputs
        for(const auto& el: inputNets) {
            primaryInputs[i].insert(el->get_name(), rand() % 2);
        }
    }
    qDebug()<< primaryInputs<<'\n';
    mSimulation->event_driven_simulation(primaryInputs);

    emit simulationOutput(mSimulation->getOutput());
    setupChartValues();
}

void MainWindow::dumpSimulationOutput(std::shared_ptr<QStringList> output) {
    mSimulationOutputTextEdit->clear();
    for(auto str : *output) {
        mSimulationOutputTextEdit->append(str);
    }
}

void MainWindow::setEditor() {
    std::shared_ptr<QFile> file = std::make_shared<QFile>();
    file->setFileName(mFilePath);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fileStream(&*file);

    QString line = fileStream.readLine();
    while(!line.isNull()) {
        mTextEdit->append(line);
        line = fileStream.readLine();
    }
}
