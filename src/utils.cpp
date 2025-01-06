#include "utils.h"

namespace Driving_Sim {
    double utils::kw_to_hp(const double kw) {
        return CONVERSION_FACTOR_KW_TO_HP * kw;
    }

    double utils::hp_to_kw(const double hp) {
        return hp / CONVERSION_FACTOR_KW_TO_HP;
    }
} // std