/*====================================================================
 * ------------------------
 * | CVS File Information |
 * ------------------------
 *
 * $RCSfile$
 *
 * $Author$
 *
 * $Date$
 *
 * $Revision$
 *
 *====================================================================*/

/*
 *  FILE: dft_output.c
 *
 *  This file contains routines that post-process the density profiles.
 *
 */

#include "mpi.h"
#include "dft_globals_const.h"
#include "rf_allo.h"

/*********************************************************************************
calc_flux:  This routine calculates and prints out the flux
            in the domain --- at steady state this should be 
            constant everywhere. */
void
calc_flux (FILE * fp, char *output_flux, double *X_old)
{

  int idim, icomp, inode, loc_i, ijk[3], iunk,
    loc_i_minus1, loc_i_plus1, dim_flx, edge;
  int inode_minus1, inode_plus1, ijk_minus1[3], ijk_plus1[3];
  double grad_mu[NCOMP_MAX], current, flux[NCOMP_MAX];
  FILE *ifp;

  ifp = fopen (output_flux, "w");
  for (inode = 0; inode < Nnodes; inode++)
    {
      node_to_ijk (inode, ijk);

      edge = FALSE;
      for (idim = 0; idim < Ndim; idim++)
	if (ijk[idim] == 0 || ijk[idim] == Nodes_x[idim] - 1)
	  edge = TRUE;

      if (!edge)
	for (idim = 0; idim < Ndim; idim++)
	  fprintf (ifp, "  %9.6f ", ijk[idim] * Esize_x[idim]);

      if (!edge)
	{
	  for (dim_flx = 0; dim_flx < Ndim; dim_flx++)
	    {

	      current = 0.0;
	      for (icomp = 0; icomp < Ncomp; icomp++)
		{
		  if (Matrix_fill_flag < 3)
		    iunk = icomp + Ncomp;
		  else
		    iunk = icomp + Ncomp + Nrho_bar;
		  if (Ipot_ff_c == COULOMB)
		    iunk++;

		  loc_i = icomp + Nunk_per_node * inode;
		  for (idim = 0; idim < Ndim; idim++)
		    {
		      if (idim == dim_flx)
			{
			  ijk_minus1[idim] = ijk[dim_flx] - 1;
			  ijk_plus1[idim] = ijk[dim_flx] + 1;
			}
		      else
			{
			  ijk_minus1[idim] = ijk[idim];
			  ijk_plus1[idim] = ijk[idim];
			}
		    }
		  inode_minus1 = ijk_to_node (ijk_minus1);
		  inode_plus1 = ijk_to_node (ijk_plus1);

		  loc_i_minus1 = iunk + Nunk_per_node * inode_minus1;
		  loc_i_plus1 = iunk + Nunk_per_node * inode_plus1;

		  grad_mu[icomp] =
		    (X_old[loc_i_plus1] -
		     X_old[loc_i_minus1]) / (2 * Esize_x[dim_flx]);

		  if (Velocity <= 1.e-6)
		    {
		      if (Linear_transport)
			flux[icomp] = -grad_mu[icomp];
		      else
			flux[icomp] = -X_old[loc_i] * grad_mu[icomp];
		      flux[icomp] *= D_coef[icomp] * 1.602e-19 *
			/*   Area_IC_old[inode]/(POW_DOUBLE_INT(3.0e-8,2)*1.e-12); */
			1.0 / (POW_DOUBLE_INT (3.0e-8, 2) * 1.e-12);
		    }
		  else
		    flux[icomp] =
		      -X_old[loc_i] * grad_mu[icomp] * D_coef[icomp] +
		      Velocity * D_coef[icomp] * X_old[loc_i];

		  if (Ipot_ff_c == 1)
		    current += flux[icomp] * Charge_f[icomp];
		  fprintf (ifp, "  %9.6f  ", flux[icomp]);
/*
 * need to write routine to collect Area_IC array on proc 0 in order to
 *  do the above calculation correctly for 1D models with varying area! For now, set Area_IC to 1 everywhere
 */
		}
	      if (Ipot_ff_c == 1)
		fprintf (ifp, "  %9.6f  ", current);

	    }			/* loop over dim_flx */
	  fprintf (ifp, "\n");
	}			/* end of test for boundary node */
    }
  fclose (ifp);
  printf ("got through first part of calculations\n");

/*
  for (idim=0; idim<Ndim; idim++) ijk[idim] = 0.5*Nodes_x[idim];
  loc_inode = ijk_to_node(ijk);
  inode_box = L2B_node[loc_inode];
  current = 0;

  for (icomp=0; icomp<Ncomp; icomp++){

      if (Matrix_fill_flag < 3) iunk =icomp+Ncomp;
      else                       iunk = icomp+Ncomp+Nrho_bar;
      if (Ipot_ff_c==COULOMB) iunk++;

      loc_i = Aztec.update_index[icomp + Nunk_per_node * loc_inode];
      loc_i_minus1 = Aztec.update_index[iunk + 
                     Nunk_per_node * (loc_inode-1)];
      loc_i_plus1 = Aztec.update_index[iunk + 
                         Nunk_per_node * (loc_inode+1)];
      grad_mu = (x[loc_i_plus1]-x[loc_i_minus1])/(2*Esize_x[0]);
      if (Ipot_ff_c==1){
      if (Linear_transport){
         flux = - grad_mu*D_coef[icomp]*1.602e-19*
                    Area_IC[inode_box]/
                    (POW_DOUBLE_INT(4.25e-8,2)*1.e-12);

         current += flux*Charge_f[icomp];
      }
      else{
         flux = - x[loc_i]*grad_mu*D_coef[icomp]*1.602e-19*
                 Area_IC[inode_box]/
                 (POW_DOUBLE_INT(4.25e-8,2)*1.e-12);
         current += flux*Charge_f[icomp];
      }
      }
      else{
         flux = - x[loc_i]*grad_mu*D_coef[icomp]+Velocity*D_coef[icomp]*x[loc_i];
      }
      fprintf(fp,"  %g ",flux);
  }
  if (Ipot_ff_c==COULOMB) fprintf(fp,"  %g ",current);
*/
}

/*******************************************************************************/