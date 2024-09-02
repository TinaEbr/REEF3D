/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2024 Hans Bihs

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

#include"initialize.h"
#include"density_vof.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"

density_vof::density_vof(lexer* p) : epsi(p->F45*p->DXM), eps(2.1*p->DXM)
{
    /*if(p->j_dir==0)        
    psi = p->F45*(1.0/2.0)*(p->DRM+p->DTM);
        
    if(p->j_dir==1)
    psi = p->F45*(1.0/3.0)*(p->DRM+p->DSM+p->DTM);
    
    H=0.0;*/
    
    double psim;
    int count;
    
    if(p->j_dir==0)        
    p->psi = p->F45*(1.0/2.0)*(p->DRM+p->DTM);
        
    if(p->j_dir==1)
    p->psi = p->F45*(1.0/3.0)*(p->DRM+p->DSM+p->DTM);
    
    
   /* if(p->B90>0 || p->B60>0)
    {
    // psi
        count=0;
        psim=0.0;
        LOOP
        if(fabs(a->phi(i,j,k))<5.0*p->DTM)
        {
        psim += p->DZN[KP];
        ++count;
        }
        
        count=pgc->globalisum(count);
        psim=pgc->globalsum(psim);
        
        p->psi = p->F45*psim/double(count);

    }*/
    
    p->psi0=p->psi;
}

density_vof::~density_vof()
{
}

double density_vof::roface(lexer *p, fdm *a, int aa, int bb, int cc)
{  
//double phival = 0.5*(a->phi(i,j,k) + a->phi(i+aa,j+bb,k+cc));

  /*  if(phival>=0.0)
        H=1.0;

    if(phival<-2.0*p->psi)
        H=0.0;

    if(phival>=-2.0*p->psi && phival<0.0)
        H=0.5*(1.0 + (phival+p->psi)/p->psi + (1.0/PI)*sin((PI*(phival+p->psi))/p->psi));
        
    roval = p->W1*H + p->W3*(1.0-H);*/
 //   double H=0.5*(a->vof(i,j,k) + a->vof(i+aa,j+bb,k+cc));
 //  double  roval = p->W1*H + p->W3*(1.0-H);
/*	H= 0.5*(a->phi(i,j,k) + a->phi(i+aa,j+bb,k+cc));

		H=MAX(H,0.0);
		H=MIN(H,1.0);
    roval = p->W1*H +   p->W3*(1.0-H); */
   

    double phival = 0.5*(a->phi(i,j,k) + a->phi(i+aa,j+bb,k+cc));

    if(phival>p->psi)
    H=1.0;

    if(phival<-p->psi)
    H=0.0;

    if(fabs(phival)<=p->psi)
    H=0.5*(1.0 + phival/p->psi + (1.0/PI)*sin((PI*phival)/p->psi));
    
    roval = p->W1*H + p->W3*(1.0-H);
/*
    if(a->vof(i,j,k)<0.001 && a->vof(i+aa,j+bb,k+cc)<0.001)
        roval=p->W3;
    else if(a->vof(i,j,k)>0.999 && a->vof(i+aa,j+bb,k+cc)>0.999)
        roval=p->W1;
    else if((a->vof(i,j,k)>0.999 && a->vof(i+aa,j+bb,k+cc)<0.001) || (a->vof(i,j,k)<0.001 && a->vof(i+aa,j+bb,k+cc)>0.999))
        roval=p->W1;
    else if((a->vof(i,j,k)>0.999 && (a->vof(i+aa,j+bb,k+cc)<=0.999 && a->vof(i+aa,j+bb,k+cc)>=0.001)) || ((a->vof(i,j,k)<=0.999 && a->vof(i,j,k)>=0.001) && a->vof(i+aa,j+bb,k+cc)>0.999))
        roval=p->W1;
    else if((a->vof(i,j,k)<0.001 && (a->vof(i+aa,j+bb,k+cc)<=0.999 && a->vof(i+aa,j+bb,k+cc)>=0.001)) || ((a->vof(i,j,k)<=0.999 && a->vof(i,j,k)>=0.001) && a->vof(i+aa,j+bb,k+cc)<0.001))
        roval=p->W1;
    else if((a->vof(i,j,k)<=0.999 && a->vof(i,j,k)>=0.001) && (a->vof(i+aa,j+bb,k+cc)<=0.999 && a->vof(i+aa,j+bb,k+cc)>=0.001))
        roval=0.5*(a->vof(i,j,k)+a->vof(i+aa,j+bb,k+cc))*p->W1+(1.0-0.5*(a->vof(i,j,k)+a->vof(i+aa,j+bb,k+cc)))*p->W3;
    else
        cout<<"density case mising"<<endl;*/
	return roval;
	
}




