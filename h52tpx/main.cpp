#include <iostream>
#include <cal/cal.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <h5 file>" << std::endl;
        return 8;
    }

    h5source src{argv[1]};
    bool first = true;
    auto data = src.next();
    for (; data; data = src.next()) {
        if (!first) {
            std::cout << '#' << std::endl;
        }

        for (const auto& pixel : data->pixels) {
            const auto& [coord, kev] = pixel;
            std::cout << coord << '\t' << kev << std::endl;
        }

        first = false;
    }

    return 0;
}
