#include "system_puregas.hpp"

namespace mcs
{
    



System_PureGas::System_PureGas(PureGas * Gas,
                              DensProfile* DensProfile, cpos_vec_type& init_R)
    :_Gas(Gas),_DensProfile(DensProfile),_R(init_R)
{
    VMC_step_made = false;
    DMC_step_made = false;

    if(! _Gas->is_configuration_physical(_R)){
        throw std::invalid_argument("Error: the initial configuration is not physical\n");
    }
    
    double E_part  = _Gas->EkinPartial(_R);
    _E_OLD.EkinFor = _Gas->EkinFor(_R);
    _E_OLD.Ekin    = E_part - _E_OLD.EkinFor;
    _E_OLD.Epot    = _Gas->Epot(_R);
    _E_OLD.Elocal  = _E_OLD.Ekin + _E_OLD.Epot; 

}

System_PureGas::System_PureGas(const System_PureGas& S)
{
    _R = S._R;
    
    _Gas = S._Gas;
    _DensProfile = S._DensProfile;
    VMC_step_made = S.VMC_step_made;
    DMC_step_made = S.DMC_step_made;

    _E_NEW = S._E_NEW;
    _E_OLD = S._E_OLD;
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
    double E_part;
    
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

    E_part         = _Gas->EkinPartial(R2);
    _E_NEW.EkinFor = _Gas->EkinFor(R2);
    _E_NEW.Ekin    = E_part - _E_NEW.EkinFor;
    _E_NEW.Epot    = _Gas->Epot(R2);
    _E_NEW.Elocal  = _E_NEW.Ekin + _E_NEW.Epot; 
    

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
    _E_OLD = _E_NEW;
}

void System_PureGas::update_DensProfile()
{
    //update density profile
    _DensProfile->update(_R);
}

void System_PureGas::store_DensProfile()
{
    //store block density profile
    _DensProfile->store_result();
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
    return _E_OLD;
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

