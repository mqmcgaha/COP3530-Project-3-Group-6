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
    // Comparison operators
    friend bool operator<(const reading& A, const reading& B)
    {
        if (A.temperature < B.temperature)
        {
            return true;
        }
        return false;
    }
    friend bool operator>(const  reading& A, const  reading& B)
    {
        if (A.temperature > B.temperature)
        {
            return true;
        }
        return false;
    }
    friend bool operator==(const reading& A, const reading& B)
    {
        if (A.temperature == B.temperature)
        {
            return true;
        }
        return false;
    }
    friend bool operator<=(const reading& A, const reading& B)
    {
        if (A.temperature <= B.temperature)
        {
            return true;
        }
        return false;
    }



};



