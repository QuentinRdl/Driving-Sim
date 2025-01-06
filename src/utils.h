#ifndef UTILS_H
#define UTILS_H

namespace std {

class utils {
public:
    /**
     *
     */
    static constexpr double CONVERSION_FACTOR_KW_TO_HP = 1.34102;

    static double kw_to_hp(double kw);
    static double hp_to_kw(double hp);
};

} // std

#endif //UTILS_H
