//
// Created by keiris on 06/03/25.
//

#ifndef PLOTTING_H
#define PLOTTING_H

#include "Physics/vehicle.h"
#include <vector>
#include <string>
#include <gnuplot-iostream.h>

void plot_etape(
    std::vector<std::pair<double,double>> vx_data, std::vector<std::pair<double,double>>vy_data,
    std::vector<std::pair<double,double>>r_data, std::vector<std::pair<double,double>> traj_data,
    std::vector<std::pair<double,double>> slip_data, std::string path, vehicleData *data);

#endif //PLOTTING_H
