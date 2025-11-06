# LICMAGS
The **LI**ght-**C**one **MA**p **G**enerator for cosmological **S**imulations (**LICMAGS**) is a tool to produce lightcone maps given a set of coeval simulation snapshots from cosmological simulation runs. The input snapshots can be any gridded map such as dark-matter density field, collapsed halo field or any line-intensity maps. The algorithm is based on Steffen's interpolation ([Steffen 1990, A&A, 239, 443](https://ui.adsabs.harvard.edu/abs/1990A%26A...239..443S/abstract)) and the lightcone slices are generated using steps from [Datta et al. 2012, MNRAS, 424, 1877](https://ui.adsabs.harvard.edu/abs/2012MNRAS.424.1877D/abstract). Unlike linear interpolation, which can produce sharp corners and discontinuous derivatives at the data points, the Steffen's interpolation guarantees smooth interpolation (continuous upto first order derivative) and preserves monotonicity (unlike cubic splines, which can produce spurious oscillations). Therefore, this method is well suited for applications in realistic physical problems, where smoothness and monotonicity is desired. This code has one pre-requisite package which is the **G**NU **S**cientific **L**ibrary (**GSL**, [https://www.gnu.org/software/gsl/](https://www.gnu.org/software/gsl/)), from which the Steffen's interpolation algorithm has been borrowed. The code also assumes a Flat LCDM cosmology to calculate the redshift list for generating lightcone slices. Instructions on how to use this tool is briefly described below.
## How to run
To use this tool the coeval snapshots need to have the following format such that it can be read with the following python script:
```
import numpy as np
f = open('path/to/coeval/snapshot/file', 'rb')
N1,N2,N3 = np.fromfile(f, count=3, dtype=np.uint64) # Dimensions of the coeval box
LL = np.fromfile(f, count=1, dtype=np.float32) # grid size of the coeval box (in Mpc)
coeval_data = np.reshape(np.fromfile(f, count=N1*N2*N3, dtype=np.float32), (N1,N2,N3),\
                         order='C')
f.close()
```

When the coeval snapshots are available, this code can be compiled with the Makefile that is given in the repository. Executing the command `make`, will generate the executable 'lightcone'. This executable can be run with the following syntax:`./lightcone input.txt output.bin`. The 'input.txt' is the parameter file (a self-explanatory version of 'input.txt' is given in the repository and can be edited accordingly for use before the code is run). A typical parameter file can look like this:
```
0.67
0.31
5
6.83 path/to/Map_z6.83.bin
7.21 path/to/Map_z7.21.bin
7.66 path/to/Map_z7.66.bin
8.09 path/to/Map_z8.09.bin
8.52 path/to/Map_z8.52.bin
```
The first two numbers are the Hubble parameter ($h$) and matter density parameter ($\Omega_{\rm m}$) and the third number is the number of coeval snapshots to be read by the code. The rest of the input will have two columns, where the first column is the list of coeval redshifts and the second column is the list of paths to the coeval snaphots. The redshifts should be arranged in ascending order, and the coeval files should be matched accordingly. The 'output.bin' is the file name in which the lightcone data is dumped. On successful completion of the run, the outputs will be 'output.bin' (or the name of the file supplied during the run) and 'redshift.txt', in which the list of redshifts of the lightcone are there. The lightcone file can be read with the following python script:
```
import numpy as np
import matplotlib.pyplot as plt

f = open('path/to/lightcone/file/output.bin', 'rb')
N1,N2,Nz = np.fromfile(f, count=3, dtype=np.uint64) # Dimensions of the lightcone box
LL = np.fromfile(f, count=1, dtype=np.float32) # grid size of the lightcone box (in Mpc)
lightcone_data = np.reshape(np.fromfile(f, count=N1*N2*Nz, dtype=np.float32), (N1,N2,Nz),\
                            order='C')
f.close()
# Nz is number of slices along the redshift axis

im = plt.imshow(lightcone_data[N,:,:], ...) # Show the N-th slice of the lightcone in the
                                            # y-z plane
```

## Acknowledgements
If you are using this tool, please acknowledge the following papers:
[https://ui.adsabs.harvard.edu/abs/1990A%26A...239..443S/abstract](https://ui.adsabs.harvard.edu/abs/1990A%26A...239..443S/abstract)
[https://ui.adsabs.harvard.edu/abs/2012MNRAS.424.1877D/abstract](https://ui.adsabs.harvard.edu/abs/2012MNRAS.424.1877D/abstract)
[https://ui.adsabs.harvard.edu/abs/2021MNRAS.507.2500M/abstract](https://ui.adsabs.harvard.edu/abs/2021MNRAS.507.2500M/abstract)
