#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <valarray>

double h;
double Omega_m;

double *
read_coeval (std::string filename, uint64_t &N1, uint64_t &N2, uint64_t &N3,
             float &grid_size, int32_t &N, std::valarray<float> &v)
{
  std::ifstream inFile, in;
  std::string fname (filename);

  try
    {
      inFile.open (fname);
      if (!inFile.is_open ())
        {
          throw std::runtime_error ("File " + fname + " cannot be opened!");
        }
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << "Error: " << e.what () << '\n';
      std::exit (EXIT_FAILURE);
    }

  inFile >> h >> Omega_m >> N;
  std::string file;

  uint64_t n[3];
  double *z_coeval = new double[N];

  inFile >> z_coeval[0] >> file;

  try
    {
      in.open (file, std::ios_base::binary);
      if (!in.is_open ())
        {
          throw std::runtime_error ("File " + file + " cannot be opened!");
        }
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << "Error: " << e.what () << '\n';
      std::exit (EXIT_FAILURE);
    }

  in.read (reinterpret_cast<char *> (n), sizeof (uint64_t) * 3);
  in.read (reinterpret_cast<char *> (&grid_size), sizeof (float));

  N1 = n[0];
  N2 = n[1];
  N3 = n[2];

  uint64_t size = N1 * N2 * N3;
  v.resize (N * size);

  in.read (reinterpret_cast<char *> (&v[0]), sizeof (float) * size);
  in.close ();

  for (int32_t i = 1; i < N; ++i)
    {
      inFile >> z_coeval[i] >> file;

      try
        {
          in.open (file, std::ios_base::binary);
          if (!in.is_open ())
            {
              throw std::runtime_error ("File " + file + " cannot be opened!");
            }
        }
      catch (const std::runtime_error &e)
        {
          std::cerr << "Error: " << e.what () << '\n';
          std::exit (EXIT_FAILURE);
        }

      in.read (reinterpret_cast<char *> (n), sizeof (uint64_t) * 3);
      in.read (reinterpret_cast<char *> (&grid_size), sizeof (float));
      in.read (reinterpret_cast<char *> (&v[i * size]), sizeof (float) * size);
      in.close ();
    }

  inFile.close ();

  return z_coeval;
} // End of read_coeval ()
