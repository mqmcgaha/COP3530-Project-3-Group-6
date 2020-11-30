#pragma once

#include <QMainWindow>

// Data Storage
#include <map>
#include <set>
#include <utility>

// Custom
#include "reading.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void read();

private slots:
    void on_stateBox_currentTextChanged(const QString &state);
    void on_pushButtonPlot_clicked();

    void on_cityBox_currentTextChanged(const QString &city);

    void on_pushButtonFit_clicked();

    void on_pushButtonExtremeUpdate_clicked();

    void on_dateEditFrom_userDateChanged(const QDate &date);

    void on_dateEditTo_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *ui;

    // Main data storage
    std::map<QString, std::set<QString>> mapper;
    std::map<std::pair<QString, QString>, std::vector<reading>> statecityToData;

    // City data (plotting time)
    std::vector<reading> currentCityData;

    // Plotting logic helpers
    bool fitMade;
};

