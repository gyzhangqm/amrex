#ifndef CNS_H_
#define CNS_H_

#include <AMReX_AmrLevel.H>
#include <AMReX_ErrorList.H>

class CNS
    :
    public amrex::AmrLevel
{
public:

    CNS ();
    CNS (amrex::Amr&            papa,
         int                    lev,
         const amrex::Geometry& level_geom,
         const amrex::BoxArray& bl,
         const amrex::DistributionMapping& dm,
         amrex::Real            time);
    virtual ~CNS ();

    CNS (const CNS& rhs) = delete;
    CNS& operator= (const CNS& rhs) = delete;
    
    // Restart from a checkpoint file.
    virtual void restart (amrex::Amr&     papa,
                          std::istream&   is,
                          bool            bReadSpecial = false) override;

    // Write checkpoint
    virtual void checkPoint(const std::string& dir,
                            std::ostream&      os,
                            amrex::VisMF::How  how = amrex::VisMF::NFiles,
                            bool               dump_old = true) override;

    // Write a plotfile to specified directory.
    virtual void writePlotFile (const std::string& dir,
                                std::ostream&      os,
                                amrex::VisMF::How  how) override;

    // Initialize data on this level from another CNS (during regrid).
    virtual void init (amrex::AmrLevel& old) override;

    // Initialize data on this level after regridding if old level did not previously exist
    virtual void init () override;
    
    // Initialize grid data at problem start-up.
    virtual void initData () override;

    // Advance grids at this level in time.
    virtual amrex::Real advance (amrex::Real time,
                                 amrex::Real dt,
                                 int  iteration,
                                 int  ncycle) override;

    virtual void computeInitialDt (int                                 finest_level,
                                   int                                 sub_cycle,
                                   amrex::Array<int>&                  n_cycle,
                                   const amrex::Array<amrex::IntVect>& ref_ratio,
                                   amrex::Array<amrex::Real>&          dt_level,
                                   amrex::Real                         stop_time) override;

    virtual void computeNewDt (int                                 finest_level,
                               int                                 sub_cycle,
                               amrex::Array<int>&                  n_cycle,
                               const amrex::Array<amrex::IntVect>& ref_ratio,
                               amrex::Array<amrex::Real>&          dt_min,
                               amrex::Array<amrex::Real>&          dt_level,
                               amrex::Real                         stop_time,
                               int                                 post_regrid_flag) override;

    virtual void post_regrid (int lbase, int new_finest) override;

    // Do work after timestep().
    virtual void post_timestep (int iteration) override;

    // After a full time step
    virtual void postCoarseTimeStep (amrex::Real time) override;

    // Do work after init().
    virtual void post_init (amrex::Real stop_time) override;

    virtual void post_restart () override;

    // Error estimation for regridding.
    virtual void errorEst (amrex::TagBoxArray& tb,
                           int                 clearval,
                           int                 tagval,
                           amrex::Real         time,
                           int                 n_error_buf = 0,
                           int                 ngrow = 0) override;

    // Define data descriptors.
    static void variableSetUp ();

    // Define tagging functions.
    static void ErrorSetUp ();

    // Cleanup data descriptors at end of run.
    static void variableCleanUp ();

    static void LoadBalance (amrex::Amr& amr);

    static int do_load_balance;

protected:

    static void read_params ();
    static int numGrow() { return NUM_GROW; };

    const amrex::MultiFab& volFrac () const { return volfrac; }

    CNS& getLevel (int lev) { return dynamic_cast<CNS&>(parent->getLevel(lev)); }

    void avgDown ();

    void buildMetrics ();

    amrex::Real estTimeStep ();

    // Compute initial time step.
    amrex::Real initialTimeStep ();

    void computeTemp (amrex::MultiFab& State, int ng);

    void compute_dSdt (const amrex::MultiFab& S, amrex::MultiFab& dSdt, amrex::Real dt);

    void fixUpGeometry ();

    amrex::MultiFab volfrac;
    amrex::MultiFab bndrycent;
    std::array<amrex::MultiFab,3> areafrac;
    std::array<amrex::MultiFab,3> facecent;

    static constexpr int NUM_GROW = 5;
    enum StateVariable {
        Density = 0, Xmom, Ymom, Zmom, Eden, Eint, Temp,
        NUM_STATE
    };

    enum StateDataType {
        State_Type = 0,
        Cost_Type
    };
    static int num_state_data_types;

    static amrex::ErrorList err_list;
    static amrex::BCRec     phys_bc;

    // Parameters
    static int verbose;
    static amrex::IntVect hydro_tile_size;
    static amrex_real cfl;
};

#endif
