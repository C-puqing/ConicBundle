/* ****************************************************************************

    Copyright (C) 2004-2012  Christoph Helmberg

    ConicBundle, Version 0.3.11
    File:  CBsources/diagscaling.hxx

    This file is part of ConciBundle, a C/C++ library for convex optimization.

    ConicBundle is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ConicBundle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***************************************************************************** */



#ifndef CONICBUNDLE_DIAGSCALING_HXX
#define CONICBUNDLE_DIAGSCALING_HXX


/**  @file diagscaling.hxx
    @brief Header declaring the class ConicBundle::BundleDiagScaling
    @version 1.0
    @date 2009-04-23
    @author Christoph Helmberg
*/


#include "bundle.hxx"

namespace ConicBundle {

/**@defgroup InternalBundleSolver
 */

  //@{

 /** @brief implements the abstract interface ConicBundle::BundleScaling for $\|y-\hat{y}\|_H^2$ with $H=weight*D$,
     where $D$ is a diagonal matrix, giving rise to an augmented model with diagonal scaling
 */

class BundleDiagonalScaling: public BundleScaling
{
private:
  CH_Matrix_Classes::Matrix D;
  CH_Matrix_Classes::Real weightu;

public:
  BundleDiagonalScaling(const CH_Matrix_Classes::Matrix& Din):BundleScaling(),D(Din){weightu=1.;}
  BundleDiagonalScaling(CH_Matrix_Classes::Integer dim, CH_Matrix_Classes::Real d=1.):
    BundleScaling(),D(dim,1,d){weightu=1.;}
  virtual ~BundleDiagonalScaling(){}

  void set_weightu(CH_Matrix_Classes::Real in_weightu);
  CH_Matrix_Classes::Real get_weightu() const
  {return weightu;}

  const CH_Matrix_Classes::Matrix& get_D() const {return D;}
  void set_D(CH_Matrix_Classes::Matrix& in_D){D=in_D*weightu;}
  CH_Matrix_Classes::Integer dim() const {return D.dim();}
  CH_Matrix_Classes::Real operator()(CH_Matrix_Classes::Integer i) const {return D(i);}
  CH_Matrix_Classes::Real operator[](CH_Matrix_Classes::Integer i) const {return D[i];}
   
  /// returns $\|B\|^2_I$
  virtual CH_Matrix_Classes::Real norm_sqr(const CH_Matrix_Classes::Matrix& B) const;
  /// returns $\|B\|^2_{H^{-1}}$
  virtual CH_Matrix_Classes::Real dnorm_sqr(const CH_Matrix_Classes::Matrix& B) const;

  /** @brief computes initial Lagrange multipliers eta for given (aggregate) 
      subgradient subg and center y so that with this eta the next candidate newy 
      would lie within lower bounds lby and upper bounds uby on the support bounds_index 
      and eta and newy would satisfy complementary slackness (i.e., both are optimal).
      
  */
  virtual int compute_first_eta(CH_Matrix_Classes::Matrix& eta,
				const CH_Matrix_Classes::Matrix& subg,
				const CH_Matrix_Classes::Matrix& y,
				const CH_Matrix_Classes::Matrix& lby,
				const CH_Matrix_Classes::Matrix& uby,
				const CH_Matrix_Classes::Indexmatrix& bound_index,
				const CH_Matrix_Classes::Indexmatrix& yfixed) const;

  /** @brief computes Lagrange multiplier eta and next candidate newy for 
      given (aggregate) subgradient subg and center y so that newy is within 
      lower bounds lby and upper bounds uby on the support bounds_index and
      eta and newy satisfy complementary slackness (i.e., both are optimal).
      If yfixed.dim()==y.dim() then in all coordinates  i with yfixed(i)!=0
      y cannot change and so these coordinates should be ignored.
      In addition, changes to the previous eta (supplied on input) are stored 
      in sparse form in update_value and update_index and subgnorm2 is
      set to $\|newy-y\|_H^2$.
  */
  virtual int update_eta_step(CH_Matrix_Classes::Matrix& newy,
			      CH_Matrix_Classes::Matrix& eta,
			      CH_Matrix_Classes::Indexmatrix& update_index,
			      CH_Matrix_Classes::Matrix& update_value,
			      CH_Matrix_Classes::Real& subgnorm2,
			      const CH_Matrix_Classes::Matrix& subg,
			      const CH_Matrix_Classes::Matrix& y,
			      const CH_Matrix_Classes::Matrix& lby,
			      const CH_Matrix_Classes::Matrix& uby,
			      const CH_Matrix_Classes::Indexmatrix& bound_index,
			      const CH_Matrix_Classes::Indexmatrix& yfixed) const;


  /** @brief computes the dual QP costs Q, d, and the constant offset to the bundle subproblem

     For simplicity consider the bundle subproblem problem 
     $$ \min_y \max_{x\in X} (b-eta-Ax)^\top y + c^\top x+\frac{u}{2}\|y-\haty\|_H^2,$$ 
     where b is a linear cost term for y, eta are the Lagrange multipliers for nonnegativity
     constraints on y and, e.g., A contains xdim subgradients $s_i$ in
     its columns, $c_i$ give the height of the supporting hyperplane
     corresponding to $s_i$ for $y=0$, $x\in X$ describes convex
     combinations and $H$ is the quadratic term. Then the dual reads
     $$ \max_{x\in X}-\frac1{2u}x^\top Qx+d^\top x+offset $$
     with $Q=A^TH^{-1}A$, d=(b-eta... 
  */
  virtual int compute_QP_costs(CH_Matrix_Classes::Symmatrix& Q,
			       CH_Matrix_Classes::Matrix& d,
			       CH_Matrix_Classes::Real& offset,
                               const BundleQPData* datap,
			       const CH_Matrix_Classes::Integer xdim,
			       const CH_Matrix_Classes::Matrix& y,
			       const CH_Matrix_Classes::Matrix& lby,
			       const CH_Matrix_Classes::Matrix& uby,
			       const CH_Matrix_Classes::Matrix& eta,
			       const CH_Matrix_Classes::Indexmatrix& yfixed) const;

  

  /** @brief computes the update of the dual QP cost terms d and offset for 
      changes in eta given by update_value and update_index 
  */
  virtual int update_QP_costs(CH_Matrix_Classes::Matrix& delta_d,  
			      CH_Matrix_Classes::Real& delta_offset,
			      const BundleQPData* datap,
			      const CH_Matrix_Classes::Integer xdim,
			      const CH_Matrix_Classes::Matrix& y,
			      const CH_Matrix_Classes::Matrix& lby,
			      const CH_Matrix_Classes::Matrix& uby,
			      const CH_Matrix_Classes::Matrix& eta,
			      const CH_Matrix_Classes::Indexmatrix& update_index,
			      const CH_Matrix_Classes::Matrix& update_value) const;

};


  //@}
}

#endif

