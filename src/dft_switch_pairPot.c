/*
//@HEADER
// ******************************************************************** 
// Tramonto: A molecular theory code for structured and uniform fluids
//                 Copyright (2006) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
// ********************************************************************
//@HEADER
*/

/*
 *  FILE: dft_switch_pairPot.c
 *
 *  This file contains routines selects the correct pair interaction potential
 *  functions to call for computing external fields based on pair potentials and
 *  for performing mean field DFT calculations.
 *
 */

#include "dft_switch_pairPot.h"

/******************************************************************************/
/* pairPot_switch:  switch to choose the correct pair potential when using
           integrated potential surfaces, atomistic surface, or when computing
           Barker-Henderson diameters */
double pairPot_switch(double r,double param1, double param2, double param3)
{
  double u;
  int Type_pairPot=0;

  switch(Type_pairPot){
      case PAIR_LJ12_6_CS:
        u= uLJ12_6_CS(r,param1,param2,param3);
        break;
      case PAIR_COULOMB:
        u = uCOULOMB(r,param1,param2);
        break;
      default:
         printf("problems with your selection of Type_pairPot\n");
         exit(-1);
         break;
  }
  return u;
}
/******************************************************************************/
/* pairPot_deriv_switch:  switch to choose the correct pair potential derivative
           needed for force calculations */
double pairPot_deriv_switch(double r, double x, double param1, double param2, double param3)
{
  double uderiv;
  int Type_pairPot=0;

  switch(Type_pairPot){
      case PAIR_LJ12_6_CS:
        uderiv= uLJ12_6_DERIV1D(r,x,param1,param2,param3);
        break;
      case PAIR_COULOMB:
        uderiv = uCOULOMB_DERIV1D(r,x,param1,param2);
        break;
      default:
         printf("problems with your selection of Type_pairPot\n");
         exit(-1);
         break;
  }
  return uderiv;
}
/******************************************************************************/
/* pairPot_ATT_CS_switch:  switch to choose the correct pair potential for
           mean field perturbation DFT calculations with a hard sphere
           reference fluid */
double pairPot_ATT_CS_switch(double r, int icomp, int jcomp)
{
  double u;
  int Type_pairPot=0;

  switch(Type_pairPot){
      case PAIR_LJ12_6_CS:
        u= uLJ12_6_ATT_CS(r,icomp,jcomp);
        break;
      case PAIR_COULOMB:
        u = uCOULOMB_ATT_noCS(r,icomp,jcomp);   /* no cut and shift implemented at this time */
        break;
      default:
         printf("problems with your selection of Type_pairPot\n");
         exit(-1);
         break;
  }
  return u;
}
/******************************************************************************/
/* pairPot_ATT_noCS_switch:  switch to choose the correct integrated MFDFT 
           pair potential (e.g. only the attractive part of the 12-6 potential)
           for setting up the integration stencils for DFT calculations. */
double pairPot_ATT_noCS_switch(double r, int icomp, int jcomp)
{
  double u;
  int Type_pairPot=0;

  switch(Type_pairPot){
      case PAIR_LJ12_6_CS:
        u= uLJ12_6_ATT_noCS(r,icomp,jcomp);
        break;
      case PAIR_COULOMB:
        u = uCOULOMB_ATT_noCS(r,icomp,jcomp);
        break;
      default:
         printf("problems with your selection of Type_pairPot\n");
         exit(-1);
         break;
  }
  return u;
}
/******************************************************************************/
/* pairPot_integral_switch:  switch to choose the correct integrated MFDFT 
           pair potential (e.g. only the attractive part of the 12-6 potential)
           for setting up the integration stencils for DFT calculations. */
double pairPot_integral_switch(double r, int icomp, int jcomp)
{
  double u;
  int Type_pairPot=0;

  switch(Type_pairPot){
      case PAIR_LJ12_6_CS:
        u= uLJ12_6_Integral(r,icomp,jcomp);
        break;
      case PAIR_COULOMB:
        u = uCOULOMB_Integral(r,icomp,jcomp);
        break;
      default:
         printf("problems with your selection of Type_pairPot\n");
         exit(-1);
         break;
  }
  return u;
}
/******************************************************************************/
