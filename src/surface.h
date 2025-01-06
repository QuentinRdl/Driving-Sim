#ifndef SURFACE_H
#define SURFACE_H

#include <string>

namespace std {

class surface {

private:
    std::string name_;
    double frictionCoeff_ = 0;

public:
    surface(std::string name, double frictionCoeff);

    std::string getName() const;
    double getFrictionCoeff() const;

    void display() const;

};

} // std

#endif //SURFACE_H
