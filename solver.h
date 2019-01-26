/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

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
--------------------------------------------------------------------*/

class lexer;
class fdm;
class fdm_fnpf;
class ghostcell;
class field;
class vec;
class matrix_diag;
class cpt;

using namespace std;

#ifndef SOLVER_H_
#define SOLVER_H_

class solver
{

public:

	virtual void start(lexer*,fdm*, ghostcell*, field&, vec&, vec&, int, int, double)=0;
    virtual void startF(lexer*, fdm_fnpf*, ghostcell*, double*, vec&, matrix_diag&, int, int, double)=0;
	virtual void solve(lexer*,fdm*, ghostcell*, vec&, vec&, int, int, int&, int, double, cpt&)=0;
	virtual void setup(lexer*,fdm*, ghostcell*,int,cpt&)=0;
	virtual void fillxvec1(lexer*,fdm*,field&)=0;
    virtual void fillxvec2(lexer*,fdm*,field&)=0;
    virtual void fillxvec3(lexer*,fdm*,field&)=0;
    virtual void fillxvec4(lexer*,fdm*,field&)=0;
	
};

#endif
