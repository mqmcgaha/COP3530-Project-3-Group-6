#pragma once
#include <QVector>
#include <utility>
#include <QDebug>
struct Series
{

    static double sumSeries(const QVector<double>& first, const QVector<double>& second, bool squareTerm, bool squareResult)
    {
        double sum = 0;
        for (int i = 0; i < first.size(); i++)
        {
            qDebug() << "Current value: " << first[i] << Qt::endl;

            // X will never go negative, so this accounts for whether x or y should be summed correctly.
            if(first[i] == -99 || second[i] == -99)
                continue;

            if (squareTerm)
                sum += first[i] * first[i];
            else
                sum += first[i];
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
            if(y[i] == -99)
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
            if(y[i] == -99)
                continue;
            pureX.push_back(x[i]);

        }
        return pureX;

    }

    static std::tuple<QVector<double>, QVector<double>, double, double> regress(const QVector<double>& x, const QVector<double>& y)
    {
        // XY term
        std::pair<double, int> pairedXY = Series::sumSeriesProduct(x, y);
        double sumXY = pairedXY.first;
        int N = pairedXY.second;

        // X and Y (Parameters: data, squareTerm, squareResult)
        double sumX = Series::sumSeries(x, y, false, false);
        double sumY = Series::sumSeries(y, x, false, false);

        // sum(X^2) and sum(X)^2
        double sumX_TermsSquared = Series::sumSeries(x, y, true, false);
        double sumX_ResultSquared = Series::sumSeries(x, y, false, true);

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

        qDebug() << "xReg contents:\n";
        for(int i = 0; i < xReg.size(); i++)
        {
            yReg[i] = m*xReg[i] + b;
        }
        for(int i = 0; i < xReg.size(); i++)
        {
            qDebug() <<yReg[i] << " ";
        }

        qDebug() << "N: " << N <<  Qt::endl;
        qDebug() << "sumX: " << sumX << Qt::endl;
         qDebug() << "sumY: " << sumY << Qt::endl;
         qDebug() << "sumX_termssquared :" << sumX_TermsSquared << Qt::endl;
         qDebug() << "sumX_ResultSquared :" << sumX_ResultSquared << Qt::endl;


        qDebug() << Qt::endl;

        return std::make_tuple(xReg, yReg, m, b);
    }


};
