#include "system_puregas.hpp"

namespace mcs
{
    



System_PureGas::System_PureGas(PureGas * Gas,
                              DensProfile* DensProfile, cpos_vec_type& init_R)
    :_Gas(Gas),_DensProfile(DensProfile),_R(init_R)
{
    VMC_step_made = false;
    DMC_step_made = false;

    OLD_EL = _Gas->Elocal(_R);

}

System_PureGas::System_PureGas(const System_PureGas& S)
{
    _R = S._R;
    
    _Gas = S._Gas;
    _DensProfile = S._DensProfile;
    VMC_step_made = S.VMC_step_made;
    DMC_step_made = S.DMC_step_made;

    NEW_EL = S.NEW_EL;
    OLD_EL = S.OLD_EL;
}

//###############################################
//#            Variational MC step              #
//###############################################

bool System_PureGas::try_VMC_step(double dt)
{
    double D = _Gas->Get_D();
    double sigma = sqrt(2*D*dt);
    _R_TMP = _Gas->diffuse(_R,sigma);
    VMC_step_made = true;
    return _Gas->is_configuration_physical(_R_TMP);
}

void System_PureGas::accept_VMC_step()
{
    if(VMC_step_made == false){
        std::cerr << "ERROR: in __FUNCTION__ :     \n"
                  << "      no VMC step have been made    \n";
        return;
    }
    VMC_step_made = false;
    //The temporary configuration is now the current one
    _R.swap(_R_TMP);
}

//###############################################
//#            Diffusional MC step              #
//###############################################

bool System_PureGas::try_DMC_step(double dt)
{
    double D = _Gas->Get_D();
    double sigma = sqrt(2*D*dt);
    size_t size = _R.size();
    
    //diffuse
    pos_vec_type R_DIFFUSED(size);
    R_DIFFUSED = _Gas->diffuse(_R,sigma);
    
    if( ! _Gas->is_configuration_physical(R_DIFFUSED)){
        return false;
    }

    //drifting
    pos_vec_type R1(size),R2(size);
    force_vec_type F1(size);

    F1 = _Gas->F(_R);
    R1 = R_DIFFUSED + D*dt*F1/2.;
    R2 = R_DIFFUSED + D*dt*(F1 + _Gas->F(R1))/4.;

    NEW_EL = _Gas->Elocal(R2);
    _R_TMP = R_DIFFUSED + D*dt*_Gas->F(R2)/2.;

    DMC_step_made = true;
    return true;
}

void System_PureGas::accept_DMC_step()
{
    if(DMC_step_made == false){
        std::cerr << "ERROR: in __FUNCTION__ :     \n"
                  << "      no DMC step have been made    \n";
        return;
    }
    //The temporary configuration is now the current one
    _R.swap(_R_TMP);
    DMC_step_made = false;
    OLD_EL = NEW_EL;
}

void System_PureGas::update_DensProfile()
{
    //update density profile
    _DensProfile->update(_R);
}

//###############################################
//#                 Getter TWF                  #
//###############################################

double System_PureGas::Get_NEW_TWF()
{
    return _Gas->trial_WF(_R_TMP);
}

double System_PureGas::Get_OLD_TWF()
{
    return _Gas->trial_WF(_R); 
}


//###############################################
//#             Getter Energies                 #
//###############################################

Energies System_PureGas::Get_Energies() const
{
    Energies Energies;
    Energies.Ekin = _Gas->Ekin(_R);
    Energies.EkinFor = _Gas->EkinFor(_R);
    Energies.Epot = _Gas->Epot(_R);
    Energies.Elocal = Energies.Epot + Energies.Ekin;
    return Energies;
}

//###############################################
//#             Printing Functions              #
//###############################################
void System_PureGas::print_conf_tofile(const std::string& filename)
{

    _Gas->print_conf_tofile(filename,_R);    

}

void System_PureGas::print_dens_profile_tofile(const std::string& filename)
{
    _DensProfile->print_norm_density_profile(filename);
}



} // namespace mcs

