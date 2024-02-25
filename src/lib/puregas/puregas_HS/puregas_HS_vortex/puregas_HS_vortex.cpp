#include "puregas_HS_vortex.hpp"

namespace mcs
{
    
PureGas_HS_Vortex::PureGas_HS_Vortex(gas_parameters& param, double a_vortex, bool SetToOne)
    :PureGas_HS(param),_a_vortex(a_vortex),_SetToOne(SetToOne)
{
    _1overa = 1./_a_vortex;
}

//###############################################
//#             Trial Wave Function             #
//###############################################

/// @brief compute the  exp trial Wave function starting from the configuration @p R
/// @param R configuration 
/// @return trial Wave function value
double PureGas_HS_Vortex::trial_WF(cpos_vec_type& R) const 
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif   
    if(_SetToOne)
        return PureGas_HS::trial_WF(R);

    double r;
    double u = 0;

    for(auto pos : R)
    {
        r = mod(pos);
        u += log(vortex_sol(r));
    }

    return PureGas_HS::trial_WF(R) + u;
}

//###############################################
//#                  Energies                   #
//###############################################

/// @brief return the Local Energy of the configuration @p R
/// @param R atoms configuration
/// @return local energy
double PureGas_HS_Vortex::Elocal(cpos_vec_type & R) const
{
    return Ekin(R) + Epot(R);
}

//================ POTENTIAL ENERGY =====================//

/// @brief Compute and return the Potential Energy of the
///        configuration @p R
/// @param R 
/// @return potential energy
double PureGas_HS_Vortex::Epot(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif

    double epot =0.;
    double r;

    for(auto pos : R)
    {
        r = mod(pos);
        epot += _D * ( _l*_l /(r*r));
    }

    return PureGas_HS::Epot(R) + epot;
}

//=============== KINETIC ENERGY ================//


/// @brief Compute and return the kinetic energy of the
///        configuration @p R
/// @param R 
/// @return kinetic energy
double PureGas_HS_Vortex::Ekin(cpos_vec_type& R) const
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
double PureGas_HS_Vortex::EkinPartial(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif  
    if(_SetToOne)
        return PureGas_HS::EkinPartial(R);
    
    double ekinpartial = 0.;
    double r = 0.;

    for(auto pos : R)
    {
        r = mod(pos);

        //reduce comp computing exp less times
        double k = _1overa/vortex_sol(r);

        ekinpartial += -_D*k*(1./r - k);
    }
    
    return PureGas_HS::EkinPartial(R) + ekinpartial;
}

//================ EKinFor ====================//

/// @brief Compute and return the Ekinf of the configuration
///        @p R.
/// @param R 
/// @return EkinFor
double PureGas_HS_Vortex::EkinFor(cpos_vec_type& R) const
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    double ekinfor = 0.;
    std::vector<vector_2D> drift(R.size());
    drift = PureGas_HS_Vortex::F(R);

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
PureGas::force_vec_type PureGas_HS_Vortex::F(cpos_vec_type& R) const 
{
#ifdef DEBUG
    assert(R.size() == _Natoms);
#endif
    if(_SetToOne)
        return PureGas_HS::F(R);


    std::vector<vector_2D> Force(R.size());
    vector_2D r_hat;
    double r, up;
    //force inbetween particles
    for(uint i_atom = 0; i_atom < R.size() -1;  ++i_atom ){
        for(uint j_atom = i_atom +1; j_atom < R.size(); ++ j_atom){
            r = mod(R[i_atom] - R[j_atom] );
            //normalized versor
            r_hat = (R[i_atom] - R[j_atom])/ r;
            up = twobody_scatt_sol_prime(r)/twobody_scatt_sol(r);

            Force[i_atom] += up*r_hat;
            Force[j_atom] -= up*r_hat;
        } 
    }

    //force due to  harmonic potential and vortex
    for(uint k_atom = 0; k_atom < R.size(); ++ k_atom){
        r = mod(R[k_atom]);
        //normalize versor
        r_hat = R[k_atom]/r;
        //harmonic potential
        up = -r*_1over_h_len*_1over_h_len;
        //vortex
        up += 1./(_a_vortex * vortex_sol(r));
        Force[k_atom] += up*r_hat;
    }

    //factors 2 come from force definition itself
    return 2*Force;
}

double PureGas_HS_Vortex::vortex_sol(double r) const
{
    return 1 - exp(-r/_a_vortex);
}




} // namespace mcs
