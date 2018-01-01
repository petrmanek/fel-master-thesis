# tpx2h5
A simple utility to convert Timepix ASCII files to HDF5 format and them with calibration.


## Dependencies

 * C++17 compiler
 * HDF5 and HDF5 C++ library


## Usage

```shell
$ ./tpx2h5 <input file> <calibration dir> <output file>
```

Input file must contain tuples of coordinate and counter value separated by white space character on individual lines. Frames may be separated by the "#" character on a single line.

Calibration directory must contain files named a.txt, b.txt, c.txt, t.txt. Each file must contain a matrix of 256x256 floating-point values in ASCII, where space is column separator and new line character is row separator.

Output file will be written in the HDF5 format. It will contain a single data floating-point set called _"pixels"_ of type Nx2, where the first column contains pixel coordinates and the second column contains corresponding energy in keV. Frames are delimited by pairs of negative values.


## Copyright
See README.md in the repository root.

