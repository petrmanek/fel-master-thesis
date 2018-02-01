#include <iostream>
#include <random>
#include <cmath>
#include <cal/cal.h>


int track_width(double k, double wmax)
{
    double e = 2.5 * k - 0.3;
    double w = wmax * std::exp(-e*e);

    return std::max(0, std::min(256, (int) w));
}

kev_t track_energy(double k, double l, double emax)
{
    double e = 4.0 * l;
    double ev = emax * std::exp(-e*e);
    double decay = std::exp(-(k + 0.5));

    return std::max(0.0, std::min(emax, decay * ev));
}

void draw_line(kev_matrix& mat)
{
    int y = 25;
    int x0 = 10;
    int l = 150;
    double wmax = 14;
    double emax = 320e6;

    int x1 = x0 + l;

    for (int x = x0; x < x1; ++x) {
        // k = [0, 1] progress along the track length (0 = start, 1 = end)
        double k = (double) x / (double) (x1 - 1);
        int w = track_width(k, wmax);
        if (w <= 0) continue;

        for (int dy = -w; dy <= w; ++dy) {
            // l = [-1, 1] progress along the track width (-1 = left edge, 0 = center, 1 = right edge)
            double l = (double) dy / (double) w;
            mat.at(dy + y, x) = track_energy(k, l, emax);
        }
    }
}

int main(int argc, char *argv[])
{
    kev_matrix mat;
    mat.fill(0);

    draw_line(mat);

    for (coord_t i = 0; i < CHIP_DIM; ++i) {
        for (coord_t j = 0; j < CHIP_DIM; ++j) {
            if (mat.at(i, j) <= 0) continue;
            std::cout << CHIP_DIM * i + j << "\t " << mat.at(i, j) << std::endl;
        }
    }

    return 0;
}
