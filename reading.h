#pragma once
#include <QDate>
struct reading
{
    double temperature;
    QDate date;

    reading(double temp_, QDate date_)
    {
        temperature = temp_;
        date = date_;
    }
};
