#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>

#include <omp.h>

double *read_coeval(char *, uint64_t &, uint64_t &, uint64_t &, float &,
                    int32_t &, std::valarray<float> &);
uint64_t interpolate(int64_t, int64_t, int64_t, double, double *, int32_t,
                 std::valarray<float> &, std::valarray<float> &);
void write_lightcone(char *, uint64_t &, uint64_t &, uint64_t &, float &, std::valarray<float>&);

int main(int argc, char *argv[]) {
  uint64_t N1;
  uint64_t N2;
  uint64_t N3;
  int32_t N;
  float grid_size;

  std::valarray<float> v;
  std::valarray<float> Light_Cone;

  char fname[2048];

  strcpy(fname, argv[1]);
  std::cout << "Reading coeval cubes...\n";
  double *z_coeval = read_coeval(fname, N1, N2, N3, grid_size, N, v);
  std::cout << "Done\n\n";

  std::cout << "Interpolating...\n";
  double T = omp_get_wtime();
  uint64_t Nz = interpolate(N1, N2, N3, grid_size, z_coeval, N, v, Light_Cone);
  T = omp_get_wtime() - T;
  std::cout << "Done\n\n";

  std::cout << "Time elapsed for interpolation: " << T << " sec\n\n";

  strcpy(fname, argv[2]);

  std::cout << "Writing lightcone data...\n";
  write_lightcone(fname, N1, N2, Nz, grid_size, Light_Cone);
  std::cout << "Done\n";

  delete[] z_coeval;
}
