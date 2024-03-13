#ifndef __SYSTEM_MIXTURE_HPP__
#define __SYSTEM_MIXTURE_HPP__

#include "../system.hpp"
#include "../../mixture/mixture_HS.hpp" //mixture
#include "../../utility/2D_vector.hpp" //vector 2D
#include "../../density_profile/density_profile.hpp" //density profile

#include <stdexcept> //std::invalid_argument


namespace mcs{



class System_Mixture : public System
{
private:
    using pos_vec_type = std::vector<vector_2D>;
    using cpos_vec_type = const pos_vec_type;
    using force_vec_type = std::vector<vector_2D>;
public:
    System_Mixture(){};
    System_Mixture(Mixture_HS *, DensProfile * ,DensProfile *,
                  pos_vec_type&, pos_vec_type&);

    System_Mixture(const System_Mixture&);

    bool try_VMC_step(double);
    bool try_DMC_step(double);

    void accept_VMC_step();
    void accept_DMC_step();

    void update_DensProfile();


    void print_conf_tofile(const std::string& filename);
    void print_dens_profile_tofile(const std::string& filename);

    double Get_NEW_TWF();
    double Get_OLD_TWF();

    std::vector<vector_2D> Get_R(uint WhichGas) const; 


    Energies Get_Energies() const;

    System* copy() const
    {
        return new System_Mixture(*this);
    };

private:
    std::vector<vector_2D> GetR1() const {return _R1;}
    std::vector<vector_2D> GetR2() const {return _R2;}

private:
    Mixture_HS * _Mixture;
    DensProfile * _DensProfile_Gas1;
    DensProfile * _DensProfile_Gas2;

    std::vector<vector_2D> _R1;
    std::vector<vector_2D> _R2;

    std::vector<vector_2D> _R1_TMP;
    std::vector<vector_2D> _R2_TMP;

    
};

}


#endif