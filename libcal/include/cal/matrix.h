//
// Created by petr on 2.1.18.
//

#ifndef LIBCAL_MATRIX_H
#define LIBCAL_MATRIX_H

#include <array>
#include <tuple>
#include <cal/primitives.h>
#include <cal/stride.h>

struct row_order
{
    std::size_t operator() (coord_t i, coord_t j) const
    {
        return CHIP_DIM * i + j;
    }

    std::tuple<coord_t, coord_t> inv(std::size_t c) const
    {
        return std::make_tuple((coord_t) (c / CHIP_DIM), (coord_t) (c % CHIP_DIM));
    }
};

template<typename Element, typename StoragePolicy = row_order>
class matrix {
public:
    using element_type = Element;
    using storage_type = std::array<element_type, CHIP_PIXELS>;

    using storage_policy_type = StoragePolicy;
    static const storage_policy_type storage_policy;

    matrix() = default;

    template<typename StrideValue>
    matrix(const stride<StrideValue>& s)
    {
        for (const auto& p : s.pixels) {
            const auto& [coord, val] = p;
            data_[coord] = val;
        }
    }

    virtual ~matrix() = default;

    constexpr element_type& at(coord_t i, coord_t j)
    {
        return data_[storage_policy(i, j)];
    }

    constexpr const element_type& at(coord_t i, coord_t j) const
    {
        return data_[storage_policy(i, j)];
    }

    void fill(const element_type& value)
    {
        std::fill(data_.begin(), data_.end(), value);
    }

private:
    storage_type data_;

};

template<typename Element, typename StoragePolicy>
const StoragePolicy matrix<Element, StoragePolicy>::storage_policy{};

using kev_matrix = matrix<kev_t>;
using count_matrix = matrix<count_t>;

#endif //LIBCAL_MATRIX_H
