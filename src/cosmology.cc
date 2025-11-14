#include <gsl/gsl_const_cgsm.h>
#include <gsl/gsl_const_num.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_sf_log.h>

#include <iostream>
#include <vector>

inline double
E (double z, double Om0)
{
  return gsl_sf_exp ((1. / 2.)
                     * gsl_sf_log (Om0 * gsl_pow_3 (z) + (1. - Om0)));
}

void
generate_redshifts (double z_start, double z_end, double grid_size, double h,
                    double Omega_m, std::vector<double> &z)
{
  const double c = GSL_CONST_CGSM_SPEED_OF_LIGHT;
  double DH = c / (100 * GSL_CONST_NUM_KILO * 1e2 * h); // In Mpc

  double k1;
  double k2;
  double k3;
  double k4;

  // Generate redshifts with Runge-Kutta 4th order soln.
  for (double z_dummy = z_start; z_dummy <= z_end;)
    {
      z.push_back (z_dummy);
      k1 = (E (z_dummy, Omega_m) / DH);
      k2 = (E (z_dummy + grid_size * k1 / 2., Omega_m) / DH);
      k3 = (E (z_dummy + grid_size * k2 / 2., Omega_m) / DH);
      k4 = (E (z_dummy + grid_size * k3, Omega_m) / DH);
      z_dummy += (grid_size / 6.) * (k1 + 2. * k2 + 2. * k3 + k4);
    }
} // End of generate_redshifts ()
