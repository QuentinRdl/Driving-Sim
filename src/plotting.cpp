//
// Created by keiris on 09/03/25.
//

#include "plotting.h"
#include "vehicle.h"

void plot_etape(
    std::vector<std::pair<double,double>> vx_data, std::vector<std::pair<double,double>>vy_data,
    std::vector<std::pair<double,double>>r_data, std::vector<std::pair<double,double>> traj_data,
    std::vector<std::pair<double,double>> slip_data, std::string path,
    vehicleData *data)
    {
    // Création d'un objet Gnuplot pour générer les fichiers images
    Gnuplot gp;
    // Plot de vx
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/vx_bicycle.png'\n";
    gp << "set title 'Vitesse Longitudinale (vx) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vx (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vx'\n";
    gp.send1d(vx_data);
    gp << "unset output\n";
    gp.flush();


    // Plot de vy
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/vy_bicycle.png'\n";
    gp << "set title 'Vitesse Latérale (vy) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vy (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vy'\n";
    gp.send1d(vy_data);
    gp << "unset output\n";
    gp.flush();

    // Plot de r (taux de lacet)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/r_bicycle.png'\n";
    gp << "set title 'Taux de Lacet (r) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'r (rad/s)'\n";
    gp << "plot '-' with lines lw 2 title 'r'\n";
    gp.send1d(r_data);
    gp << "unset output\n";
    gp.flush();

    // Plot de la trajectoire (x en fonction de y)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/trajectory.png'\n";
    gp << "set title 'Trajectoire du Véhicule'\n";
    gp << "set xlabel 'Position X (m)'\n";
    gp << "set ylabel 'Position Y (m)'\n";
    gp << "plot '-' with lines lw 2 title 'Trajectoire'\n";
    gp.send1d(traj_data);
    gp << "unset output\n";
    gp.flush();

    if (slip_data.empty() == true) {
        std::cout << path << " : Plotting slip data is not possible, as the data is empty" << std::endl;
        return;
    }

    // Plot de slip_data (si possible)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/slip_data.png'\n";
    gp << "set title 'Donnés sur le slip angle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'Slip (rad)'\n";
    gp << "plot '-' with lines lw 2 title 'vy'\n";
    gp.send1d(slip_data);
    gp << "unset output\n";
    gp.flush();

}
