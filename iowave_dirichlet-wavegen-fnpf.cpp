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

#include"iowave.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"


void iowave::dirichlet_wavegen_fnpf(lexer *p, ghostcell* pgc, double *Fi, slice &Fifsf, slice &eta)
{
    count=0;
    for(n=0;n<p->gcslin_count;n++)
    {
        i=p->gcslin[n][0];
        j=p->gcslin[n][1];
        
        Fifsf(i-1,j) = Fifsf(i,j) - Fifsfval[count]*p->DXP[IM1];
        Fifsf(i-2,j) = Fifsf(i,j) - Fifsfval[count]*2.0*p->DXP[IM1];
        Fifsf(i-3,j) = Fifsf(i,j) - Fifsfval[count]*3.0*p->DXP[IM1];
        
        eta(i-1,j) = etaval[count];
        eta(i-2,j) = etaval[count];
        eta(i-3,j) = etaval[count];
        
        ++count;
    }
    
    
    count=0;
    for(n=0;n<p->gcslin_count;n++)
    {
        i=p->gcslin[n][0];
        j=p->gcslin[n][1];
        
        FKLOOP
        {
        Fi[FIm1JK] = Fi[FIJK] - Fival[count]*p->DXP[IM1];
        
        ++count;
        }
    }
}