#include "../lib/density_profile/density_profile.hpp"

int main()
{
    uint Natom = 100;
    uint Nsteps = 30;
    uint Harmonic_lenght = 50;
    double deltar = double(Harmonic_lenght)/Nsteps;

    mcs::DensProfile dens_prof(Natom,Nsteps,deltar);
    std::vector<vector_2D> R(Natom);

    double r = 0;
    for ( uint i = 0; i < Natom; ++i)
    {
        r = deltar/2*i;
        R[i] = vector_2D(r,0);
    }

    for(uint i = 0; i < 10; ++i){
        dens_prof.update(R);
    }

    // dens_prof.print_norm_density_profile();

}