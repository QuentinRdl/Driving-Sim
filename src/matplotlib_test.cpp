//
// Created by keiris on 1/21/25.
//
#include <iostream>
#include <vector>
#include <map>
#include <string>
//#include "matplotlibcpp.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using std::cout;
using std::map;
using std::string;
using std::vector;

int main()
{
    vector<int> x1(10);
    vector<int> x2(10);
    vector<int> y1(10);
    vector<int> y2(10);

    for(int i = 0; i <10; ++i)
    {
        x1[i] = i;
        y1[i] = i;
        x2[i] = i;
        y2[i] = i*2;
    }

    plt::subplot(1,2,1); // <- error raised point
    plt::plot(x1,y1);
    plt::title("y=x");

    plt::subplot(1,2,2);
    plt::plot(x2,y2,"k-");
    plt::title("y=2x");

    plt::show();
}
