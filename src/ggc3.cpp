/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2022 Hans Bihs

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

#include"mgc3.h"
#include"cart3.h"
#include"lexer.h"

void mgc3::make_ggc(lexer* p)
{
	cart3::ggcsize=1;
	p->Iarray(cart3::ggc,cart3::ggcsize,3);
}

void mgc3::fill_ggc(lexer* p)
{
	int q,qq,n,nn,a;
	int check;
	
	p->Iarray(cart3::ggcmem,kmax*jmax*imax);

//--------------------------
//WALL1

	GC3LOOP
	{
	    i=p->gcb3[n][0];
		j=p->gcb3[n][1];
		k=p->gcb3[n][2];

		if(p->gcb3[n][3]==1)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i-q-1-p->imin)*p->jmax*p->kmax + (j-p->jmin)*p->kmax + k-p->kmin]<0)
        cart3::ggcmem[(i-imin-q-1)*jmax*kmax + (j-jmin)*kmax + k-kmin]+=1;

		if(p->gcb3[n][3]==4)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i+q+1-p->imin)*p->jmax*p->kmax + (j-p->jmin)*p->kmax + k-p->kmin]<0)
		cart3::ggcmem[(i-imin+q+1)*jmax*kmax + (j-jmin)*kmax + k-kmin]+=1;

		if(p->gcb3[n][3]==3)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i-p->imin)*p->jmax*p->kmax + (j-p->jmin-q-1)*p->kmax + k-p->kmin]<0)
        cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin-q-1)*kmax + k-kmin]+=1;

		if(p->gcb3[n][3]==2)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i-p->imin)*p->jmax*p->kmax + (j-p->jmin+q+1)*p->kmax + k-p->kmin]<0)
		cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin+q+1)*kmax + k-kmin]+=1;

		if(p->gcb3[n][3]==5)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i-p->imin)*p->jmax*p->kmax + (j-p->jmin)*p->kmax + k-p->kmin-q-1]<0)
		cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin-q-1]+=1;

		if(p->gcb3[n][3]==6)
		for(q=0;q<p->margin;++q)
        if(p->flag3[(i-p->imin)*p->jmax*p->kmax + (j-p->jmin)*p->kmax + k-p->kmin+q+1]<0)
		cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin+q+1]+=1;
	}

// count entries
	cart3::ggccount=0;
	a=0;
	for(i=0;i<imax;++i)
	for(j=0;j<jmax;++j)
	for(k=0;k<kmax;++k)
	{
        if(cart3::ggcmem[a]>1)
        {
        ++cart3::ggccount;
        }

	++a;
	}
	
	p->Iresize(cart3::ggc,cart3::ggcsize,cart3::ggccount*p->margin, 3, 3);
	cart3::ggcsize=cart3::ggccount*p->margin;

//--------------------------
//WALL2
	n=0;
	QQGC3LOOP
	{
        i=p->gcb3[qq][0];
		j=p->gcb3[qq][1];
		k=p->gcb3[qq][2];

		if(p->gcb3[qq][3]==1)
		for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin-q-1)*jmax*kmax + (j-jmin)*kmax + k-kmin]>1)
        {
            if(cart3::ggcmem[(i-imin-q-1)*jmax*kmax + (j-jmin)*kmax + k-kmin]<10)
            {
            cart3::ggcmem[(i-imin-q-1)*jmax*kmax + (j-jmin)*kmax + k-kmin]=n+10;
			cart3::ggc[n][0]=i-q-1;
			cart3::ggc[n][1]=j;
			cart3::ggc[n][2]=k;
			++n;
            }
        }

        if(p->gcb3[qq][3]==4)
        for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin+q+1)*jmax*kmax + (j-jmin)*kmax + k-kmin]>1)
        {
            if(cart3::ggcmem[(i-imin+q+1)*jmax*kmax + (j-jmin)*kmax + k-kmin]<10)
            {
            cart3::ggcmem[(i-imin+q+1)*jmax*kmax + (j-jmin)*kmax + k-kmin]=n+10;
			cart3::ggc[n][0]=i+q+1;
			cart3::ggc[n][1]=j;
			cart3::ggc[n][2]=k;
			++n;
            }
        }

        if(p->gcb3[qq][3]==3)
        for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin-q-1)*kmax + k-kmin]>1)
        {
            if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin-q-1)*kmax + k-kmin]<10)
            {
            cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin-q-1)*kmax + k-kmin]=n+10;
			cart3::ggc[n][0]=i;
			cart3::ggc[n][1]=j-q-1;
			cart3::ggc[n][2]=k;
			++n;
            }
        }

		if(p->gcb3[qq][3]==2)
		for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin+q+1)*kmax + k-kmin]>1)
        {
            if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin+q+1)*kmax + k-kmin]<10)
            {
            cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin+q+1)*kmax + k-kmin]=n+10;
			cart3::ggc[n][0]=i;
			cart3::ggc[n][1]=j+q+1;
			cart3::ggc[n][2]=k;
			++n;
            }
        }

        if(p->gcb3[qq][3]==5)
        for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin-q-1]>1)
        {
            if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin-q-1]<10)
            {
            cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin-q-1]=n+10;
			cart3::ggc[n][0]=i;
			cart3::ggc[n][1]=j;
			cart3::ggc[n][2]=k-q-1;
			++n;
            }
        }

        if(p->gcb3[qq][3]==6)
        for(q=0;q<p->margin;++q)
		if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin+q+1]>1)
        {
            if(cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin+q+1]<10)
            {
            cart3::ggcmem[(i-imin)*jmax*kmax + (j-jmin)*kmax + k-kmin+q+1]=n+10;
			cart3::ggc[n][0]=i;
			cart3::ggc[n][1]=j;
			cart3::ggc[n][2]=k+q+1;
			++n;
            }

        }
	}
	cart3::ggccount=n;
	
	p->del_Iarray(cart3::ggcmem,kmax*jmax*imax);
}

