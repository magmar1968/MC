#include "puregas_HS.hpp"

namespace mcs
{
    

//###############################################
//#               Constructors                  #
//###############################################


PureGas_HS::PureGas_HS(gas_parameters& param)
    : PureGas(param.Natoms,param.harmonic_lenght,param.D)
{
    _k0    = param.k0;
    _alpha = param.alpha;
    _1overalpha = 1./_alpha;
    _Rv    = param.Rv;
    _Rcore = param.Rcore;
    _C     = - j0(_k0*_Rcore)/y0(_k0*_Rcore);
    _Gamma = - (_alpha*_k0)/ exp( -_Rv/_alpha) * 
                ( j1(_k0*_Rv) + _C * y1(_k0*_Rv) );
}

//###############################################
//#       Generate Initial Configuration        #
//###############################################

PureGas::pos_vec_type PureGas_HS::gen_initial_conf()
{
    pos_vec_type R_OUT(_Natoms);
    bool regen = true;
    while(regen == true)
    {
        for(auto& pos : R_OUT){
            pos.x = _gauss(_RNG)*_h_len;
            pos.y = _gauss(_RNG)*_h_len;
        }
        regen = CheckHsCrossing(R_OUT);
    }
    return R_OUT;
}

//###############################################
//#             Trial Wave Function             #
//###############################################

/// @brief compute the  exp trial Wave function starting from the configuration @p R
/// @param R configuration 
/// @return trial Wave function value
double PureGas_HS::trial_WF(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif


    double r,x;
    double u = 0.;

    //interaction inbetween particles
    for(uint i_atom = 0; i_atom < R.size() -1; ++i_atom){
        for(uint j_atom = i_atom + 1; j_atom < R.size(); ++j_atom ){
            r = mod(R[i_atom] - R[j_atom]);
            u += log(twobody_scatt_sol(r)); 
        }
    }

    //interaction with the harmonic potential
    for(uint k_atom = 0; k_atom < R.size(); ++ k_atom){
        r = mod(R[k_atom]);
        x = r*_1over_h_len;
        u += - x*x/2.; 
    }
    return u;
}


//###############################################
//#                  Energies                   #
//###############################################

/// @brief return the Local Energy of the configuration @p R
/// @param R atoms configuration
/// @return local energy
double PureGas_HS::Elocal(cpos_vec_type & R) const
{
    return Ekin(R) + Epot(R);
}

//================ POTENTIAL ENERGY =====================//

/// @brief Compute and return the Potential Energy of the
///        configuration @p R
/// @param R 
/// @return potential energy
double PureGas_HS::Epot(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    double epot = 0.,r;
    double K = _Rcore*_Rcore*_1over_h_len*_1over_h_len*_1over_h_len*_1over_h_len;

    //harmonic potential
    for(uint k_atom = 0; k_atom < R.size(); ++k_atom){
        r = mod(R[k_atom]);
        epot += K*r*r;
    }
    return epot;
}

//=============== KINETIC ENERGY ================//


/// @brief Compute and return the kinetic energy of the
///        configuration @p R
/// @param R 
/// @return kinetic energy
double PureGas_HS::Ekin(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif

    return EkinPartial(R) - EkinFor(R);
}

//============== EkinPartial ==================//

/// @brief Compute the Kinetic Energy whiout the EkinFor
///        contribute for configuration @p R. Usefull in 
///        case EkinFor must be redefined.
/// @param R Configuration
/// @return Ekin Partial
double PureGas_HS::EkinPartial(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif  
    double r,up,us;
    double r1m;
    double ekinpartial = 0;
    for(uint i_atom = 0; i_atom < R.size() -1; ++i_atom){
        for(uint j_atom = i_atom + 1; j_atom < R.size(); ++j_atom ){
            r  = mod(R[i_atom] - R[j_atom]);
            r1m = 1./r;
            up = twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);
            us = twobody_scatt_sol_dprime(r)/twobody_scatt_sol(r);

            // factor 2 take in account that we are iterating only 
            // over half of the times
            ekinpartial += -_D*2*( us + up*r1m - up*up);
        }
    }

    //add kinetic term due to the interaction whit the harmonic potential
    ekinpartial += -_D*(-2*_1over_h_len*_1over_h_len)*R.size();

    return ekinpartial;
}

//================ EKinFor ====================//

/// @brief Compute and return the Ekinf of the configuration
///        @p R.
/// @param R 
/// @return EkinFor
double PureGas_HS::EkinFor(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    double ekinfor = 0.;
    std::vector<vector_2D> drift(R.size());
    drift = F(R);

    for(uint k_atom = 0; k_atom < R.size(); ++k_atom){
        ekinfor += _D*0.25*(drift[k_atom] * drift[k_atom]);
    }
    return ekinfor; 
}


//###############################################
//#             Drift Force                     #
//###############################################

/// @brief Compute and return a std::vector of resulting
///        force over each atom from configuration @p R 
/// @param R 
/// @return resulting force
PureGas::force_vec_type PureGas_HS::F(cpos_vec_type& R) const 
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif

    std::vector<vector_2D> Force(R.size());
    vector_2D r_hat;
    double r, twiceup;
    //force inbetween particles
    for(uint i_atom = 0; i_atom < R.size() -1;  ++i_atom ){
        for(uint j_atom = i_atom +1; j_atom < R.size(); ++ j_atom){
            r = mod(R[i_atom] - R[j_atom] );
            //normalized versor
            r_hat = (R[i_atom] - R[j_atom])/ r;
            twiceup = 2*twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);

            Force[i_atom] += twiceup*r_hat;
            Force[j_atom] -= twiceup*r_hat;
        } 
    }

    //force due to the harmonic potential
    for(uint k_atom = 0; k_atom < R.size(); ++ k_atom){
        r = mod(R[k_atom]);
        //normalize versor
        r_hat = R[k_atom]/r;
        twiceup = -2*r*_1over_h_len*_1over_h_len;
        Force[k_atom] += twiceup*r_hat;
    }

    //factors 2 come from the force definition itself
    return Force;
}


//###############################################
//#               Diffuse Gas                   #
//###############################################
PureGas::pos_vec_type 
PureGas_HS::diffuse(cpos_vec_type& R, double sigma)
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    pos_vec_type R_OUT(_Natoms);
    for(uint i_pos = 0;i_pos < R.size(); ++i_pos ){
        R_OUT[i_pos].x = R[i_pos].x + _gauss(_RNG)*sigma;
        R_OUT[i_pos].y = R[i_pos].y + _gauss(_RNG)*sigma;
    }
    return R_OUT;
}

//###############################################
//#          Check Hard Sphere Crossings        #
//###############################################

/// @brief check if one or more particles are closer than Rcore
/// @param R array of particle's positions 
/// @return true if there's a crossing false if not
bool PureGas_HS::CheckHsCrossing(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    for(size_t i_atom = 0; i_atom < R.size() - 1; ++ i_atom){
        for(size_t j_atom = i_atom + 1; j_atom < R.size(); ++ j_atom){
            if(abs(R[i_atom].x - R[j_atom].x) > _Rcore){
                continue;
            }else{
                if(abs(R[i_atom].y - R[j_atom].y) > _Rcore){
                    continue;
                }else{
                    if( mod( R[i_atom] - R[j_atom]) <= _Rcore)
                        // crossing encountered, return true
                        return true;
                }
            }
        }
    }
    //if there's no crossing return false
    return false;
}

bool PureGas_HS::is_configuration_physical(cpos_vec_type& R) const
{
    return !CheckHsCrossing(R);
}

//###############################################
//#            Printing Methods                 #
//###############################################

void PureGas_HS::print_TWF_tofile(const std::string& filename) const
{
    std::ofstream ofs(filename,std::fstream::out);
    ofs << "step,r,corr,corrprime,corrdoubleprime\n";
    const uint NPartititions = 5000; 
    double deltar = 10.*_Rv/double(NPartititions);
    double r;

    for(uint step = 0; step < NPartititions; ++step){
        r = deltar * step;
        if(r < _Rcore){
            ofs << step << "," << r <<"," 
                << std::setprecision(15)
                << 0.0 <<"," << 0.0 << "," << 0.0  << std::endl;
                
        }else{
            ofs << step << "," << r << "," 
                << std::setprecision (15)
                << twobody_scatt_sol(r) << "," 
                << twobody_scatt_sol_prime(r) << ","
                << twobody_scatt_sol_dprime(r) << std::endl;
        }
    }   
   
}

//###############################################
//#             Printing Methods                #
//###############################################

void PureGas_HS::print_conf_tofile(const std::string& filename,
                                    cpos_vec_type& R) const
{
    std::ofstream ofs(filename,std::fstream::out);
    ofs << "atom,x,y\n";

    for(uint i_atom = 0; i_atom< R.size(); ++i_atom){
        ofs << i_atom      << "," << std::setprecision(15)
            << R[i_atom].x << ","
            << R[i_atom].y << std::endl;
    }
}




//###############################################
//#    two body scattering problem solutions    #
//###############################################

double PureGas_HS::twobody_scatt_sol(double r) const
{
    if(r > _Rv)
#ifndef FASTEXP
        return 1 - _Gamma*exp( -r* _1overalpha);
#else 
        return 1 - _Gamma*fastExp( -r* _1overalpha);
#endif
    else if (r <= _Rv and r > _Rcore)
        return j0(_k0*r) + _C * y0(_k0*r);
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r << " < Rcore \n";
    }
}

double PureGas_HS::twobody_scatt_sol_prime(double r) const
{
   if(r > _Rv)
#ifndef FASTEXP
        return _Gamma*exp( -r*_1overalpha)*_1overalpha;
#else
        return _Gamma*fastExp( -r*_1overalpha)*_1overalpha;
#endif
    else if (r <= _Rv and r > _Rcore)
        return - _k0*( j1( _k0*r) + _C* y1(_k0*r) ); 
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r  << " < Rcore \n";
    } 
}

double PureGas_HS::twobody_scatt_sol_dprime(double r) const 
{
    if(r > _Rv)
#ifndef FASTEXP
        return - _Gamma*exp(-r*_1overalpha)*_1overalpha*_1overalpha ;
#else    
        return - _Gamma*fastExp(-r*_1overalpha)*_1overalpha*_1overalpha ;
#endif
    else if (r <= _Rv and r > _Rcore)
        return -_k0*_k0/2. * 
               (
                    (j0(_k0*r) - jn(2,_k0*r)) + _C*(y0(_k0*r) - yn(2,_k0*r))
               );
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r  <<" < Rcore \n";
    }
}


} // namespace mcs