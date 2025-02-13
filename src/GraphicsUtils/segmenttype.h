//
// Created by rgld_ on 24/01/25.
//

#ifndef SEGMENTTYPE_H
#define SEGMENTTYPE_H
#include <string>

#define BASE_PATH "/home/rgld_/01-dev-projects/cpp-projects/Driving-Sim/images/"


class SegmentType {

public:

    enum Value {
        SMALL_STRAIGHT,
        LONG_STRAIGHT,
        SMALL_TURN,
        MEDIUM_TURN,
        LARGE_TURN,
        S_TURN,
        U_TURN
    };

private:
    explicit SegmentType(const Value v): value(v) {}
    Value value;

    std::string getPath() const {
        std::string path;
        switch (this->value) {
            case SMALL_STRAIGHT:
                path = "small_straight.png";
            break;
            case LONG_STRAIGHT:
                path = "long_straight.png";
            break;
            case SMALL_TURN:
                path = "small_turn.png";
            break;
            case MEDIUM_TURN:
                path = "medium_turn.png";
            break;
            case LARGE_TURN:
                path = "large_turn.png";
            break;
            case S_TURN:
                path = "s_turn.png";
            break;
            case U_TURN:
                path = "u_turn.png";
            break;
            default: throw std::invalid_argument("Invalid segmentType: not implemented yet maybe ?");
        }

        return BASE_PATH + path;
    }

public:
    static std::string getPath(const Value value) {
        const SegmentType type(value);
        return type.getPath();
    }
};



#endif //SEGMENTTYPE_H
