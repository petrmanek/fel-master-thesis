#include <iostream>
#include <list>
#include <cal/cal.h>

template<typename EdgeDetector>
void evaluate_detection(const kev_frame& f)
{
    EdgeDetector detector;
    std::list<edge> edges;

    auto ms = measure<std::chrono::milliseconds>([&]() {
        detector(f, std::back_inserter(edges));
    });

    std::cout << "  - " << EdgeDetector::name << "\t " << edges.size() << "\t " << ms << " ms" << std::endl;

    // TODO: evaluate some quality criterion
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <h5 file> [h5 files ...]" << std::endl;
        return 8;
    }

    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << std::endl;

        h5source src{argv[i]};
        auto data = src.next();
        for (; data; data = src.next()) {
            kev_frame frame{*data};
            frame.mat();

            evaluate_detection<greedy_ransac<kev_t>>(frame);
        }

        std::cout << std::endl;
    }



    return 0;
}
