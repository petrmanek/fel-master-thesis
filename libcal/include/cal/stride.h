//
// Created by petr on 1.1.18.
//

#ifndef LIBCAL_STRIDE_H
#define LIBCAL_STRIDE_H

#include <tuple>
#include <vector>
#include <cal/primitives.h>

template<typename Value>
struct stride {
    using value_type = Value;
    using pixel_type = std::tuple<coord_t, value_type>;

    stride() = default;
    virtual ~stride() = default;

    std::vector<pixel_type> pixels;
};

using count_stride = stride<count_t>;
using kev_stride = stride<kev_t>;

#endif //LIBCAL_STRIDE_H
