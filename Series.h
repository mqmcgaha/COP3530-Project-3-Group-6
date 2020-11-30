#pragma once
#include <QVector>
#include <utility>
struct Series
{

    static double sumSeries(const QVector<double>& data, bool squareTerm, bool squareResult)
    {
        double sum = 0;
        for (int i = 0; i < data.size(); i++)
        {
            if(((int)data[i]) == -99)
                continue;
            else if (squareTerm)
                sum += data[i] * data[i];
            else
                sum += data[i];
        }
        if (squareResult)
            return sum * sum;
        else
            return sum;

    }

    static std::pair<double, int> sumSeriesProduct(const QVector<double>& x, const QVector<double>& y)
    {
        double sum = 0;
        int numSkips = 0;
        if (x.size() != y.size())
            return std::make_pair(-9999999, -9999999);
        for (int i = 0; i < x.size(); i++)
        {
            if( ((int)(y[i])) == -99)
            {
                numSkips++;
                continue;
            }
            sum += x[i] * y[i];
        }

        return std::make_pair(sum, x.size() - numSkips);
    }

    static QVector<double> removeNulls(const QVector<double>& x, const QVector<double>& y)
    {
        QVector<double> pureX;
        for(int i = 0; i < x.size(); i++)
        {
            if(((int)y[i]) == -99)
                continue;
            pureX.push_back(x[i]);

        }
        return pureX;

    }
};
