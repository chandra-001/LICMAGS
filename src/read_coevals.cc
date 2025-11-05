#include <cstdint>
#include <fstream>
#include <iostream>
#include <valarray>

double h;
double Omega_m;

double *read_coeval(char *filename, uint64_t &N1, uint64_t &N2, uint64_t &N3,
                    float &grid_size, int32_t &N, std::valarray<float> &v) {
  std::ifstream inFile, in;
  inFile.open(filename);
  inFile >> h >> Omega_m >> N;
  std::string file;

  uint64_t n[3];
  double *z_coeval = new double[N];

  inFile >> z_coeval[0] >> file;

  in.open(file, std::ios_base::binary);
  in.read(reinterpret_cast<char *>(n), sizeof(uint64_t) * 3);
  in.read(reinterpret_cast<char *>(&grid_size), sizeof(float));

  N1 = n[0];
  N2 = n[1];
  N3 = n[2];

  uint64_t size = N1 * N2 * N3;
  v.resize(N * size);

  in.read(reinterpret_cast<char *>(&v[0]), sizeof(float) * size);
  in.close();

  for (int32_t i = 1; i < N; ++i) {
    inFile >> z_coeval[i] >> file;
    in.open(file, std::ios_base::binary);
    in.read(reinterpret_cast<char *>(n), sizeof(uint64_t) * 3);
    in.read(reinterpret_cast<char *>(&grid_size), sizeof(float));
    in.read(reinterpret_cast<char *>(&v[i * size]), sizeof(float) * size);
    in.close();
  }

  inFile.close();

  return z_coeval;
}  // End of read_coeval ()
