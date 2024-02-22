#ifndef __MIXTURE_HS_HPP__
#define __MIXTURE_HS_HPP__
#include "../puregas/puregas_HS/puregas_HS.hpp" //PureGas
#include <string> //string
#include <vector> //vectors
#include <assert.h>


namespace mcs{


struct gas_inter_params
{
    double k0;
    double alpha;
    double Rv;
    double Rcore;
    size_t Natoms_Gas1;
    size_t Natoms_Gas2;
};


class Mixture_HS{
private:
    using pos_vec_type = std::vector<vector_2D>;
    using cpos_vec_type = const pos_vec_type;
    using force_vec_type = std::vector<vector_2D>;

public:
    Mixture_HS(){};
    Mixture_HS(PureGas_HS*,PureGas_HS*,gas_inter_params&);

    cpos_vec_type gen_initial_conf_gas1(){return _Gas1->gen_initial_conf();}
    cpos_vec_type gen_initial_conf_gas2(){return _Gas2->gen_initial_conf();}

    //trial Wave Function
    double trial_WF(cpos_vec_type&,cpos_vec_type& ) const ;

    //energies
    double Elocal(cpos_vec_type&,cpos_vec_type&   ) const ;
    double Ekin(cpos_vec_type&,cpos_vec_type&     ) const ;
    double EkinFor(cpos_vec_type&,cpos_vec_type&  ) const ;
    double Epot(cpos_vec_type&,cpos_vec_type&     ) const ;

    //force
    force_vec_type F1(cpos_vec_type&,cpos_vec_type&) const;
    force_vec_type F2(cpos_vec_type&,cpos_vec_type&) const;
    force_vec_type F(cpos_vec_type&,cpos_vec_type&) const;


    //diffuse 
    pos_vec_type diffuse_gas1(cpos_vec_type&,double);
    pos_vec_type diffuse_gas2(cpos_vec_type&,double);

    //Check Harc Core Crossing
    bool CheckHsCrossing(cpos_vec_type&,cpos_vec_type&) const;
    bool CheckHsIntCrossing(cpos_vec_type&,cpos_vec_type&) const;

    bool is_configuration_physical(cpos_vec_type&,cpos_vec_type&) const;

    //Printing
    void print_TWF_tofile(const std::string& filename );  
    void print_conf_tofile(const std::string& filename,
                           cpos_vec_type&,cpos_vec_type&);

    //getters
    double Get_D1()const {return _Gas1->Get_D();};
    double Get_D2()const {return _Gas2->Get_D();};


private:
    double twobody_scatt_sol(double)        const ;
    double twobody_scatt_sol_prime(double)  const ;
    double twobody_scatt_sol_dprime(double) const ;

    bool  check_size(cpos_vec_type&, cpos_vec_type&) const;

private:
    PureGas_HS * _Gas1;
    PureGas_HS * _Gas2;


    double _k0_int;
    double _alpha_int;
    double _Rv_int;
    double _Rcore_int;
    double _Gamma_int;
    double _C_int;

    size_t _Natoms_Gas1;
    size_t _Natoms_Gas2;


};
}

#endif