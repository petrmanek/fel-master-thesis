# h5tpx
A simple utility to convert HDF5 files back to Timepix ASCII format.


## Dependencies

 * C++17 compiler
 * libcal (see root repository)
 * HDF5 and HDF5 C++ library


## Usage

```shell
$ ./tpx2h5 <h5 file>
```

The utility will print non-zero pixels to standard output. Every line will contain a pixel coordinate and energy value in keV.
Frames will be delimited by a single line containing the "#" character.

Make sure you pipe the output to some file or viewer utility. There will probably be a lot of it.


## Copyright
See README.md in the repository root.

