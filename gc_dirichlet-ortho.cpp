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

#include"lexer.h"
#include"ghostcell.h"
#include"field.h"
#include"math.h"
#include"density_f.h"

void ghostcell::dirichlet_ortho(lexer *p,field& f,double dist,int gcv, int bc, int cs)
{
	int sfc=0;
	
	wallvalue=0.0;
	weight=1.0;
	margin=3;
	
	ys=1;
    if(dist>deltax*(1.0-1.0e-6) && dist<deltax*(1.0+1.0e-6))
    ys=0;

	
//fill pos[]
	for(m=0;m<=orderdir-3;m++)
	pos[m]=-deltax*double(orderdir+sfc-m-2);

	pos[orderdir-2]=0.0-double(sfc)*p->dx;
	pos[orderdir-1]=dist;

	for(m=0;m<margin;m++)
	x[m]=deltax*double(m+2-ys-sfc);

//fill y[]
	if(cs==1 )
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i+orderdir+sfc-m-2,j,k);

	if(cs==2)
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i,j-orderdir-sfc+m+2,k);

	if(cs==3)
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i,j+orderdir+sfc-m-2,k);

	if(cs==4)
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i-orderdir-sfc+m+2,j,k);

	if(cs==5)
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i,j,k+orderdir+sfc-m-2);

	if(cs==6)
	for(m=0;m<=orderdir-2;m++)
	y[m]=f(i,j,k-orderdir-sfc+m+2);

	y[orderdir-1]=wallvalue;

	if(ys==1 && dist<gamma*deltax)
    {
    imagepoint(f,x_ip,val_ip,dist,cs);

    pos[orderdir-2] = x_ip;
    y[orderdir-2] = val_ip;
    }
		

	for(q=0; q<margin; ++q)
	{
	    y[orderdir+q]=0.0;

		for(m=0;m<orderdir;m++)
		{
			weight=1.0;
			for(n=0;n<orderdir;++n)
			{
			if(m!=n)
			weight*=(x[q]-pos[n])/(pos[m]-pos[n]+1.0e-20);
			}
		y[orderdir+q]+=weight*y[m];
		}
	}

// write extrapolated ghost cell values into f()

	if(ys>=0)
    {
	if(cs==1)
	for(q=0;q<margin;++q)
	f(i-q-1+sfc,j,k) = y[orderdir+q-1+ys];// - p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,-1,0,0));

	if(cs==2)
	for(q=0;q<margin;++q)
	f(i,j+q+1-sfc,k) = y[orderdir+q-1+ys];// + p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,0,2,0));

	if(cs==3)
	for(q=0;q<margin;++q)
	f(i,j-q-1+sfc,k) = y[orderdir+q-1+ys];// - p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,0,-1,0));

	if(cs==4)
	for(q=0;q<margin;++q)
	f(i+q+1-sfc,j,k) = y[orderdir+q-1+ys];// + p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,2,0,0));

	if(cs==5)
	for(q=0;q<margin;++q)
	f(i,j,k-q-1+sfc) = y[orderdir+q-1+ys];// - p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,0,0,-1));

	if(cs==6)
	for(q=0;q<margin;++q)
	f(i,j,k+q+1-sfc) = y[orderdir+q-1+ys];// + p->dt*(double(q+1)*deltax*dP[orderpress+q-1+ys])/(pdens->roface(p,a,0,0,2));
	}

}

