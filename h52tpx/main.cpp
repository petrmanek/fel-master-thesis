#include <iostream>
#include <chrono>
#include <cal/cal.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <h5 file>" << std::endl;
        return 8;
    }

    std::size_t frame_count = 0;
    auto ms = measure<std::chrono::milliseconds>([&]() {
        h5source src{argv[1]};
        auto data = src.next();
        for (; data; data = src.next()) {
            if (frame_count) {
                std::cout << '#' << std::endl;
            }

            for (const auto& pixel : data->pixels) {
                const auto& [coord, kev] = pixel;
                std::cout << coord << '\t' << kev << std::endl;
            }

            ++frame_count;
        }
    });

    std::cerr << "Time elapsed:   " << ms << " ms" << std::endl;
    std::cerr << "Frames read:    " << frame_count << std::endl;
    std::cerr << "Time per frame: " << ((double) ms / (double) frame_count) << " ms" << std::endl;

    return 0;
}
