//
// Created by keiris on 06/03/25.
//

#ifndef PLOTTING_H
#define PLOTTING_H

#include <vector>
#include <string>
#include <gnuplot-iostream.h>

void plot_etape(
    std::vector<std::pair<float, float>> vx_data, std::vector<std::pair<float, float>>vy_data,
    std::vector<std::pair<float, float>>r_data, std::vector<std::pair<float, float>> traj_data,
    std::vector<std::pair<float, float>> slip_data, std::string path);

#endif //PLOTTING_H
