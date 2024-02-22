#ifndef __DensProfile_HPP__
#define __DensProfile_HPP__
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "../utility/2D_vector.hpp"
#include <iomanip>

#ifdef DEBUG
#include <assert.h>
#endif


namespace mcs{


class DensProfile
{
    using uint = unsigned int; 
    using cuint = const uint;
    using cdouble = const double;
private:
    static const int N0Steps = 10;
public:
    DensProfile(){};
    DensProfile(cuint,cdouble);
    DensProfile(cuint,cuint,cdouble);
    ~DensProfile(){};


    //methods
    void update(const std::vector<vector_2D>& );
    void update(std::vector<vector_2D>::const_iterator, 
                std::vector<vector_2D>::const_iterator);
    void print_norm_density_profile(const std::string& filename = "./data/dens_profile.csv");

private:
    void normalize();

private:
    uint   _Natoms;    //Number of atoms in the gas
    uint   _Nupdated;  //Number of time that the dens prof, had been updated
    uint   _Nsteps;    //Number of dens profile steps
    double _MaxR;      //Max radius of the dens profile
    double _DeltaR;    //size of a dens profile step

    std::vector<uint> _density_profile;  //the effective density profile
    std::vector<double> _norm_density_profile; //the normalized density profile
};

}

#endif 