//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_RANSAC_H
#define LIBCAL_RANSAC_H

#include <optional>
#include <array>
#include <random>
#include <algorithm>
#include <cmath>
#include <cal/frame.h>
#include <cal/edge.h>


template<typename Element>
struct ransac
{
    using element_type = Element;
    using pixel_type = typename frame<element_type>::stride_type::pixel_type;
    using storage_policy_type = typename frame<element_type>::matrix_type::storage_policy_type;
    using sample_array = std::array<pixel_type, 2>;

    static constexpr auto name = "RANSAC";

    template<typename OutputIt>
    void operator() (const frame<element_type>& f, OutputIt it) const
    {
        const auto& pixels = f.str().pixels;

        std::array<pixel_type, 2> samples{};
        std::vector<double> inliers{};
        inliers.resize(pixels.size());

        std::mt19937 generator{std::random_device{}()};

        {
            // FIXME: Figure out the repetition scheme of this block.

            auto edge = next_edge(f, samples, inliers, generator);
            if (edge) {
                *it++ = *edge;
            }

        }
    }

    template<typename GeneratorType>
    std::optional<edge> next_edge(const frame<element_type>& f, sample_array& samples, std::vector<double>& inliers, GeneratorType& generator) const
    {
        const auto& pixels = f.str().pixels;
        const double n = pixels.size();
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

            if (support > best_support) {
                // Improve.
                local_optimization(f, model);
                best_model = std::make_optional(model);
                best_support = calculate_support(pixels.cbegin(), pixels.cend(), model, inliers.begin());

                // Estimate number of iterations.
                iterations = max_iterations(support, n);
            }
        }

        return best_model;
    }

    double obj_func(int x1, int y1, int x2, int y2) const
    {
        // FIXME: Calculate some integral in the frame.
        return 0;
    }

    void local_optimization(const frame<element_type>& f, edge& e) const
    {
        const auto& storage_policy = frame<element_type>::matrix_type::storage_policy;
        auto [y1, x1] = storage_policy.inv(e.start);
        auto [y2, x2] = storage_policy.inv(e.end);

        // TODO: Consider neighborhood.

    }

    template<typename InputIt, typename OutputIt>
    double calculate_support(InputIt first, InputIt last, const edge& e, OutputIt it) const
    {
        // FIXME: Calculate support.
        return 0;
    }

    uint32_t max_iterations(double support, double points) const
    {
        static constexpr double p = 0.995;
        double w = support / points;

        return (uint32_t) std::ceil(std::log(1.0 - p) / std::log(1.0 - w * w));
    }
};

#endif //LIBCAL_RANSAC_H
