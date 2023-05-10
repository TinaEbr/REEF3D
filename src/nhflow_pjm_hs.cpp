/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2023 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"nhflow_pjm_hs.h"
#include"lexer.h"
#include"fdm_nhf.h" 
#include"ghostcell.h"
#include"nhflow_poisson.h"
#include"solver.h"
#include"ioflow.h"
#include"heat.h"
#include"concentration.h"
#include"density_f.h"
#include"fnpf_cds2.h"
#include"fnpf_cds4.h"
#include"fnpf_cds6.h"
#include"fnpf_weno3.h"
#include"fnpf_weno5.h"
#include"patchBC_interface.h"
#include"nhflow_hydrostatic_HLL.h"

#define HX (fabs(d->hx(i,j))>1.0e-20?d->hx(i,j):1.0e20)
#define HXP (fabs(0.5*(d->WL(i,j)+d->WL(i+1,j)))>1.0e-20?0.5*(d->WL(i,j)+d->WL(i+1,j)):1.0e20)
#define HY (fabs(d->hy(i,j))>1.0e-20?d->hy(i,j):1.0e20)
#define WLVL (fabs(d->WL(i,j))>1.0e-20?d->WL(i,j):1.0e20)
 
nhflow_pjm_hs::nhflow_pjm_hs(lexer* p, fdm_nhf *d, patchBC_interface *ppBC)
{
    pBC = ppBC;
    
	pd = new density_f(p);
    
    pdx = new fnpf_cds2(p);
    
    p->Darray(Fx,p->imax*p->jmax*(p->kmax+2));
    p->Darray(Fy,p->imax*p->jmax*(p->kmax+2));
    p->Darray(dFx,p->imax*p->jmax*(p->kmax+2));
    p->Darray(dFy,p->imax*p->jmax*(p->kmax+2));
    p->Darray(dFx_n,p->imax*p->jmax*(p->kmax+2));
    p->Darray(dFy_n,p->imax*p->jmax*(p->kmax+2));
    
    gcval_press=540;  

	gcval_u=7;
	gcval_v=8;
	gcval_w=9;
    
    phs = new nhflow_hydrostatic_HLL(p,pBC);
}

nhflow_pjm_hs::~nhflow_pjm_hs()
{
}

void nhflow_pjm_hs::start(lexer*p, fdm_nhf *d, solver* psolv, ghostcell* pgc, ioflow *pflow, double *U, double *V, double *W, double alpha)
{
}

void nhflow_pjm_hs::ucorr(lexer* p, fdm_nhf *d, double *U, double alpha)
{	
}

void nhflow_pjm_hs::vcorr(lexer* p, fdm_nhf *d, double *U, double alpha)
{	 
}

void nhflow_pjm_hs::wcorr(lexer* p, fdm_nhf *d, double *W, double alpha)
{
}
 
void nhflow_pjm_hs::rhs(lexer *p, fdm_nhf *d, ghostcell *pgc, double *U, double *V, double *W, double alpha)
{
}
 
void nhflow_pjm_hs::vel_setup(lexer *p, fdm_nhf *d, ghostcell *pgc, double *U, double *V, double *W, double alpha)
{
}

void nhflow_pjm_hs::upgrad(lexer*p, fdm_nhf *d, slice &eta, slice &eta_n)
{
    if(p->D38==1 && p->A540==1)
    LOOP
    if(p->wet[IJ]==1)
    d->F[IJK] -= PORVALNH*fabs(p->W22)*
                (p->A223*eta(i+1,j) + (1.0-p->A223)*eta_n(i+1,j) - p->A223*eta(i-1,j) - (1.0-p->A223)*eta_n(i-1,j))/(p->DXP[IP]+p->DXP[IM1]);

    if(p->D38==1 && p->A540==2)
    LOOP
    if(p->wet[IJ]==1)
	d->F[IJK] -= PORVALNH*fabs(p->W22)*(d->eta(i+1,j) - d->eta(i,j))/p->DXP[IP];
    
    
    if(p->D38==2 && p->A540==1)
    LOOP
    if(p->wet[IJ]==1)
    d->F[IJK] -= PORVALNH*(1.0/WLVL)*(p->A223*dFx[IJK] + (1.0-p->A223)*dFx_n[IJK]) 
               - PORVALNH*fabs(p->W22)*(1.0/WLVL)*eta(i,j)*(d->depth(i+1,j) - d->depth(i-1,j))/(p->DXP[IP]+p->DXP[IM1]);
               
}

void nhflow_pjm_hs::vpgrad(lexer*p, fdm_nhf *d, slice &eta, slice &eta_n)
{
    if(p->D38==1 && p->A540==1)
    LOOP
    if(p->wet[IJ]==1)
	d->G[IJK] -= PORVALNH*fabs(p->W22)*
                 (p->A223*eta(i,j+1) + (1.0-p->A223)*eta_n(i,j+1) - p->A223*eta(i,j-1) - (1.0-p->A223)*eta_n(i,j-1))/(p->DYP[JP]+p->DYP[JM1]);
    
    if(p->D38==1 && p->A540==2)
    LOOP
    if(p->wet[IJ]==1)
	d->G[IJK] -= PORVALNH*fabs(p->W22)*(d->eta(i,j+1) - d->eta(i,j))/p->DYP[JP];
}

void nhflow_pjm_hs::wpgrad(lexer*p, fdm_nhf *d, slice &eta, slice &eta_n)
{
}


void nhflow_pjm_hs::hydrostatic_HLL(lexer*p, fdm_nhf *d, ghostcell* pgc, slice &eta, double *U, double *V)
{
    phs->face_flux_3D(p,pgc,d,eta,U,V,Fx,Fy);
    
    ULOOP
    dFx_n[IJK] = dFx[IJK];
    
    VLOOP
    dFy_n[IJK] = dFy[IJK];
    
    LOOP
    dFx[IJK] = (Fx[IJK] - Fx[Im1JK])/p->DXN[IP];
    
    LOOP
    dFy[IJK] = (Fy[IJK] - Fy[IJm1K])/p->DYN[JP];
    
    
    
    
}
