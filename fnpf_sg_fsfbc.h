/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

This file is part of REEF3D.

REEF3D is fra->eps software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Fra->eps Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. Sa->eps the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, sa->eps <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"fnpf_sg_fsf.h"
#include"slice4.h"

class fnpf_sg_laplace;
class field;
class fnpf_convection;
class fnpf_ddx;
class fnpf_etadisc;
class solver2D;

using namespace std;

#ifndef FNPF_SG_FSFBC_H_
#define FNPF_SG_FSFBC_H_

class fnpf_sg_fsfbc : public fnpf_sg_fsf, public increment 
{
public:
	fnpf_sg_fsfbc(lexer*, fdm_fnpf*, ghostcell*);
	virtual ~fnpf_sg_fsfbc();
    
    virtual void fsfdisc(lexer*,fdm_fnpf*,ghostcell*,slice&,slice&);
    virtual void fsfdisc_ini(lexer*,fdm_fnpf*,ghostcell*,slice&,slice&);
    virtual void kfsfbc(lexer*,fdm_fnpf*,ghostcell*);
    virtual void dfsfbc(lexer*,fdm_fnpf*,ghostcell*,slice&);
    virtual void fsfwvel(lexer*,fdm_fnpf*,ghostcell*,slice&,slice&);
    virtual void wetdry(lexer*,fdm_fnpf*,ghostcell*,slice&,slice&);
    virtual void breaking(lexer*,fdm_fnpf*,ghostcell*,slice&,slice&,slice&,double);
    virtual void coastline(lexer*,fdm_fnpf*,ghostcell*,slice&);
    virtual void damping(lexer*,fdm_fnpf*,ghostcell*,slice&,int,double);
    
    
    void filter(lexer*, fdm_fnpf*,ghostcell*, slice&);

    fnpf_convection *pconvec;
    fnpf_convection *pconeta;
    fnpf_etadisc *pdf;
    fnpf_convection *pdx;
    fnpf_ddx *pddx;
    solver2D *psolv;
    
    
private:
    double ivel,jvel,kvel;
    
    double visc;


    slice4 Fxx;
    
};

#endif
