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
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"6DOF_external_motion.h"
#include <Eigen/Dense>

class lexer;
class fdm;
class fdm_nhf;
class fdm2D;
class ghostcell;
class vrans;
class net;
class field;

using namespace std;

#ifndef SIXDOF_EXTERNAL_MOTION_FIXED_H_
#define SIXDOF_EXTERNAL_MOTION_FIXED_H_

class sixdof_external_motion_fixed : public sixdof_external_motion
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    
    virtual void external_motion_trans(lexer*, ghostcell*, Eigen::Vector3d&, Eigen::Vector3d&);
    virtual void external_motion_rot(lexer*, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector4d&, Eigen::Matrix<double, 3, 4>&,  Eigen::Matrix3d&);

    virtual void ini(lexer*,ghostcell*);
    
private:
    double ramp_vel(lexer*);
    double ramp_draft(lexer*);
    
    
    double Uext, Vext, Wext, Pext, Qext, Rext;
};

#endif
