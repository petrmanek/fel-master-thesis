#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <cmath>
#include <H5Cpp.h>

std::tuple<hsize_t, hsize_t>
read_length(const std::string& file)
{
    std::ifstream input{file};
    hsize_t frames = 1; // +1 to account for the last frame
    hsize_t pixels = 0;
    bool prev_empty = false;

    for (std::string line{}; std::getline(input, line); ) {
        if (line.find('#') != std::string::npos) {
            if (!prev_empty)
                ++frames;
            prev_empty = true;
        } else {
            ++pixels;
            prev_empty = false;
        }
    }

    return std::make_tuple(frames, pixels);
}

struct calibration {
    double a, b, c, t;
};

static constexpr int TPX_DIM = 256;

std::vector<calibration>
read_calibration(const std::string& path) {
    std::ifstream a{path + "/a.txt"};
    std::ifstream b{path + "/b.txt"};
    std::ifstream c{path + "/c.txt"};
    std::ifstream t{path + "/t.txt"};

    std::vector<calibration> calib{};
    calibration item{};

    for (int i = 0; i < TPX_DIM; ++i) {
        for (int j = 0; j < TPX_DIM; ++j) {
            a >> item.a;
            b >> item.b;
            c >> item.c;
            t >> item.t;
            calib.emplace_back(item);
        }
    }

    return calib;
}

double
counter_to_energy(const std::vector<calibration>& calib, uint16_t coord, uint16_t counter)
{
    const calibration& k = calib[coord];

    if (k.a == 0 || k.c > 100000 || k.t > 100000) {
        return -1;
    }

    // If clock frequency for calibration differs from final one -> rescale = final/cali
    double TOT1 = counter; // * rescale_;
    double energy = (1 / (2 * k.a) * (TOT1 + k.a * k.t - k.b + std::sqrt(std::pow((k.b + k.a * k.t - TOT1), 2) + 4 * k.a * k.c)));

    // If energy > 918 = distortion level reached - slightly depends on values used energy response completely breaks down above ~1800 keV.
    if (energy > 918) {
        energy = energy - 0.888 * (energy - 918);
    }

    return energy;
}

void
convert_data(const std::string& in_path, const std::vector<calibration>& calib, hsize_t records, const std::string& out_path)
{
    H5::H5File file{out_path, H5F_ACC_TRUNC};
    hsize_t dimsf[] = {records, 2};

    /*
     * The data is stored like this:
     * coord1 energy1
     * coord2 energy2
     * ...
     * coordN energyN
     */
    H5::DataSpace dataspace{2, dimsf};
    H5::DataSpace filespace{2, dimsf};
    H5::FloatType datatype{H5::PredType::NATIVE_DOUBLE};
    H5::DataSet dataset = file.createDataSet("pixels", datatype, dataspace);

    // Allocate buffer of size BUFFER_SIZE x 2
    static constexpr hsize_t BUFFER_SIZE = 4096;
    double *buffer = new double[BUFFER_SIZE * 2];
    hsize_t dimsm[] = {BUFFER_SIZE, 2};
    H5::DataSpace memspace{2, dimsm};

    bool prev_empty = false;
    hsize_t fill_idx = 0; // Valid range of buffer.
    hsize_t write_idx = 0; // Total no. of records written.

    // Read file line by line.
    std::ifstream input{in_path};
    uint16_t coord, counter;
    for (std::string line{}; std::getline(input, line); ) {
        if (line.find('#') != std::string::npos) {
            if (!prev_empty) {
                // Swallow consecutive empty frames.
                buffer[fill_idx * 2 + 0] = -1.0;
                buffer[fill_idx * 2 + 1] = -1.0;
                ++fill_idx;
            }

            prev_empty = true;
        } else {
            // Read data from ASCII.
            std::stringstream ss{line};
            ss >> coord >> counter;

            // Write data to binary buffer.
            buffer[fill_idx * 2 + 0] = coord;
            buffer[fill_idx * 2 + 1] = counter_to_energy(calib, coord, counter);
            ++fill_idx;

            prev_empty = false;
        }

        if (fill_idx == BUFFER_SIZE) {
            // Buffer full, flush it out.
            std::cerr << "write(offset=" << write_idx << ", size=" << BUFFER_SIZE << ")" << std::endl;

            hsize_t count[] = {BUFFER_SIZE, 2};
            hsize_t start[] = {write_idx, 0};
            filespace.selectHyperslab(H5S_SELECT_SET, count, start);
            dataset.write(buffer, H5::PredType::NATIVE_DOUBLE, memspace, filespace);

            // Move in the file, reset the buffer.
            write_idx += BUFFER_SIZE;
            fill_idx = 0;
        }
    }

    if (fill_idx > 0) {
        // Perform one last write.
        std::cerr << "write(offset=" << write_idx << ", size=" << fill_idx << ")" << std::endl;

        // Use only the valid portion of buffer.
        hsize_t count[] = {fill_idx, 2};
        hsize_t start[] = {write_idx, 0};
        filespace.selectHyperslab(H5S_SELECT_SET, count, start);

        hsize_t start2[] = {0, 0};
        memspace.selectHyperslab(H5S_SELECT_SET, count, start2);

        dataset.write(buffer, H5::PredType::NATIVE_DOUBLE, memspace, filespace);

        // No moving here, doesn't matter.
    }

    delete[] buffer;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " <in> <calib> <out>" << std::endl;
        return 8;
    }

    const std::string in_path{argv[1]};
    const std::string calibration_path{argv[2]};
    const std::string out_path{argv[3]};

    std::cerr << "reading " << in_path << std::endl;
    const auto [frames, pixels] = read_length(in_path);

    std::cerr << "have " << frames << " frames and " << pixels << " pixels" << std::endl;

    std::cerr << "reading " << calibration_path << std::endl;
    std::vector<calibration> calib = read_calibration(calibration_path);

    convert_data(in_path, calib, frames + pixels, out_path);
    return 0;
}
