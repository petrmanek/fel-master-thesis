//
// Created by petr on 2.1.18.
//

#include <cal/h5.h>

h5source::h5source(const std::string &path)
        :file_{path, H5F_ACC_RDONLY}, pixels_{file_.openDataSet("pixels")},
         dims_{0, 0}, next_read_idx_{0}, next_buffer_idx_{0}, valid_range_{0}
{
    H5::DataSpace space = pixels_.getSpace();
    space.getSimpleExtentDims(dims_);
}


std::optional<kev_stride> h5source::next()
{
    kev_stride stride;
    bool reading = true;

    H5::FloatType data_type{H5::PredType::NATIVE_DOUBLE};
    H5::DataSpace file_space{2, dims_};

    while (reading) {
        if (next_buffer_idx_ == valid_range_) {
            if (next_read_idx_ >= dims_[0]) {
                // No more data.
                return std::nullopt;
            }

            // Determine how much data will be read.
            hsize_t count[] = {BUFFER_SIZE, 2};
            hsize_t start[] = {next_read_idx_, 0};

            if (BUFFER_SIZE > dims_[0] - next_read_idx_) {
                // Last read may not utilize the entire buffer.
                count[0] = dims_[0] - next_read_idx_;
            }

            H5::DataSpace mem_space{2, count};
            file_space.selectHyperslab(H5S_SELECT_SET, count, start);

            // Read HDF5.
            pixels_.read(buffer_.begin(), data_type, mem_space, file_space);

            // Move on.
            valid_range_ = count[0];
            next_buffer_idx_ = 0;
            next_read_idx_ += count[0];
        }

        const double& coord = buffer_[next_buffer_idx_ * 2 + 0];
        const double& kev = buffer_[next_buffer_idx_ * 2 + 1];

        if (coord < 0 && kev < 0) {
            // End of frame. Stop reading.
            reading = false;
        } else {
            stride.pixels.emplace_back((coord_t) coord, kev);
        }

        ++next_buffer_idx_;
    }

    return std::make_optional(stride);
}
