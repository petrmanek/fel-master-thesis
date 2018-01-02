//
// Created by petr on 2.1.18.
//

#ifndef LIBCAL_H5_H
#define LIBCAL_H5_H

#include <array>
#include <optional>
#include <H5Cpp.h>
#include <cal/stride.h>

class h5source
{
    H5::H5File file_;
    H5::DataSet pixels_;
    hsize_t dims_[2];
    hsize_t next_read_idx_;
    hsize_t next_buffer_idx_;

    static constexpr hsize_t BUFFER_SIZE = 1024;
    std::array<double, 2 * BUFFER_SIZE> buffer_;
    hsize_t valid_range_;

public:
    h5source(const std::string& path);
    virtual ~h5source() = default;

    std::optional<kev_stride> next();

};

#endif //LIBCAL_H5_H
