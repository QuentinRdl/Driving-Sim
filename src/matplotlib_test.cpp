//
// Created by keiris on 1/21/25.
//
#include <iostream>
#include <vector>
#include <map>
#include <string>
//#include "matplotlibcpp.h"
#include "matplotlibcpp.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
int main() {
    plt::backend("Qt5Agg");
    plt::plot({1,3,2,4});
    plt::show();
}