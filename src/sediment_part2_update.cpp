/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2024 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
Authors: Hans Bihs, Alexander Hanke
--------------------------------------------------------------------*/

#include"sediment_part2.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"
#include"vrans.h"
#include"sediment_fdm.h"

void sediment_part2::update_cfd(lexer *p, fdm *a, ghostcell *pgc, ioflow *pflow, reinitopo *ptopo)
{
    ALOOP
	if(a->topo(i,j,k)<0.0)
	{
	por(i,j,k)= p->S24; //porosity
	d50(i,j,k) = p->S20;  //d50
    
	}
    
    
    
    pvrans->sedpart_update(p,a,pgc,por,d50);
    
}