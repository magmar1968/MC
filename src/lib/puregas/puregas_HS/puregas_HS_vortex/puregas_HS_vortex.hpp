#ifndef __PUREGAS_HS_VORTEX_HPP__
#define __PUREGAS_HS_VORTEX_HPP__
#include "../puregas_HS.hpp"

namespace mcs
{


class PureGas_HS_Vortex : public PureGas_HS
{
public:
    PureGas_HS_Vortex(){};
    PureGas_HS_Vortex(gas_parameters&, double,bool SetToOne = false);

    double trial_WF(cpos_vec_type&) const override;

      //Energies
    double Elocal(cpos_vec_type&   ) const override;
    double Ekin(cpos_vec_type&     ) const override;
    double EkinPartial(cpos_vec_type&) const override;
    double EkinFor(cpos_vec_type&  ) const override;
    double Epot(cpos_vec_type&     ) const override;

    //Force
    force_vec_type F(cpos_vec_type&) const override;

    //setter
    void inline SetToOne(){_SetToOne = true;}

private:
    double vortex_sol(double)        const;

private:
    double _a_vortex;  //vortex size
    double _1overa;    // 1/a_vortex
    double _l = 1;     //angular momentum eigenstate

    bool   _SetToOne = false;

};
    
} // namespace mcs





#endif 