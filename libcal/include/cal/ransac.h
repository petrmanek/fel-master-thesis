//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_RANSAC_H
#define LIBCAL_RANSAC_H

#include <optional>
#include <array>
#include <random>
#include <algorithm>
#include <list>
#include <cal/frame.h>


template<typename Element>
struct greedy_ransac
{
    using element_type = Element;
    using pixel_type = typename frame<element_type>::stride_type::pixel_type;

    using sample_array = std::array<pixel_type, 2>;
    using pixel_list = std::list<pixel_type>;

    static constexpr auto name = "Greedy RANSAC";

    template<typename OutputIt>
    void operator() (const frame<element_type>& f, OutputIt it) const
    {
        // Copy the pixel stride.
        pixel_list pixels{f.str().pixels.cbegin(), f.str().pixels.cend()};

        std::array<pixel_type, 2> samples{};
        std::vector<double> inliers{};
        inliers.resize(pixels.size());

        std::mt19937 generator{std::random_device{}()};

        while (pixels.size() >= 2) {
            auto edge = next_edge(pixels, samples, inliers, generator);

            if (!edge) {
                // Something wrong. RANSAC did not find an edge even though there is enough samples.
                break;
            }

            *it++ = *edge;

            // TODO: Remove the support of the edge from pixels.
        }
    }

    template<typename GeneratorType>
    std::optional<edge> next_edge(pixel_list& pixels, sample_array& samples, std::vector<double>& inliers, GeneratorType& generator) const
    {
        uint32_t iterations{1};

        double best_support{-1};
        std::optional<edge> best_model{std::nullopt};

        for (uint32_t i = 0; i < iterations; ++i) {
            // Sample points at random.
            std::sample(pixels.cbegin(), pixels.cend(), samples.begin(), 2, generator);

            // Estimate model.
            const auto& [coord0, val0] = samples[0];
            const auto& [coord1, val1] = samples[1];
            edge model{coord0, coord1};

            // Calculate support.
            double support = calculate_support(pixels.cbegin(), pixels.cend(), model, inliers.begin());

            // Improve.
            if (support > best_support) {
                best_support = support;
                best_model = std::make_optional(model);
            }

            // Estimate number of iterations.
            iterations = max_iterations();
        }

        return best_model;
    }

    template<typename InputIt, typename OutputIt>
    double calculate_support(InputIt first, InputIt last, const edge& e, OutputIt it) const
    {

    }

    uint32_t max_iterations() const
    {
        // FIXME: Calculate number of iterations based on the desired probability of good sampling.
        return 0;
    }
};

#endif //LIBCAL_RANSAC_H
