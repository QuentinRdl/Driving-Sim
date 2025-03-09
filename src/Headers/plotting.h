//
// Created by keiris on 06/03/25.
//

#ifndef PLOTTING_H
#define PLOTTING_H

#include <vector>
#include <string>
#include <gnuplot-iostream.h>
#include <filesystem> // Etre sûr que le fichier à plot existe
#include "vehicle.h"

class Plotting {
public:
    void plot_etape(
        std::vector<std::pair<float, float>> vx_data, std::vector<std::pair<float, float>>vy_data,
        std::vector<std::pair<float, float>>r_data, std::vector<std::pair<float, float>> traj_data,
        std::vector<std::pair<float, float>> slip_data, std::string path);

    void convertToArray(
        std::vector<std::pair<float, float>>& vx_data, std::vector<std::pair<float, float>>& vy_data,
        std::vector<std::pair<float, float>>& r_data, std::vector<std::pair<float, float>>& traj_data,
        std::vector<std::pair<float, float>>& slip_data, vehicleData *data, size_t size);

    void plotStepFromArray(vehicleData *data, size_t size, std::string path);

};

#endif //PLOTTING_H
