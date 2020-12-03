#pragma once
#include <vector>
#include <iostream>
#include "reading.h"
using namespace std;
/*
I believe passing a vector of non-primitives would cause an issue in recursive calls
1st option: create a vector of reading pointers, reorder these pointers by temperature
2nd option: create another vector of readings, but this one will be sorted by temperature NOT date
*/

struct Heap
{
    static void swap(vector<reading>& readingsList, int A, int B)
    {
        auto temp = readingsList[A];
        readingsList[A] = readingsList[B];
        readingsList[B] = temp;
    }
    static void heapify(vector<reading>& readingsList, int index, int _size)
    {
        int smallest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        if (leftChild < _size && readingsList[leftChild] < readingsList[smallest])
            smallest = leftChild;
        if (rightChild < _size && readingsList[rightChild] < readingsList[smallest])
            smallest = rightChild;
        if (smallest != index)
        {
            Heap::swap(readingsList, smallest, index);
            heapify(readingsList, smallest, _size);
        }
    }
    static void heapSort(vector<reading>& readingsList)
    {
        int listSize = readingsList.size();
        // Build the heap
        for (int i = (listSize / 2 - 1); i >= 0; i--)
            heapify(readingsList, i, listSize);
        for (int i = listSize - 1; i > 0; i--)
        {
            Heap::swap(readingsList, 0, i);
            heapify(readingsList, 0, i);
        }
    }
    static vector<reading> heapHottest(vector<reading>& readingsList, int nth)
    {
        vector<reading> results;
        heapSort(readingsList);
        int listSize = readingsList.size();
        int nthLimit = nth;
        if (nth > listSize)
        {
            nthLimit = listSize;
        }
        for (int i = 0; i < nthLimit; i++)
        {
            // -99 indicates a null/missing value in the data
            if (readingsList[i].temperature == -99)
            {
                nthLimit++;
                continue;
            }
            else
            {
                results.push_back(readingsList[i]);
            }
        }
        // Extract n elements of the heap one at a time
        return results;
    }
    static vector<reading> heapColdest(vector<reading>& readingsList, int nth)
    {
        vector<reading> results;
        heapSort(readingsList);
        int listSize = readingsList.size();
        int nthLimit = nth;
        if (nth > listSize)
        {
            nthLimit = listSize;
        }
        for (int i = (listSize - 1); i >= (listSize - nthLimit); i--)
        {
            // -99 indicates a null/missing value in the data
            if (readingsList[i].temperature == -99)
            {
                nthLimit++;
                continue;
            }
            else
            {
                results.push_back(readingsList[i]);
            }
        }
        // Extract n elements of the heap one at a time
        return results;
    }
};

