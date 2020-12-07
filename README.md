# COP3530-Project-3-Group-6
Climate Change Visualizer

README:

This application plots temperature data in the United States from roughly 1995 to 2020 in many major cities. The data can be fit to a linear model to observe average increase, and the most extreme tempearture readings can be displayed in a table.

The GUI application used for this project is known as Qt (https://www.qt.io/).

qcustomplot.cpp and qcustomplot.h were provided as part of the QCustomPlot library, found here:https://www.qcustomplot.com/

The .pro file is the project file which builds the project using the QMake system. This process was automated using the Qt Creator IDE.

The data used for this project is in a a modified table from https://www.kaggle.com/sudalairajkumar/daily-temperature-of-major-cities. Using the R language,
only the observations for the United States were used, and thus the columns "region" and "country" were removed for their resulting redundancy.
