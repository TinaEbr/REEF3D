/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

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
--------------------------------------------------------------------*/

#include"pjm_nse.h"
#include"lexer.h"
#include"fdm.h" 
#include"ghostcell.h"
#include"poisson.h"
#include"solver.h"
#include"momentum.h"
#include"ioflow.h"
#include"heat.h"
#include"concentration.h"
#include"density_f.h"
#include"density_comp.h"
#include"density_conc.h"
#include"density_heat.h"
#include"denisty_rheology.h"
#include"density_vof.h"
 
pjm_nse::pjm_nse(lexer* p, fdm *a, heat *&pheat, concentration *&pconc) 
{
    if(p->F30>0 && p->H10==0 && p->W30==0 && p->W90==0)
	pd = new density_f(p);
	
	if(p->F30>0 && p->H10==0 && p->W30==1 && p->W90==0)
	pd = new density_comp(p);
	
	if(p->F30>0 && p->H10>0 && p->W90==0)
	pd = new density_heat(pheat);
	
	if(p->F30>0 && p->C10>0 && p->W90==0)
	pd = new density_conc(p,conc);
	
	if(p->F30>0 && p->H10==0 && p->W30==0 && p->W90>0)
	pd = new density_rheology(p);
    
    if(p->F80>0 && p->H10==0 && p->W30==0 && p->W90==0)
	pd = new density_vof(p);
    
    
    if(p->B76==0)
    gcval_press=40;  

    if(p->B76==1)
    gcval_press=41;

    if(p->B76==2)
    gcval_press=42;

    if(p->B76==3)
    gcval_press=43;
	
	if(p->B76==4) 
    gcval_press=44;
	
	if(p->B76==5) 
    gcval_press=45;
	
	gcval_u=7;
	gcval_v=8;
	gcval_w=9;
}

pjm_nse::~pjm_nse()
{
}

void pjm_nse::start(fdm* a,lexer*p, poisson* ppois,solver* psolv, ghostcell* pgc, momentum *pmom, ioflow *pflow, field& uvel, field& vvel, field& wvel, double alpha)
{
    if(p->mpirank==0 && (p->count%p->P12==0))
    cout<<".";
			
	vel_setup(p,a,pgc,uvel,vvel,wvel,alpha);	
    rhs(p,a,pgc,uvel,vvel,wvel,alpha);
	
    
    pgc->start4(p,a->press,gcval_press);
    
    ppois->start(p,a,a->press);
	
        starttime=pgc->timer();

    psolv->start(p,a,pgc,a->press,a->xvec,a->rhsvec,5,gcval_press,p->N44);
	
        endtime=pgc->timer();

	pgc->start4(p,a->press,gcval_press);
	
	ucorr(a,p,uvel,alpha);
	vcorr(a,p,vvel,alpha);
	wcorr(a,p,wvel,alpha);

    p->poissoniter=p->solveriter;

	p->poissontime=endtime-starttime;

	if(p->mpirank==0 && innercounter==p->N50-1 && (p->count%p->P12==0))
	cout<<"piter: "<<p->solveriter<<"  ptime: "<<setprecision(3)<<p->poissontime<<endl;
}

void pjm_nse::ucorr(fdm* a, lexer* p, field& uvel,double alpha)
{	
	ULOOP
	uvel(i,j,k) -= alpha*p->dt*CPOR1*PORVAL1*((a->press(i+1,j,k)-a->press(i,j,k))
	/(p->DXP[IP]*p->W1));
}

void pjm_nse::vcorr(fdm* a, lexer* p, field& vvel,double alpha)
{	
	VLOOP
	vvel(i,j,k) -= alpha*p->dt*CPOR2*PORVAL2*((a->press(i,j+1,k)-a->press(i,j,k))
	/(p->DYP[JP]*p->W1));
}

void pjm_nse::wcorr(fdm* a, lexer* p, field& wvel,double alpha)
{	
	WLOOP
	wvel(i,j,k) -= alpha*p->dt*CPOR3*PORVAL3*((a->press(i,j,k+1)-a->press(i,j,k))
	/(p->DZP[KP]*p->W1));
}
 
void pjm_nse::rhs(lexer *p, fdm* a, ghostcell *pgc, field &u, field &v, field &w,double alpha)
{
	double uvel,vvel,wvel;
	
    count=0;
    FLUIDLOOP
    {
	a->rhsvec.V[count]=0.0;
    ++count;
    }
	
    pip=p->Y50;
    
    count=0;
    FLUIDLOOP
    {
    a->rhsvec.V[count] =  -(u(i,j,k)-u(i-1,j,k))/(alpha*p->dt*p->DXN[IP])
						   -(v(i,j,k)-v(i,j-1,k))/(alpha*p->dt*p->DYN[JP])
						   -(w(i,j,k)-w(i,j,k-1))/(alpha*p->dt*p->DZN[KP]);
                           
    ++count;
    }
    
    pip=0;
}
 
void pjm_nse::vel_setup(lexer *p, fdm* a, ghostcell *pgc, field &u, field &v, field &w,double alpha)
{
	pgc->start1(p,u,gcval_u);
	pgc->start2(p,v,gcval_v);
	pgc->start3(p,w,gcval_w);
	
	u.ggcpol(p);
	v.ggcpol(p);
	w.ggcpol(p);
}

void pjm_nse::pressure_norm(lexer*p, fdm* a, ghostcell* pgc)
{
    double sum=0.0;

    LOOP
    sum+=a->press(i,j,k);

    sum=pgc->globalsum(sum);

    sum/=double(p->cellnumtot);

    LOOP
    a->press(i,j,k)-=sum;
}

void pjm_nse::upgrad(lexer*p,fdm* a)
{
    //dp/dx = g*d(eta-z)/dx
    
    if(p->D38==1)
    ULOOP
    {
	a->F(i,j,k)-=PORVAL1*fabs(p->W22)*(a->eta(i+1,j)-a->eta(i,j))/p->DXP[IP];
    }
    
    if(p->D38==2)
    ULOOP
    {
	a->F(i,j,k)-=PORVAL1*(a->eta(i+1,j)-a->eta(i,j))/p->DXP[IP];
    }
}

void pjm_nse::vpgrad(lexer*p,fdm* a)
{
    if(p->D38==1)
    VLOOP
    {
	a->G(i,j,k)-=PORVAL2*fabs(p->W22)*(a->eta(i,j+1)-a->eta(i,j))/p->DYP[JP];
    }
    
    if(p->D38==2)
    VLOOP
    {
	a->G(i,j,k)-=PORVAL2*(a->eta(i,j+1)-a->eta(i,j))/p->DYP[JP];
    }
}

void pjm_nse::wpgrad(lexer*p,fdm* a)
{
    double z1,z2;
    
    if(p->D38==1)
    WLOOP
    {
    z1 = p->ZP[KP];
    z2 = p->ZP[KP1];
	a->H(i,j,k)-=PORVAL3*fabs(p->W22)*(-z2+z1)/p->DZP[KP];
    }
    
    if(p->D38==2)
    WLOOP
    {
    z1 = p->ZP[KP];
    z2 = p->ZP[KP1];
	a->H(i,j,k)-=PORVAL3*(-z2+z1)/p->DZP[KP];
    }
}

void pjm_nse::fillapu(lexer*p,fdm* a)
{
}

void pjm_nse::fillapv(lexer*p,fdm* a)
{
}

void pjm_nse::fillapw(lexer*p,fdm* a)
{
}

void pjm_nse::ptimesave(lexer *p, fdm *a, ghostcell *pgc)
{
}






