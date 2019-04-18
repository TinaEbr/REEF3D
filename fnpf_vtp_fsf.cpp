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

#include"fnpf_vtp_fsf.h"
#include"lexer.h"
#include"fdm_fnpf.h"
#include"ghostcell.h"
#include<sys/stat.h>
#include<sys/types.h>

fnpf_vtp_fsf::fnpf_vtp_fsf(lexer *p, fdm_fnpf *c, ghostcell *pgc)
{
	if(p->I40==0)
    {
	p->printtime=0.0;
    }
	
	p->printcount=0;
	
	// Create Folder
	if(p->mpirank==0 && p->P14==1)
	mkdir("./REEF3D_FNPF_VTP_FSF",0777);
	
	
}

fnpf_vtp_fsf::~fnpf_vtp_fsf()
{
}

void fnpf_vtp_fsf::start(lexer *p, fdm_fnpf *c, ghostcell* pgc, ioflow *pflow)
{	
	// Print out based on iteration
    if((p->count%p->P20==0 && p->P30<0.0 && p->P34<0.0 && p->P10==1 && p->P20>0)  || (p->count==0 &&  p->P30<0.0))
    {
    print2D(p,c,pgc);
    }
		
    // Print out based on time
    if((p->simtime>p->printtime && p->P30>0.0 && p->P34<0.0 && p->P10==1) || (p->count==0 &&  p->P30>0.0))
    {
    print2D(p,c,pgc);
		
    p->printtime+=p->P30;
    }
	
}

void fnpf_vtp_fsf::print2D(lexer *p, fdm_fnpf *c, ghostcell* pgc)
{	
    c->eta.ggcpol(p);
    
	if(p->mpirank==0)
    pvtu(p,c,pgc);
    
	name_iter(p,c,pgc);
	
	
	// Open File
	ofstream result;
	result.open(name, ios::binary);
    
    // offsets
    n=0;
	offset[n]=0;
	++n;
	
	// Points
    offset[n]=offset[n-1]+4*(p->pointnum2D)*3+4;
    ++n;
	
	// velocity
	offset[n]=offset[n-1]+4*(p->pointnum2D)*3+4;
	++n;
    
    // elevation
	offset[n]=offset[n-1]+4*(p->pointnum2D)+4;
	++n;
	
	// depth
	offset[n]=offset[n-1]+4*(p->pointnum2D)+4;
	++n;
    
    // breaking
	offset[n]=offset[n-1]+4*(p->pointnum2D)+4;
	++n;
	
	// Cells
    offset[n]=offset[n-1] + 4*p->polygon_sum*3+4;
    ++n;
    offset[n]=offset[n-1] + 4*p->polygon_sum+4;
    ++n;
	offset[n]=offset[n-1] + 4*p->polygon_sum+4;
    ++n;
	
	
	result<<"<?xml version=\"1.0\"?>"<<endl;
	result<<"<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">"<<endl;
	result<<"<PolyData>"<<endl;
	result<<"<Piece NumberOfPoints=\""<<p->pointnum2D<<"\" NumberOfPolys=\""<<p->polygon_sum<<"\">"<<endl;
    
    n=0;
	result<<"<Points>"<<endl;
    result<<"<DataArray type=\"Float32\"  NumberOfComponents=\"3\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
    result<<"</Points>"<<endl;
	
	
    result<<"<PointData >"<<endl;
    result<<"<DataArray type=\"Float32\" Name=\"velocity\" NumberOfComponents=\"3\" format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
    result<<"<DataArray type=\"Float32\" Name=\"elevation\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
	result<<"<DataArray type=\"Float32\" Name=\"depth\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
    result<<"<DataArray type=\"Float32\" Name=\"breaking\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
    result<<"</PointData>"<<endl;

    

    result<<"<Polys>"<<endl;
    result<<"<DataArray type=\"Int32\"  Name=\"connectivity\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
	result<<"<DataArray type=\"Int32\"  Name=\"offsets\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
	++n;
    result<<"<DataArray type=\"Int32\"  Name=\"types\"  format=\"appended\" offset=\""<<offset[n]<<"\" />"<<endl;
    ++n;
	result<<"</Polys>"<<endl;

    result<<"</Piece>"<<endl;
    result<<"</PolyData>"<<endl;
    
    
    //----------------------------------------------------------------------------
    result<<"<AppendedData encoding=\"raw\">"<<endl<<"_";
	
	//  XYZ
	iin=4*(p->pointnum2D)*3;
	result.write((char*)&iin, sizeof (int));
    TPSLICELOOP
	{
    
	ffn=float(float(i+1)*p->dx+p->originx);
	result.write((char*)&ffn, sizeof (float));

	ffn=float(float(j+1)*p->dx+p->originy);
	result.write((char*)&ffn, sizeof (float));

	ffn=float(pgc->gcsl_ipol4eta(p,c->eta,c->bed)+p->wd);
	result.write((char*)&ffn, sizeof (float));
	}
	
    //  Velocities
    iin=4*(p->pointnum2D)*3;
	result.write((char*)&iin, sizeof (int));
    TPSLICELOOP
	{
    k = p->knoz-1;
	ffn=float(p->ipol4(c->u));
	result.write((char*)&ffn, sizeof (float));

	ffn=float(p->ipol4(c->v));
	result.write((char*)&ffn, sizeof (float));
	
	ffn=float(p->ipol4(c->w));
	result.write((char*)&ffn, sizeof (float));
	}
    
    //  Elevation
	iin=4*(p->pointnum2D);
	result.write((char*)&iin, sizeof (int));
    TPSLICELOOP
	{
	ffn=float(pgc->gcsl_ipol4(p,c->eta)+p->wd);
	result.write((char*)&ffn, sizeof (float));
	}
	
	//  Depth
	iin=4*(p->pointnum2D);
	result.write((char*)&iin, sizeof (int));
	TPSLICELOOP
	{
	ffn=float(pgc->gcsl_ipol4(p,c->depth));
	result.write((char*)&ffn, sizeof (float));
	}
    
    //  Breaking
	iin=4*(p->pointnum2D);
	result.write((char*)&iin, sizeof (int));
	TPSLICELOOP
	{
        
	ffn=float(pgc->gcsl_ipol4(p,c->breaking_print));
	result.write((char*)&ffn, sizeof (float));
	}

    //  Connectivity
    iin=4*(p->polygon_sum)*3;
    result.write((char*)&iin, sizeof (int));
    SLICEBASELOOP
	{
	// Triangle 1
	iin=int(c->nodeval2D(i-1,j-1))-1;
	result.write((char*)&iin, sizeof (int));

	iin=int(c->nodeval2D(i,j-1))-1;
	result.write((char*)&iin, sizeof (int));

	iin=int(c->nodeval2D(i,j))-1;
	result.write((char*)&iin, sizeof (int));
	
	
	// Triangle 2
	iin=int(c->nodeval2D(i-1,j-1))-1;
	result.write((char*)&iin, sizeof (int));

	iin=int(c->nodeval2D(i,j))-1;
	result.write((char*)&iin, sizeof (int));

	iin=int(c->nodeval2D(i-1,j))-1;
	result.write((char*)&iin, sizeof (int));
	}
    
    
    //  Offset of Connectivity
    iin=4*(p->polygon_sum);
    result.write((char*)&iin, sizeof (int));
	for(n=0;n<p->polygon_sum;++n)
	{
	iin=(n+1)*3;
	result.write((char*)&iin, sizeof (int));
	}
    
//  Cell types
    iin=4*(p->polygon_sum);
    result.write((char*)&iin, sizeof (int));
	for(n=0;n<p->polygon_sum;++n)
	{
	iin=7;
	result.write((char*)&iin, sizeof (int));
	}

    result<<endl<<"</AppendedData>"<<endl;
    result<<"</VTKFile>"<<endl;

	result.close();
	
	++p->printcount;

}


