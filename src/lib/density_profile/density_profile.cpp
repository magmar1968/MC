#include "density_profile.hpp"


namespace mcs{

/// @brief Density profile constructor
/// @param Natoms Atoms number
/// @param DeltaR Spatial step
DensProfile::DensProfile(cuint Natoms, cdouble DeltaR)
    :DensProfile(Natoms,this->N0Steps,DeltaR)
{
}

/// @brief Density profile constructor
/// @param Natoms Atoms number
/// @param Nsteps Number of initial step for the density progile
/// @param DeltaR spatial step size
DensProfile::DensProfile(cuint Natoms, cuint N0steps, cdouble DeltaR)
    :_Natoms(Natoms),_Nsteps(N0steps),_DeltaR(DeltaR)
{
    _Nupdated = 0;
    _density_profile.resize(_Nsteps);
    std::fill(_density_profile.begin(),_density_profile.end(),0);
    _MaxR = _DeltaR * _Nsteps;
}



//update density profile from a gas configuration
void DensProfile::update(const std::vector<vector_2D>& R)
{
    double dist;
    uint Nenlarging, new_size, i_step;
    for(auto pos : R)
    {
        dist = mod(pos);
        
        if( dist >= _MaxR){
            Nenlarging = floor( (dist - _MaxR)/_DeltaR) + 1;
            new_size   = _Nsteps + Nenlarging;
            //resize density profile size
            _density_profile.resize(new_size,0.);
            //update parameters to accostum to new size 
            _Nsteps = new_size;
            _MaxR   = new_size * _DeltaR; 
        }
        // //update density profile
        i_step = floor(dist/_DeltaR);
        _density_profile[i_step] += 1;
    }
    _Nupdated += 1;
}

void DensProfile::update(std::vector<vector_2D>::const_iterator R_begin,
                             std::vector<vector_2D>::const_iterator R_end)
{
    double dist;
    uint Nenlarging, new_size, i_step;
    for(auto pos = R_begin; pos != R_end; ++pos)
    {
        dist = mod(*pos);
        
        if( dist >= _MaxR){
            Nenlarging = floor( (dist - _MaxR)/_DeltaR) + 1;
            new_size   = _Nsteps + Nenlarging;
            //resize density profile size
            _density_profile.resize(new_size,0.);
            //update parameters to accostum to new size 
            _Nsteps = new_size;
            _MaxR   = new_size * _DeltaR; 
        }
        // //update density profile
        i_step = floor(dist/_DeltaR);
        _density_profile[i_step] += 1;
    }
    _Nupdated += 1;
}





void DensProfile::print_norm_density_profile(const std::string& filename)
{
    std::fstream  ofs(filename,std::fstream::out);
    double rmin,rmax;

    if(ofs.bad()){
        std::cerr << "ERROR: unable to print density profile" << std::endl;
        return;
    }

    normalize();

    //else
    ofs << "rmin,rmax,dens" << std::endl;
    for(uint i_step = 0; i_step < _Nsteps; ++i_step)
    {
        rmin = i_step*_DeltaR;
        rmax = (i_step+1)*_DeltaR; 
        ofs << std::setprecision(15) 
            <<  rmin<<"," 
            << rmax <<","
            << _norm_density_profile[i_step] << std::endl;

    }
    
}


void DensProfile::normalize()
{
#ifdef DEBUG
    uint sum_vec = 0;
    for(auto el : _density_profile){
        sum_vec += el;
    }
    assert(sum_vec%_Natoms == 0);
    assert(sum_vec%_Nupdated == 0);
    assert(sum_vec == (_Natoms*_Nupdated));
#endif


    //init _norm_density_profile
    _norm_density_profile.resize(_Nsteps);
    std::fill(_norm_density_profile.begin(),
              _norm_density_profile.end(),0.);

    double rmin,rmax,area;
    double val;
    for(uint i = 0; i < _Nsteps; ++i)
    {
        val = _density_profile[i];
        val /= double(_Nupdated);  
        rmin = i*_DeltaR;
        rmax = (i+1)*_DeltaR;
        area = M_PI * (rmax*rmax - rmin*rmin);
        //normalize by the are
        _norm_density_profile[i] = val / area; 
    }

    
}



}