#pragma once
//This class serves as another implementation to compare to the heap implementation. Since they are in direct comparison, the reading struct containing basic operation definitons as well as the vector of readins containing all the information will remain the same.
#include "reading.h"
struct Merge {
    //std::vector<reading> readings;
     /*
     bool operator < (reading& A, reading& B)
     {
         if (A.temperature < B.temperature)
         {
             return true;
         }
         return false;
     }
     bool operator > (reading& A, reading& B)
     {
         if (A.temperature > B.temperature)
         {
             return true;
         }
         return false;
     }
     bool operator == (reading& A, reading& B)
     {
         if (A.temperature = B.temperature)
         {
             return true;
         }
         return false;
     }
     */

    static std::vector<reading> mergeHottest(std::vector<reading> readingsList, int k) {

        Merge::MergeSort(readingsList, 0, readingsList.size() - 1);
        // rbegin starts at end and increments towards beginning
        std::vector<reading> ret(readingsList.rbegin(), readingsList.rbegin() + k);
        return ret;
    }

    static std::vector<reading> mergeColdest(std::vector<reading> readingsList, int k) {
        Merge::MergeSort(readingsList, 0, readingsList.size() - 1);
        int count = 0;
        while (readingsList[count].temperature == -99)
            count++;

        std::vector<reading> ret(readingsList.begin() + count, readingsList.begin() + count + k);
        return ret;
    }
    //module 6 Sorting 2
    //Merges is a helper function that gets called by MergeSort.
    static void Merges(std::vector<reading>& readingsList, int start, int mid, int end) {
        //MergeSort breaks down the vector into smaller sub vectors recursively. Merge is the function that rebuilds the vectors in sorted order.
        std::vector<reading> r1;
        std::vector<reading> r2;
        int i1 = mid - start + 1;
        int i2 = end - mid;
        for (int i = 0; i < i1; i++)
            r1.push_back(readingsList.at(start + i));
        for (int i = 0; i < i2; i++)
            r2.push_back(readingsList.at(mid + 1 + i));
        int i = start;
        int c1 = 0;
        int c2 = 0;
        while (c1 < r1.size() && c2 < r2.size()) {
            if (r1.at(c1).temperature <= r2.at(c2).temperature) {
                readingsList[i] = r1.at(c1);
                c1++;
            }
            else {
                readingsList[i] = r2.at(c2);
                c2++;
            }
            i++;
        }

        while (c1 < r1.size()) {
            readingsList[i] = r1.at(c1);
            c1++;
            i++;
        }

        while (c2 < r2.size()) {
            readingsList[i] = r2.at(c2);
            c2++;
            i++;
        }
    }

    //module 6 Sorting 2
    //when calling this fucntion, int start = 0 and int end = readingsList.size()-1
    static void MergeSort(std::vector<reading>& readingsList, int start, int end) {
        //MergeSort calls itself recursively until it reaches the point where start and end pass eachother.
        //int mid is the midpoint of start and end.
        if (start < end) {
            int mid = (start + end) / 2;
            Merge::MergeSort(readingsList, start, mid);
            Merge::MergeSort(readingsList, mid + 1, end);
            Merge::Merges(readingsList, start, mid, end);
        }
    }


};
