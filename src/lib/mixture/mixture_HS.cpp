#include "mixture_HS.hpp"

namespace mcs
{
    


//###############################################
//#               Constructors                  #
//###############################################

Mixture_HS::Mixture_HS(PureGas_HS * Gas1, PureGas_HS * Gas2,gas_inter_params& param)
    :_Gas1(Gas1),_Gas2(Gas2)
{
    _Natoms_Gas1 = param.Natoms_Gas1;
    _Natoms_Gas2 = param.Natoms_Gas2;

    _k0_int    = param.k0;
    _alpha_int = param.alpha;
    _Rv_int    = param.Rv;
    _Rcore_int = param.Rcore;
    
    _C_int     = - j0(_k0_int*_Rcore_int)/y0(_k0_int*_Rcore_int);
    _Gamma_int = - (_alpha_int*_k0_int)/ exp( -_Rv_int/_alpha_int) * 
                   ( j1(_k0_int*_Rv_int) + _C_int * y1(_k0_int*_Rv_int) );
}


//###############################################
//#             Trial Wave Function             #
//###############################################

/// @brief compute and return the  exp trial Wave function starting from the 
///        configuration @p R1 and @p R2
/// @param R1 configuration Gas 1
/// @param R2 configuration Gas 2
/// @return trial Wave function value
double Mixture_HS::trial_WF(cpos_vec_type& R1, cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif

    double u = 0.;
    double r = 0.;
    u += _Gas1->trial_WF(R1); //gas1 part
    u += _Gas2->trial_WF(R2); //gas2 part

    //add interaction inbetween gasses part
    for(auto pos1: R1){
        for(auto pos2 : R2){
            r = mod(pos2 - pos1);
            u += log(twobody_scatt_sol(r));
        }
    } 
    return u;
}


//###############################################
//#                  Energies                   #
//###############################################

/// @brief Compute and return Local energy of the configurations
///        @p R1 and @p R2
/// @param R1 configuration for the first gas
/// @param R2 configuration for the second gas
/// @return Elocal
double Mixture_HS::Elocal(cpos_vec_type & R1,cpos_vec_type & R2) const 
{
#ifdef DEBUG    
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif

    return Epot(R1,R2) + Ekin(R1,R2);
}

//================ POTENTIAL ENERGY =====================//

/// @brief Compute and return the potential energy of the 
///        configurations @p R1 and   @p R2
/// @param R1 configuration for the first gas
/// @param R2 configuration for the second gas
/// @return potential Energy
double Mixture_HS::Epot(cpos_vec_type& R1, cpos_vec_type& R2) const 
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif
    return _Gas1->Epot(R1) + _Gas2->Epot(R2);
}

//=============== KINETIC ENERGY ================//

/// @brief Compute and return the Kinetic Energy of the Mixture_HS
///        using as configuration @p R1 and @p R2
/// @param R1 configuration gas1 
/// @param R2 configuration gas2
/// @return Kinetic Energy
double Mixture_HS::Ekin(cpos_vec_type& R1, cpos_vec_type& R2) const 
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif 
    
    return EkinPartial(R1,R2) - EkinFor(R1,R2);
}


//================Ekin Partial=================//
double Mixture_HS::EkinPartial(cpos_vec_type& R1, cpos_vec_type& R2) const
{
    double ekinpart = 0;
    double r    = 0.;
    double r1m,up,us;
    double D1 = _Gas1->Get_D();
    double D2 = _Gas2->Get_D();

    //add EkinPartial
    ekinpart += _Gas1->EkinPartial(R1);
    ekinpart += _Gas2->EkinPartial(R2);

    //interaction between gasses
    for(uint i_atom = 0; i_atom < R1.size(); ++i_atom){
        for(uint j_atom = 0;j_atom < R2.size(); ++j_atom){
            r  = mod(R1[i_atom] - R2[j_atom]);
            r1m = 1./r;
            up = twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);
            us = twobody_scatt_sol_dprime(r)/twobody_scatt_sol(r);

            //the interaction is symmetric except for D1,D2
            ekinpart += -D1*( us + up*r1m - up*up);
            ekinpart += -D2*( us + up*r1m - up*up);
        }
    }
    return ekinpart;
}


//================ EKinFor ====================//

double Mixture_HS::EkinFor(cpos_vec_type& R1, cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif 
    double ekinfor = 0.;

    double D1 = _Gas1->Get_D();
    double D2 = _Gas2->Get_D();

    std::vector<vector_2D> drift1 = F1(R1,R2);
    std::vector<vector_2D> drift2 = F2(R1,R2);

    for(uint k_atom = 0; k_atom < R1.size(); ++k_atom){
        ekinfor += D1*0.25*(drift1[k_atom] * drift1[k_atom]);
    }
    for(uint k_atom = 0; k_atom < R2.size(); ++k_atom){
        ekinfor += D2*0.25*(drift2[k_atom] * drift2[k_atom]);
    }
    return ekinfor;
}


//###############################################
//#             Drift Force                     #
//###############################################

/// @brief Return the total force over the particles of gas 1
/// @param R1 configuration Gas1 
/// @param R2 configuration Gas2 
/// @return total force
Mixture_HS::force_vec_type Mixture_HS::F1(cpos_vec_type&R1,cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif 
   std::vector<vector_2D> Force = _Gas1->F(R1);
    vector_2D r_hat;
    double r, twiceup;

    //interaction force
    for(uint i_atom = 0; i_atom < R1.size();  ++i_atom ){
        for(uint j_atom = 0; j_atom < R2.size(); ++j_atom){
            r = mod(R1[i_atom] - R2[j_atom]);
            //normalized versor
            r_hat = (R1[i_atom] - R2[j_atom])/r;
            twiceup = 2*twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);
            Force[i_atom] += twiceup*r_hat; //factor 2 becayse F = 2 * \nabla psi / psi
        }
    }
    return Force; 
}

Mixture_HS::force_vec_type Mixture_HS::F2(cpos_vec_type& R1, cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif  
    std::vector<vector_2D> Force = _Gas2->F(R2);
    vector_2D r_hat;
    double r, twiceup;

    //interaction force
    for(uint i_atom = 0; i_atom < R2.size();  ++i_atom ){
        for(uint j_atom = 0; j_atom < R1.size(); ++j_atom){
            r = mod(R2[i_atom] - R1[j_atom]);
            //normalized versor
            r_hat = (R2[i_atom] - R1[j_atom])/r;
            twiceup = 2*twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);
            Force[i_atom] += twiceup*r_hat;
        }
    }
    return Force;
}


//###############################################
//#               Diffuse Gasses                #
//###############################################

Mixture_HS::pos_vec_type Mixture_HS::diffuse_gas1(cpos_vec_type& R1, double sigma)
{
    return _Gas1->diffuse(R1,sigma);
}

Mixture_HS::pos_vec_type Mixture_HS::diffuse_gas2(cpos_vec_type& R2, double sigma)
{
    return _Gas2->diffuse(R2,sigma);
}

//###############################################
//#          Check Hard Sphere Crossings        #
//###############################################

bool Mixture_HS::CheckHsCrossing(cpos_vec_type& R1, cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif  
    return _Gas1->CheckHsCrossing(R1) || _Gas2->CheckHsCrossing(R2) ||
           CheckHsIntCrossing(R1,R2);
}

bool Mixture_HS::CheckHsIntCrossing(cpos_vec_type& R1, cpos_vec_type& R2) const
{
#ifdef DEBUG
    assert(R1.size() == _Natoms_Gas1);
    assert(R2.size() == _Natoms_Gas2);
#endif 
    for(auto pos1: R1){
        for(auto pos2 : R2){
            if(abs(pos1.x - pos2.x) > _Rcore_int){
                continue;
            }
            else{
                if(abs(pos1.y - pos2.y) > _Rcore_int){
                    continue;
                }
                else{
                    if(mod(pos1 - pos2) <= _Rcore_int){
                        // crossing encountered, return true
                        return true;
                    }
                }
            }
        }
    }
    return false;

}

bool 
Mixture_HS::is_configuration_physical(cpos_vec_type& R1,cpos_vec_type& R2) const
{
    return !CheckHsCrossing(R1,R2);
}


//###############################################
//#             Printing Functions              #
//###############################################

void Mixture_HS::print_TWF_tofile(const std::string& filename)
{
    size_t last_index = filename.find_last_of(".");
    std::string rawname = filename.substr(0,last_index);
    std::string ext = filename.substr(last_index,filename.size());
    
    //print gasses TWF
    _Gas1->print_TWF_tofile(rawname + "_Gas1" + ext);
    _Gas2->print_TWF_tofile(rawname + "_Gas2" + ext);

    //print interactino TWF
    std::string filename_int = rawname + "_int" + ext;

    std::ofstream ofs(filename_int,std::fstream::out);
    ofs << "step,r,corr,corrprime,corrdoubleprime\n";
    const uint NPartititions = 5000; 
    double deltar = 10.*_Rv_int/double(NPartititions);
    double r;

    for(uint step = 0; step < NPartititions; ++step){
        r = deltar * step;
        if(r < _Rcore_int){
            ofs << step << "," << r <<"," 
                << std::setprecision(15)
                << 0.0 <<"," << 0.0 << "," << 0.0 << std::endl;
                
        }else{
            ofs << step << "," << r << "," 
                << std::setprecision (15)
                << twobody_scatt_sol(r) << "," 
                << twobody_scatt_sol_prime(r) << ","
                << twobody_scatt_sol_dprime(r) << std::endl;
        }

    }

}

void Mixture_HS::print_conf_tofile(const std::string& filename,
                                  cpos_vec_type& R1, cpos_vec_type& R2)
{
    size_t last_index = filename.find_last_of(".");
    std::string rawname = filename.substr(0,last_index);
    std::string ext = filename.substr(last_index,filename.size());

    _Gas1->print_conf_tofile(rawname + "_Gas1" + ext,R1);
    _Gas2->print_conf_tofile(rawname + "_Gas2" + ext,R2);
}



//###############################################
//#    two body scattering problem solutions    #
//###############################################

double Mixture_HS::twobody_scatt_sol(double r) const
{
    if(r > _Rv_int)
        return 1 - _Gamma_int*exp( -r/ _alpha_int);
    else if (r <= _Rv_int and r > _Rcore_int)
        return j0(_k0_int*r) + _C_int * y0(_k0_int*r);
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r << " < Rcore \n";
    }
}

double Mixture_HS::twobody_scatt_sol_prime(double r) const
{
   if(r > _Rv_int)
        return _Gamma_int*exp( -r/ _alpha_int)/_alpha_int;
    else if (r <= _Rv_int and r > _Rcore_int)
        return - _k0_int*( j1( _k0_int*r) + _C_int* y1(_k0_int*r) ); 
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r  << " < Rcore \n";
    } 
}

double Mixture_HS::twobody_scatt_sol_dprime(double r) const 
{
    if(r > _Rv_int)
        return - _Gamma_int*exp(-r/ _alpha_int)/ (_alpha_int*_alpha_int);
    else if (r <= _Rv_int and r > _Rcore_int)
        return -_k0_int*_k0_int/2. * 
               (
                    (j0(_k0_int*r) - jn(2,_k0_int*r)) + 
                    _C_int*(y0(_k0_int*r) - yn(2,_k0_int*r))
               );
    else{
        return 0;
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ r="<< r  <<" < Rcore \n";
    }
}




bool Mixture_HS::check_size(cpos_vec_type & R1,cpos_vec_type & R2) const
{
    return (R1.size() == _Natoms_Gas1 and R2.size() == _Natoms_Gas2);
}

} // namespace mcs