#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>
#include <omp.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

extern double h;
extern double Omega_m;

void generate_redshifts (double, double, double, double, double,
                         std::vector<double> &);

uint64_t
interpolate (int64_t N1, int64_t N2, int64_t N3, double grid_size,
             double *z_coeval, int32_t N, const std::vector<float> &v,
             std::vector<float> &Light_Cone)
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

  const float *__restrict v_p = v.data ();
  const double *__restrict z_p = z.data ();
  float *__restrict Light_Cone_p = Light_Cone.data ();

  int32_t Nthreads = 4;

#pragma omp parallel for collapse(2) num_threads(Nthreads)
  for (int64_t i = 0; i < N1; ++i)
    for (int64_t j = 0; j < N2; ++j)
      {
        gsl_interp_accel *acc = gsl_interp_accel_alloc ();
        gsl_spline *spline_steffen = gsl_spline_alloc (gsl_interp_steffen, N);

        std::vector<double> vv (N);
        double *__restrict vv_p = vv.data ();

        for (int64_t p = 0; p < z.size (); ++p)
          {
            index = (p % N3) + N3 * (j + N2 * i);

            for (int64_t ii = 0; ii < N; ++ii)
              vv_p[ii] = static_cast<double> (v_p[index + stride * ii]);

            gsl_spline_init (spline_steffen, z_coeval, &vv[0], N);

            Light_Cone_p[p + z.size () * (j + N2 * i)]
                = gsl_spline_eval (spline_steffen, z_p[p], acc);
          }

        gsl_spline_free (spline_steffen);
        gsl_interp_accel_free (acc);
      }

  return static_cast<uint64_t> (z.size ());
} // End of interpolate ()
