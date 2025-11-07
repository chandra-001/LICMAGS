#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>

#include <omp.h>

extern double h;
extern double Omega_m;

void generate_redshifts (double, double, double, double, double,
                         std::vector<double> &);

uint64_t
interpolate (int64_t N1, int64_t N2, int64_t N3, double grid_size,
             double *z_coeval, int32_t N, std::valarray<float> &v,
             std::valarray<float> &Light_Cone)
{
  std::vector<double> z;
  generate_redshifts (z_coeval[0], z_coeval[N - 1], grid_size, h, Omega_m, z);

  std::ofstream outFile{ "redshifts.txt" };
  for (auto redshift : z)
    outFile << redshift << "\n";

  outFile.close ();

  int64_t index;
  int64_t stride = N1 * N2 * N3;

  Light_Cone.resize (N1 * N2 * z.size ());

#pragma omp parallel for collapse(2)
  for (int64_t i = 0; i < N1; ++i)
    for (int64_t j = 0; j < N2; ++j)
      {

        gsl_interp_accel *acc = gsl_interp_accel_alloc ();
        gsl_spline *spline_steffen = gsl_spline_alloc (gsl_interp_steffen, N);
        std::valarray<double> vv (N);

        for (int64_t p = 0; p < z.size (); ++p)
          {
            index = (p % N3) + N3 * (j + N2 * i);

            for (int64_t ii = 0; ii < N; ++ii)
              vv[ii] = static_cast<double> (v[index + stride * ii]);

            gsl_spline_init (spline_steffen, z_coeval, &vv[0], N);

            Light_Cone[p + z.size () * (j + N2 * i)]
                = gsl_spline_eval (spline_steffen, z[p], acc);
          }

        gsl_spline_free (spline_steffen);
        gsl_interp_accel_free (acc);
      }

  return static_cast<uint64_t> (z.size ());
} // End of interpolate ()
