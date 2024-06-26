#include "system_mixture.hpp"


namespace mcs
{



System_Mixture::System_Mixture(Mixture_HS * Mixture, DensProfile*DP1, DensProfile * DP2,
                               pos_vec_type& R1, pos_vec_type& R2)
    :_Mixture(Mixture),_DensProfile_Gas1(DP1),_DensProfile_Gas2(DP2),
    _R1(R1),_R2(R2)
{   
    VMC_step_made = false;
    DMC_step_made = true;

    if( ! _Mixture->is_configuration_physical(_R1,_R2) ){
        throw std::invalid_argument("Error: the initial configuration is not physical\n");
    }
    double E_part  = _Mixture->EkinPartial(_R1,_R2);
    _E_OLD.EkinFor = _Mixture->EkinFor(_R1,_R2);
    _E_OLD.Ekin    = E_part - _E_OLD.EkinFor;
    _E_OLD.Epot    = _Mixture->Epot(_R1,_R2);
    _E_OLD.Elocal  = _E_OLD.Ekin + _E_OLD.Epot; 
}

System_Mixture::System_Mixture(const System_Mixture& S)
{
    //deep copy positions
    _R1 = S._R1;
    _R2 = S._R2;

    //shallow copy arguments
    _Mixture = S._Mixture;
    _DensProfile_Gas1 = S._DensProfile_Gas1;
    _DensProfile_Gas2 = S._DensProfile_Gas2;
    VMC_step_made = S.VMC_step_made;
    DMC_step_made = S.DMC_step_made;

    _E_NEW = S._E_NEW;
    _E_OLD = S._E_OLD;   
}



bool System_Mixture::try_VMC_step(double dt)
{
    double D1 = _Mixture->Get_D1();
    double D2 = _Mixture->Get_D2();
    double sigma1 = sqrt(2*D1*dt);
    double sigma2 = sqrt(2*D2*dt);
    
    _R1_TMP = _Mixture->diffuse_gas1(_R1,sigma1);
    _R2_TMP = _Mixture->diffuse_gas2(_R2,sigma2);

    if(_Mixture->is_configuration_physical(_R1_TMP,_R2_TMP)){
        VMC_step_made = true;
        return true;
    }
    else{
        VMC_step_made = false;
        return false;
    }
}

void System_Mixture::accept_VMC_step()
{
    if(VMC_step_made == false){
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ :     \n"
                  << "      no VMC step have been made    \n";
        return;
    }
    VMC_step_made = false;
    //the temporary configuration is now the current one
    _R1.swap(_R1_TMP);
    _R2.swap(_R2_TMP);

}

//###############################################
//#            Diffusional MC step              #
//###############################################

bool System_Mixture::try_DMC_step(double dt)
{
    double D1 = _Mixture->Get_D1();
    double D2 = _Mixture->Get_D2();
    double sigma1 = sqrt(2*D1*dt);
    double sigma2 = sqrt(2*D2*dt);
    

    size_t size1 = _R1.size();
    size_t size2 = _R2.size();

    pos_vec_type R1_DIFFUSED(size1),R1_1(size1),R1_2(size1);
    pos_vec_type R2_DIFFUSED(size2),R2_1(size2),R2_2(size2);
    force_vec_type F1_1(size1), F2_1(size2);

    //we make use that a DMC step is just a VMC step + drift
    R1_DIFFUSED = _Mixture->diffuse_gas1(_R1,sigma1);
    R2_DIFFUSED = _Mixture->diffuse_gas2(_R2,sigma2);

    //if the configuration is NOT physical
    if( ! _Mixture->is_configuration_physical(R1_DIFFUSED,R2_DIFFUSED))
    {
        DMC_step_made = false;
        return false;
    }
    // else: DRIFT!
    F1_1 = _Mixture->F1(_R1,_R2);
    R1_1 = R1_DIFFUSED + D1*dt*F1_1/2.;
    R1_2 = R1_DIFFUSED + D1*dt*(F1_1 + _Mixture->F1(R1_1,_R2))/4.;

    F2_1 = _Mixture->F2(_R1,_R2);
    R2_1 = R2_DIFFUSED + D2*dt*F2_1/2.;
    R2_2 = R2_DIFFUSED + D2*dt*(F2_1 + _Mixture->F2(_R1,R2_1))/4.;

    double E_part  = _Mixture->EkinPartial(R1_2,R2_2);
    _E_NEW.EkinFor = _Mixture->EkinFor(R1_2,R2_2);
    _E_NEW.Ekin    = E_part - _E_NEW.EkinFor;
    _E_NEW.Epot    = _Mixture->Epot(R1_2,R2_2);
    _E_NEW.Elocal  = _E_NEW.Ekin + _E_NEW.Epot;

    _R1_TMP = R1_DIFFUSED + D1*dt*_Mixture->F1(R1_2,_R2)/2.;
    _R2_TMP = R2_DIFFUSED + D2*dt*_Mixture->F2(_R1,R2_2)/2.;

    DMC_step_made = true;

    return true;
}

void System_Mixture::accept_DMC_step()
{
    if(DMC_step_made == false){
        std::cerr << "ERROR: in __PRETTY_FUNCTION__ :     \n"
                  << "      no DMC step have been made    \n";
        return;
    }
    //the temporary configuration is now the current one
    _R1.swap(_R1_TMP);
    _R2.swap(_R2_TMP);

    DMC_step_made = false;
    _E_OLD = _E_NEW;

}


void System_Mixture::update_DensProfile()
{
    //update density profile
    _DensProfile_Gas1->update(_R1);
    _DensProfile_Gas2->update(_R2);
}

void System_Mixture::store_DensProfile()
{
    //store density profiles
    _DensProfile_Gas1->store_result();
    _DensProfile_Gas2->store_result();
}

//###############################################
//#                 Getter TWF                  #
//###############################################

double System_Mixture::Get_NEW_TWF()
{
    return _Mixture->trial_WF(_R1_TMP,_R2_TMP);
}

double System_Mixture::Get_OLD_TWF()
{
    return _Mixture->trial_WF(_R1,_R2); 
}



//###############################################
//#             Getter Energies                 #
//###############################################

Energies System_Mixture::Get_Energies() const
{
    return _E_OLD;
}


//###############################################
//#        Getter Configuration                 #
//###############################################
std::vector<vector_2D> System_Mixture::Get_R(uint WhichGas) const 
{   
    if(!(WhichGas == 1 or WhichGas == 2))
        throw std::invalid_argument( "ERROR: in System_Mixture::Get_R input must be either 1 or 2\n");

    if(WhichGas == 1)
        return GetR1();
    else
        return GetR2();
}

//###############################################
//#            Printing Functions               #
//###############################################

void System_Mixture::print_conf_tofile(const std::string & filename)
{
    _Mixture->print_conf_tofile(filename,_R1,_R2);
}

void System_Mixture::print_dens_profile_tofile(const std::string & filename)
{
    size_t last_index = filename.find_last_of(".");
    std::string rawname = filename.substr(0,last_index);
    std::string ext = filename.substr(last_index,filename.size());

    _DensProfile_Gas1->print_norm_density_profile(rawname + "_Gas1" + ext);
    _DensProfile_Gas2->print_norm_density_profile(rawname + "_Gas2" + ext);
}


}
