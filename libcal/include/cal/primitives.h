//
// Created by petr on 2.1.18.
//

#ifndef LIBCAL_PRIMITIVES_H
#define LIBCAL_PRIMITIVES_H

#include <cstdint>

using coord_t = uint16_t;
using count_t = uint16_t;
using kev_t = double;

static constexpr std::size_t CHIP_DIM = 256u;
static constexpr std::size_t CHIP_PIXELS = CHIP_DIM * CHIP_DIM;

#endif //LIBCAL_PRIMITIVES_H
