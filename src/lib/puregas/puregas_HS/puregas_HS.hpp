#ifndef __PUREGAS_HS_HPP__
#define __PUREGAS_HS_HPP__
#include "../puregas.hpp"
#include "../../utility/fast_exp/fast_exp.hpp"
#include <fstream> //fstream
#include <iomanip> //setprecision

namespace mcs
{


struct gas_parameters
{
    double k0;
    double alpha;
    double Rv;
    double Rcore;
    double D;
    double harmonic_lenght;
    uint Natoms;
};

class PureGas_HS : public PureGas
{
public:
    PureGas_HS(){};
    PureGas_HS(gas_parameters&);

    pos_vec_type gen_initial_conf() ;

    //Trial Wave Function
    virtual double trial_WF(cpos_vec_type& ) const;

    //Energies
    virtual double Elocal(cpos_vec_type&   ) const;
    virtual double Ekin(cpos_vec_type&     ) const;
    virtual double EkinPartial(cpos_vec_type&) const;
    virtual double EkinFor(cpos_vec_type&  ) const;
    virtual double Epot(cpos_vec_type&     ) const;

    //Force
    virtual force_vec_type F(cpos_vec_type&) const;

    //diffuse
    pos_vec_type diffuse(cpos_vec_type&,double);


    //Check Harc Core Crossing
    bool CheckHsCrossing(cpos_vec_type&) const;
    bool is_configuration_physical(cpos_vec_type&) const;

    //printing
    void print_TWF_tofile(const std::string& filename  ) const;  
    void print_conf_tofile(const std::string&filename ,
                                 cpos_vec_type&) const;

protected:
    double twobody_scatt_sol(double)        const ;
    double twobody_scatt_sol_prime(double)  const ;
    double twobody_scatt_sol_dprime(double) const ;
private:
    double _k0;     //
    double _alpha;  //
    double _1overalpha; //
    double _Rv;     // 
    double _Rcore;  //
    double _Gamma;  //
    double _C;      //

};

}


#endif