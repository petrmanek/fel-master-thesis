//
// Created by petr on 1.1.18.
//

#ifndef LIBCAL_STRIDE_H
#define LIBCAL_STRIDE_H

#include <cstdint>
#include <vector>

using coord_t = uint16_t;
using count_t = uint16_t;

struct stride {
    std::vector<coord_t> coords;
    std::vector<count_t> counts;
};

#endif //LIBCAL_STRIDE_H
