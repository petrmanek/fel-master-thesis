//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_FRAME_H
#define LIBCAL_FRAME_H

#include <optional>
#include <cal/primitives.h>
#include <cal/stride.h>
#include <cal/matrix.h>

template<typename Element>
class frame {
public:
    using element_type = Element;
    using stride_type = stride<element_type>;
    using matrix_type = matrix<element_type, row_order>;

    frame(const stride_type& s) :stride_{s} {}
    virtual ~frame() = default;

    const stride_type& str() const
    {
        return stride_;
    }

    const matrix_type& mat()
    {
        if (!matrix_) {
            // Lazy conversion.
            matrix_ = std::make_optional(stride_);
        }

        return *matrix_;
    }

private:
    stride_type stride_;
    std::optional<matrix_type> matrix_;

};

using count_frame = frame<count_t>;
using kev_frame = frame<kev_t>;

#endif //LIBCAL_FRAME_H
