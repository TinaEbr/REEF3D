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

#include"ioflow_f.h"
#include"lexer.h"
#include"fdm.h"

void ioflow_f::pressure_io(lexer *p, fdm* a, ghostcell *pgc)
{
    
    if(p->B76==1)
    {
    pressure_inlet(p,a,pgc);
    }
	
	if(p->B76==2)
    {
    pressure_outlet(p,a,pgc);
    }

    if(p->B76==3)
    {
    pressure_inlet(p,a,pgc);
    pressure_outlet(p,a,pgc);
    }

    if(p->B76==4)
    {
    pressure_bed(p,a,pgc);
    }

    if(p->B76==5)
    {
    pressure_inlet(p,a,pgc);
    pressure_outlet(p,a,pgc);
    pressure_bed(p,a,pgc);
    }
}

void ioflow_f::pressure_outlet(lexer *p, fdm *a, ghostcell *pgc)
{
    double pval=0.0;

        for(n=0;n<p->gcout_count;++n)
        {
        i=p->gcout[n][0];
        j=p->gcout[n][1];
        k=p->gcout[n][2];
		pval=0.0;
		
			if(p->B77==1)
			{
			pval=(p->phiout - p->pos_z())*a->ro(i,j,k)*fabs(p->W22)*p->B79;
			
			a->press(i+1,j,k)=pval;
			a->press(i+2,j,k)=pval;
			a->press(i+3,j,k)=pval;
			}
		
			if(p->B77==2)
			{
			pval=0.0;

			a->press(i+1,j,k)=pval;
			a->press(i+2,j,k)=pval;
			a->press(i+3,j,k)=pval;
			}
			
			if(p->B77==3)
			if(p->pos_z() <= p->B80)
			{
			pval=(p->phiout - p->pos_z())*a->ro(i,j,k)*fabs(p->W22)*p->B79;
			
			a->press(i+1,j,k)=pval;
			a->press(i+2,j,k)=pval;
			a->press(i+3,j,k)=pval;
			}
			

			if(p->B77==4)
			{
			if(a->phi(i,j,k)>-0.6*p->dx && p->pos_z() > p->B80)
			pval=0.0;
			
			if(a->phi(i,j,k)>-0.6*p->dx && p->pos_z() <= p->B80)
			pval=(p->phiout - p->pos_z())*a->ro(i,j,k)*fabs(p->W22)*p->B79;
			
			if(a->phi(i,j,k)<=-0.6*p->dx)
			pval=a->press(i,j,k);

			a->press(i+1,j,k)=pval;
			a->press(i+2,j,k)=pval;
			a->press(i+3,j,k)=pval;
			}
			
			if(p->B77==5)
			{
			if(a->phi(i,j,k)>-0.6*p->dx)
			pval=0.0;
			
			if(a->phi(i,j,k)<=-0.6*p->dx)
			pval=a->press(i,j,k);

			a->press(i+1,j,k)=pval;
			a->press(i+2,j,k)=pval;
			a->press(i+3,j,k)=pval;
			}
        }
}

void ioflow_f::pressure_inlet(lexer *p, fdm *a, ghostcell *pgc)
{
    double pval=0.0;

    for(n=0;n<p->gcin_count;n++)
    {
    i=p->gcin[n][0];
    j=p->gcin[n][1];
    k=p->gcin[n][2];
		
		if(a->phi(i,j,k)>=0.0)
        pval=(p->phimean - p->pos_z())*a->ro(i,j,k)*fabs(p->W22);
		
		if(a->phi(i,j,k)<0.0)
        pval = a->press(i,j,k);

        a->press(i-1,j,k)=pval;
        a->press(i-2,j,k)=pval;
        a->press(i-3,j,k)=pval;
    }
}

void ioflow_f::pressure_wall(lexer *p, fdm *a, ghostcell *pgc)
{
    double pval=0.0;

    GC4LOOP
    if(p->gcb4[n][3] != 5 && p->gcb4[n][3] != 6 && (p->gcb4[n][4] ==3 || p->gcb4[n][4] ==21 || p->gcb4[n][4] ==22))
    {
    i=p->gcb4[n][0];
    j=p->gcb4[n][1];
    k=p->gcb4[n][2];


        if(a->phi(i,j,k)>0.0 || p->I56==0)
        {
        pval=a->phi(i,j,k)*a->ro(i,j,k)*fabs(p->W22);

            if(p->gcb4[n][3]==1)
            {
            a->press(i-1,j,k)=pval;
            a->press(i-2,j,k)=pval;
            a->press(i-3,j,k)=pval;
            }

            if(p->gcb4[n][3]==2)
            {
            a->press(i,j+1,k)=pval;
            a->press(i,j+2,k)=pval;
            a->press(i,j+3,k)=pval;
            }

            if(p->gcb4[n][3]==3)
            {
            a->press(i,j-1,k)=pval;
            a->press(i,j-2,k)=pval;
            a->press(i,j-3,k)=pval;
            }

            if(p->gcb4[n][3]==4)
            {
            a->press(i+1,j,k)=pval;
            a->press(i+2,j,k)=pval;
            a->press(i+3,j,k)=pval;
            }
        }
    }
}

void ioflow_f::pressure_bed(lexer *p, fdm *a, ghostcell *pgc)
{
    double pval=0.0;

    GC4LOOP
    if(p->gcb4[n][3] == 5 )
    {
    i=p->gcb4[n][0];
    j=p->gcb4[n][1];
    k=p->gcb4[n][2];


        if(a->phi(i,j,k)>0.0)
        {
        pval=a->phi(i,j,k)*a->ro(i,j,k)*fabs(p->W22);

        a->press(i,j,k-1)=pval;
        a->press(i,j,k-2)=pval;
        a->press(i,j,k-3)=pval;
        }
    }
}

double ioflow_f::local_fsf(lexer *p, fdm *a, ghostcell *pgc)
{
    double wsf=-1.0e20;
    int count=0;

        KLOOP
        PCHECK
        {
            if(a->phi(i,j,k)>=0.0 && a->phi(i,j,k+1)<0.0)
            wsf=MAX(wsf,-(a->phi(i,j,k)*p->dx)/(a->phi(i,j,k+1)-a->phi(i,j,k)) + p->pos_z());

            if(a->phi(i,j,k)>0.0)
            ++count;
        }

    //wsf=pgc->globalmax(wsf);

    if(count==0)
    wsf=0.0;

    return wsf;
}
