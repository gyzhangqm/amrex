
#include <AMReX_MultiFab.H>
#include <AMReX_iMultiFab.H>
#include <AMReX_Geometry.H>

using namespace amrex;

extern "C" {

    void amrex_fi_new_multifab (MultiFab*& mf, const BoxArray*& ba, 
				const DistributionMapping*& dm,
				int nc, int ng, const int* nodal)
    {
	mf = new MultiFab(amrex::convert(*ba, IntVect(nodal)), *dm, nc, ng);
	ba = &(mf->boxArray());
	dm = &(mf->DistributionMap());
    }

    void amrex_fi_delete_multifab (MultiFab* mf)
    {
	delete mf;
    }

    int amrex_fi_multifab_ncomp (const MultiFab* mf)
    {
	return mf->nComp();
    }

    int amrex_fi_multifab_ngrow (const MultiFab* mf)
    {
	return mf->nGrow();
    }

    const BoxArray* amrex_fi_multifab_boxarray (const MultiFab* mf)
    {
	return &(mf->boxArray());
    }

    const DistributionMapping* amrex_fi_multifab_distromap (const MultiFab* mf)
    {
	return &(mf->DistributionMap());
    }
    
    void amrex_fi_multifab_dataptr_iter (MultiFab* mf, MFIter* mfi, Real*& dp, int lo[3], int hi[3])
    {
	FArrayBox& fab = (*mf)[*mfi];
	dp = fab.dataPtr();
	const Box& bx = fab.box();
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	}
    }

    void amrex_fi_multifab_dataptr_int (MultiFab* mf, int igrd, Real*& dp, int lo[3], int hi[3])
    {
	FArrayBox& fab = (*mf)[igrd];
	dp = fab.dataPtr();
	const Box& bx = fab.box();
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	}
    }

    Real amrex_fi_multifab_min (const MultiFab* mf, int comp, int nghost)
    {
	return mf->min(comp,nghost);
    }

    Real amrex_fi_multifab_max (const MultiFab* mf, int comp, int nghost)
    {
	return mf->max(comp,nghost);
    }

    Real amrex_fi_multifab_norm0 (const MultiFab* mf, int comp)
    {
	return mf->norm0(comp);
    }

    Real amrex_fi_multifab_norm1 (const MultiFab* mf, int comp)
    {
	return mf->norm1(comp);
    }

    Real amrex_fi_multifab_norm2 (const MultiFab* mf, int comp)
    {
	return mf->norm2(comp);
    }

    void amrex_fi_multifab_setval (MultiFab* mf, Real val, int ic, int nc, int ng)
    {
        mf->setVal(val, ic, nc, ng);
    }

    void amrex_fi_multifab_copy (MultiFab* dstmf, const MultiFab* srcmf,
                                 int srccomp, int dstcomp, int nc, int ng)
    {
        MultiFab::Copy(*dstmf, *srcmf, srccomp, dstcomp, nc, ng);
    }

    void amrex_fi_multifab_parallelcopy (MultiFab* dstmf, const MultiFab* srcmf,
                                         int srccomp, int dstcomp, int nc,
                                         int srcng, int dstng, const Geometry* geom)
    {
        dstmf->ParallelCopy(*srcmf,srccomp,dstcomp,nc,srcng,dstng,geom->periodicity());
    }

    void amrex_fi_multifab_fill_boundary (MultiFab* mf, const Geometry* geom, 
					  int c, int nc, int cross)
    {
	mf->FillBoundary(c, nc, geom->periodicity(), cross);
    }

    void amrex_fi_build_owner_imultifab (iMultiFab*& msk, const BoxArray*& ba,
                                         const DistributionMapping*& dm,
                                         const MultiFab* data, const Geometry* geom)
    {
        auto owner_mask = data->OwnerMask(geom->periodicity());
        msk = owner_mask.release();
        ba = &(msk->boxArray());
        dm = &(msk->DistributionMap());
    }

    void amrex_fi_multifab_override_sync (MultiFab* mf, const Geometry* geom)
    {
        mf->OverrideSync(geom->periodicity());
    }

    void amrex_fi_multifab_override_sync_mask (MultiFab* mf, const Geometry* geom, const iMultiFab* msk)
    {
        mf->OverrideSync(*msk, geom->periodicity());
    }

    void amrex_fi_multifab_sum_boundary (MultiFab* mf, const Geometry* geom, int icomp, int ncomp)
    {
        mf->SumBoundary(icomp, ncomp, geom->periodicity());
    }

    void amrex_fi_multifab_average_sync (MultiFab* mf, const Geometry* geom)
    {
        mf->AverageSync(geom->periodicity());
    }

    // iMultiFab

    void amrex_fi_new_imultifab (iMultiFab*& imf, const BoxArray*& ba, 
				 const DistributionMapping*& dm,
				 int nc, int ng)
    {
	imf = new iMultiFab(*ba, *dm, nc, ng);
	ba = &(imf->boxArray());
	dm = &(imf->DistributionMap());
    }

    void amrex_fi_delete_imultifab (iMultiFab* imf)
    {
	delete imf;
    }

    void amrex_fi_imultifab_dataptr (iMultiFab* imf, MFIter* mfi, int*& dp, int lo[3], int hi[3])
    {
	IArrayBox& fab = (*imf)[*mfi];
	dp = fab.dataPtr();
	const Box& bx = fab.box();
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	}
    }

    // MFIter routines

    void amrex_fi_new_mfiter_r (MFIter*& mfi, MultiFab* mf, int tiling)
    {
	mfi = new MFIter(*mf, (bool)tiling);
    }

    void amrex_fi_new_mfiter_i (MFIter*& mfi, iMultiFab* imf, int tiling)
    {
	mfi = new MFIter(*imf, (bool)tiling);
    }

    void amrex_fi_delete_mfiter (MFIter* mfi)
    {
	delete mfi;
    }

    void amrex_fi_increment_mfiter (MFIter* mfi, int* isvalid)
    {
	++(*mfi);
	*isvalid = mfi->isValid();
    }

    void amrex_fi_mfiter_is_valid (MFIter* mfi, int* isvalid)
    {
	*isvalid = mfi->isValid();
    }

    void amrex_fi_mfiter_tilebox (MFIter* mfi, int lo[3], int hi[3], int nodal[3])
    {
	const Box& bx = mfi->tilebox();
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	const IntVect& t = bx.type();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	    nodal[i] = t[i];
	}
    }

    void amrex_fi_mfiter_nodaltilebox (MFIter* mfi, int dir, int lo[3], int hi[3], int nodal[3])
    {
	const Box& bx = mfi->nodaltilebox(dir);
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	const IntVect& t = bx.type();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	    nodal[i] = t[i];
	}
    }

    void amrex_fi_mfiter_fabbox (MFIter* mfi, int lo[3], int hi[3], int nodal[3])
    {
	const Box& bx = mfi->fabbox();
	const int* lov = bx.loVect();
	const int* hiv = bx.hiVect();
	const IntVect& t = bx.type();
	for (int i = 0; i < BL_SPACEDIM; ++i) {
	    lo[i] = lov[i];
	    hi[i] = hiv[i];
	    nodal[i] = t[i];
	}
    }
}

