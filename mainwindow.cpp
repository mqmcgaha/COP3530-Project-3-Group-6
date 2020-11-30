#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

// Data storage


// Data parsing
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Custom classes
#include "Series.h"
#include "reading.h"







 // Need this maybe?
// struct City
// {
//     QString name;
//     QDate startDate;
//     QDate endDate;
//     vector<reading> readings;

//     City(QString name_)
//     {
//         name = name_;
//     }
//     void insertReading(double temp_, int month_, int day_, int year_)
//     {
//         QDate tempDate(year_, month_, day_);
//         if(readings.size() == 0)
//             startDate = tempDate;

//        readings.push_back(reading(temp_, tempDate));


//     }

// };






void MainWindow::read()
{
    std::ifstream file("../fileIONullsDiscardedOnInitialRead/NewData.csv");

     std::string line;

    if(file.is_open())
    {
        getline(file, line); // skip header
        while(getline(file, line))
        {
              std::istringstream hold(line); // store

              getline(hold, line, ','); // parse from stream
              QString state = QString::fromStdString(line);
              state.remove(0, 1);
              state.remove(state.length() - 1, 1);

              //ui->stateBox->addItem(state);

              getline(hold, line, ',');
              QString city = QString::fromStdString(line);
              city.remove(0, 1);
              city.remove(city.length() - 1, 1);

              getline(hold, line, ',');
              int month = stoi(line);

              getline(hold, line, ',');
              int day = stoi(line);

              getline(hold, line, ',');
              int year = stoi(line);

              getline(hold, line, ',');
              double temp = stod(line);

              mapper[state].insert(city);
              statecityToData[std::make_pair(state, city)].push_back(reading(temp, QDate(year, month, day)));
         }

    }

}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Read data
    read();

    // Set up the drop boxes for states
    for(auto iter = mapper.begin(); iter != mapper.end(); iter++)
    {
           ui->stateBox->addItem(iter->first);
    }

    // Disable fit until plot is pushed first
    ui->pushButtonFit->setEnabled(false);

    // Set up the extreme box
    ui->lineEditNumDaysExtreme->setText("5");
    ui->radioButtonColdest->setChecked(true);

    // Set up bools
    fitMade = false;

    // Set up initial axes
    ui->customPlot->xAxis->setLabel("Time (Days since start)");
    ui->customPlot->yAxis->setLabel("Temperature (°F)");

    // Initial Y axis
    ui->customPlot->yAxis->setRange(-50.0, 150.0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_stateBox_currentTextChanged(const QString &state)
{
    ui->cityBox->clear(); // Reset menu
    std::set<QString> cities = mapper[state]; // Get selected state's cities
    for(auto iter = cities.begin(); iter != cities.end(); iter++) // Insert cities into dropdown menu
           ui->cityBox->addItem(*iter);

    // Disable fitting
    ui->pushButtonFit->setEnabled(false);
}

void MainWindow::on_cityBox_currentTextChanged(const QString &city)
{
    // With the city, must change the available dates
    QString state = ui->stateBox->currentText();
   std::vector<reading> data = statecityToData[std::make_pair(state, city)];
    if(data.size() == 0)
    {
        ui->labelStartTime->setText("First available: ");
        ui->labelEndTime->setText("Last available: ");
         return;
    }


   QDate start = data.front().date;
   QDate end = data.back().date;

   QString dateString = start.toString("M/d/yyyy");
   ui->labelStartTime->setText("First available: " + dateString);

   QString dateString2 = end.toString("M/d/yyyy");
   ui->labelEndTime->setText("Last available: " + dateString2);

   // Disable fitting
   ui->pushButtonFit->setEnabled(false);

}



void MainWindow::on_pushButtonPlot_clicked()
{
    // Data
    QString state = ui->stateBox->currentText();
    QString city = ui->cityBox->currentText();
    currentCityData = statecityToData[std::make_pair(state, city)];
    QDate dataBegin =currentCityData.front().date;
    QDate dataEnd = currentCityData.back().date;

    // User-provided dates
    QDate selectedStart = ui->dateEditFrom->date();
    QDate selectedEnd = ui->dateEditTo->date();


    // If any selection placed before (after) the beginning (end), invalid range
    if((selectedStart < dataBegin) || (selectedStart > dataEnd) || (selectedEnd < dataBegin) || (selectedEnd > dataEnd))
    {
        QMessageBox::warning(this, "Invalid Date Range", "Selected date out of bounds of data. Please see available dates below.");
        return;
    }
    // If selected start is after selected end in time, invalid range
    if(selectedStart > selectedEnd)
    {
        QMessageBox::warning(this, "Invalid Date Range", "Start date greater than end date.");
        return;
    }

    // Else, we may proceed to plot the data

    // First fine the indices of the selected dates to create a subset of the data
    int startIndex = 0;
    int endIndex = currentCityData.size() - 1;
    for(unsigned int i = 0; i < currentCityData.size(); i++)
    {
        if(currentCityData.at(i).date == selectedStart)
            startIndex = i;
        if(currentCityData.at(i).date == selectedEnd)
           endIndex = i;
    }
    // Now create a subsetted vector
    std::vector<reading> subset(currentCityData.begin() + startIndex, currentCityData.begin() + endIndex + 1);

    // Now treat the data to get it ready to plot. Must turn dates into indices--the x-axis, and set temp to y axis
    QVector<double> x(subset.size());
    QVector<double> y(subset.size());

    // We are plotting the -99 points but they will not show up on the data (set the axis to about -50 to 150);
    // May have to revise this if we are doing linear regression
    for(unsigned int i = 0; i < subset.size(); i++)
    {
        x[i] = i;
        y[i] = subset.at(i).temperature;

    }

    // Get plot variable
    auto& myPlot = ui->customPlot;
    myPlot->addGraph();
    myPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    myPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    myPlot->graph(0)->setData(x, y, true);

    // labels
    myPlot->xAxis->setLabel("Time (Days since start)");
    myPlot->yAxis->setLabel("Temperature (°F)");

    // axes ranges
    myPlot->xAxis->setRange(0, subset.size());
    myPlot->yAxis->setRange(-50.0, 150.0);

    // Clear the linear graph if any
    if(fitMade)
        myPlot->graph(1)->data()->clear();

    myPlot->replot();



//         QVector<double> x(city.length()), y(city.length());

//         for(int i = 0; i < city.length(); i++)
//         {
//             x[i] = i;
//             y[i] = i;
//         }

//         // add graph and assign data
//         myPlot->addGraph();
//         myPlot->graph(0)->setData(x, y);

//         // set labels
//         myPlot->xAxis->setLabel("x");
//         myPlot->yAxis->setLabel("y");

//         // set ranges
//         myPlot->xAxis->setRange(0, 20);
//         myPlot->yAxis->setRange(0, 20);

//         myPlot->replot();


    // Enable fitting
    ui->pushButtonFit->setEnabled(true);
}


void MainWindow::on_pushButtonFit_clicked()
{
    // Data
    QString state = ui->stateBox->currentText();
    QString city = ui->cityBox->currentText();
    std::vector<reading> myData = statecityToData[std::make_pair(state, city)];
    QDate dataBegin = myData.front().date;
    QDate dataEnd = myData.back().date;

    // User-provided dates
    QDate selectedStart = ui->dateEditFrom->date();
    QDate selectedEnd = ui->dateEditTo->date();


    // If any selection placed before (after) the beginning (end), invalid range
    if((selectedStart < dataBegin) || (selectedStart > dataEnd) || (selectedEnd < dataBegin) || (selectedEnd > dataEnd))
    {
        QMessageBox::warning(this, "Invalid Date Range", "Selected date out of bounds of data. Please see available dates below.");
        return;
    }
    // If selected start is after selected end in time, invalid range
    if(selectedStart > selectedEnd)
    {
        QMessageBox::warning(this, "Invalid Date Range", "Start date greater than end date.");
        return;
    }

    // Else, we may proceed to plot the data

    // First fine the indices of the selected dates to create a subset of the data
    int startIndex = 0;
    int endIndex = myData.size() - 1;
    for(unsigned int i = 0; i < myData.size(); i++)
    {
        if(myData.at(i).date == selectedStart)
            startIndex = i;
        if(myData.at(i).date == selectedEnd)
           endIndex = i;
    }
    // Now create a subsetted vector
    std::vector<reading> subset(myData.begin() + startIndex, myData.begin() + endIndex + 1);

    // Now treat the data to get it ready to plot. Must turn dates into indices--the x-axis, and set temp to y axis
    QVector<double> x(subset.size());
    QVector<double> y(subset.size());

    // We are plotting the -99 points but they will not show up on the data (set the axis to about -50 to 150);
    // May have to revise this if we are doing linear regression
    for(unsigned int i = 0; i < subset.size(); i++)
    {
        x[i] = i;
        y[i] = subset.at(i).temperature;

    }

    // Given now, the x and y, perform linear regression

    // XY term
    std::pair<double, int> pairedXY = Series::sumSeriesProduct(x, y);
     double sumXY = pairedXY.first;
    int N = pairedXY.second;


    // X and Y (Parameters: data, squareTerm, squareResult)
    double sumX = Series::sumSeries(x, false, false);
    double sumY = Series::sumSeries(y, false, false);

    // sum(X^2) and sum(X)^2
    double sumX_TermsSquared = Series::sumSeries(x, true, false);
    double sumX_ResultSquared = Series::sumSeries(x, false, true);

    double numerator = N*sumXY - sumX * sumY;
    double denominator = N*sumX_TermsSquared - sumX_ResultSquared;

    double m = numerator / denominator;
    double b = (sumY - m * sumX) / N;

    // Create regression line
    QVector<double> xReg = Series::removeNulls(x, y);
    QVector<double> yReg(N);


//    qDebug() <<"N: "<< N << " X size: " << xReg.size() << Qt::endl;
//    qDebug() <<"sub.size(): "<< subset.size() << " PairedResult: " << pairedXY.second << Qt::endl;
//    for(int i = 0; i < xReg.size(); i++)
//    {
//        qDebug() << y[xReg[i]] << " ";
//    }

    for(int i = 0; i < xReg.size(); i++)
    {
        yReg[i] = m*xReg[i] + b;
    }

    // Get plot variable
    auto& myPlot = ui->customPlot;
    myPlot->addGraph();
    myPlot->graph(1)->setData(xReg, yReg, true);

    // labels
    myPlot->xAxis->setLabel("Time (Days since start)");
    myPlot->yAxis->setLabel("Temperature (°F)");

    // axes ranges
    myPlot->xAxis->setRange(0, subset.size());
    myPlot->yAxis->setRange(-50.0, 150.0);

    // Color the line

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    myPlot->graph(1)->setPen(pen);
    myPlot->replot();

    // Set results to labels
    QString mStr = QString::number(m, 'f', 5);
    QString bStr = QString::number(b, 'f', 5);

    ui->labelM->setText("m: " + mStr + " °F / day");
    ui->labelB->setText("b: " + bStr + " °F");


    fitMade = true;


}

void MainWindow::on_pushButtonExtremeUpdate_clicked()
{
    QString numDaysStr = ui->lineEditNumDaysExtreme->text();

    // Pass into string conversion. Turns it to false if failed
    bool ok;
    int numDays = numDaysStr.toInt(&ok);

    // Return and advise user to enter a valid  argument
    if(!ok)
    {
        QMessageBox::warning(this, "Invalid Argument", "Number of days must be an integer.");
        return;
    }

    // Proceed with adding to list if valid



}

void MainWindow::on_dateEditFrom_userDateChanged(const QDate &date)
{
    ui->pushButtonFit->setEnabled(false);

}

void MainWindow::on_dateEditTo_userDateChanged(const QDate &date)
{
    ui->pushButtonFit->setEnabled(false);

}
