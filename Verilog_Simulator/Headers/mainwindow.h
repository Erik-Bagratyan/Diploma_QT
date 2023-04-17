#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <QtCharts>

#include <memory>

class QTabBar;
class QTextEdit;
class QTabWidget;
class Simulation;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    void setupTabBar();
    void setupSimulationTab();
    void setupTextEditTab();
    void setupFileDialogTab();
public:
    void setupChartValues();
private:
    void setEditor();
private slots:
    void openFile();
    void startSimulation();
    void dumpSimulationOutput(std::shared_ptr<QStringList> output);
signals:
    void simulationOutput(std::shared_ptr<QStringList> output);
private:
    std::shared_ptr<QTabBar> mTabBar;
    std::shared_ptr<Simulation> mSimulation;
    QString mFilePath;
    QTextEdit* mTextEdit;
    QTextEdit* mSimulationOutputTextEdit;
    QTabWidget* mTabs;
    QChartView * mChartView;
};

#endif // MAINWINDOW_H
