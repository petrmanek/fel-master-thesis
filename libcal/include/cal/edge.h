//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_EDGE_H
#define LIBCAL_EDGE_H

#include <cal/primitives.h>

struct edge {
    coord_t start;
    coord_t end;

    edge(coord_t s, coord_t e) :start{s}, end{e} { }

};

#endif //LIBCAL_EDGE_H
