#include <fstream>
#include <valarray>

void write_lightcone(char *fname, uint64_t &N1, uint64_t &N2, uint64_t &Nz, float &grid_size, std::valarray<float> &Light_Cone) {
  std::ofstream out{fname, std::ios_base::binary};
  out.write(reinterpret_cast<char *>(&N1), sizeof(uint64_t));
  out.write(reinterpret_cast<char *>(&N2), sizeof(uint64_t));
  out.write(reinterpret_cast<char *>(&Nz), sizeof(uint64_t));
  out.write(reinterpret_cast<char *>(&grid_size), sizeof(uint64_t));
  out.write(reinterpret_cast<char *>(&Light_Cone[0]),
            sizeof(float) * N1 * N2 * Nz);
  out.close();
}
