#ifndef __SYSTEM_PUREGAS_HPP__
#define __SYSTEM_PUREGAS_HPP__
#include "../system.hpp"
#include "../../puregas/puregas.hpp" //PureGas
#include "../../utility/2D_vector.hpp" //vector 2D
#include "../../density_profile/density_profile.hpp" //density profile

namespace mcs
{


class System_PureGas : public System
{
private:
    using pos_vec_type = std::vector<vector_2D>;
    using cpos_vec_type = const pos_vec_type;
    using force_vec_type = std::vector<vector_2D>;
public:

    System_PureGas(){};
    System_PureGas(PureGas *, DensProfile*, cpos_vec_type&);
    System_PureGas(const System_PureGas&);
    ~System_PureGas(){};

    bool try_VMC_step(double);
    bool try_DMC_step(double);

    void accept_VMC_step();
    void accept_DMC_step();

    void update_DensProfile();

    void print_conf_tofile(const std::string& filename);
    void print_dens_profile_tofile(const std::string& filename);

    std::vector<vector_2D> Get_R(uint WhichGas) const {return _R;};

    double Get_NEW_TWF();
    double Get_OLD_TWF();

    Energies Get_Energies() const;

    System* copy() const
    {
        return new System_PureGas(*this);
    }

private:
    PureGas * _Gas;
    DensProfile * _DensProfile;
    
    std::vector<vector_2D> _R;     //store the current configuration
    std::vector<vector_2D> _R_TMP; //store the possible configuration

    
};
}

#endif 