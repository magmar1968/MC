#ifndef __MC_OPTIONS_H__
#define __MC_OPTIONS_H__


enum class McOptions
{
    DMC                     = 1ul << 0,  // Diffusion MC option on
    VMC                     = 1ul << 1,  // Variational MC option on
    PureGas                 = 1ul << 2,  // Simulate the behaviour of a pure gas
    GasMixture              = 1ul << 3,  // Simulate the behaviour of a gas mixture
    AddVortex               = 1ul << 4,  // Add a vortex to the first gas 
    SetVortTWFtoOne         = 1ul << 5,  // Set  vortex TWF to one
    PrintEnergyEvolution    = 1ul << 6,  // Print the energy evolution of the process
    PrintDensityProfile     = 1ul << 7,  // Print the density profile of the system 
    PrintInitConfiguration  = 1ul << 8,  // Print the initial configuration
    PrintFinalConfiguration = 1ul << 9,  // Print the system final configuration
    PrintTrialWaveFunction  = 1ul << 10, // Print the shape of the TWF
    PrintSimulationOptions  = 1ul << 11, // Print to screen the simulation parameters
    ReadInitConfFromFile    = 1ul << 12, // Read Initial Gas Configuration From File
    Initial                 = 1ul << 13
};


inline McOptions operator|(McOptions a, McOptions b)
{
    return static_cast<McOptions>(static_cast<int>(a) | static_cast<int>(b));
}


inline McOptions operator&(McOptions a, McOptions b)
{
    return static_cast<McOptions>(static_cast<int>(a) & static_cast<int>(b));
}

inline McOptions operator^(McOptions a, McOptions b)
{
    return static_cast<McOptions>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline bool is_flag_on(McOptions opt, McOptions  flag)
{
    return int(opt & flag) != 0;
}

inline bool is_flag_off(McOptions opt, McOptions  flag)
{
    return !is_flag_on(opt,flag);
}


#endif 