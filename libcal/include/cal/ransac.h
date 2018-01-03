//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_RANSAC_H
#define LIBCAL_RANSAC_H

#include <cal/frame.h>

template<typename Element>
struct ransac
{
    using element_type = Element;

    static constexpr auto name = "RANSAC";

    template<typename OutputIt>
    void operator() (const frame<element_type>& f, OutputIt it) const
    {
        // TODO: Implement me.
    }
};

#endif //LIBCAL_RANSAC_H
