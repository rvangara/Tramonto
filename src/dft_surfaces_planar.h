/* This file was automatically generated.  Do not edit! */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(HAS_VALUES_H)
#include <values.h>
#include <unistd.h>
#include <string.h>
#endif
#include "mpi.h"
#include "az_aztec.h"
#include "az_aztec_defs.h"
#include "rf_allo.h"
#include "dft_basic_lin_prob_mgr_wrapper.h"
#include "dft_poly_lin_prob_mgr_wrapper.h"
#include "dft_hardsphere_lin_prob_mgr_wrapper.h"
#include "Tramonto_ConfigDefs.h"
#define NDIM_MAX  3
extern double Esize_x[NDIM_MAX];
extern double **X_wall;
extern int *B2L_node;
int ijk_box_to_node_box(int *ijk_box);
void ijk_to_ijk_box(int *ijk,int *ijk_box);
extern int Ndim;
extern int *L2G_node;
void node_to_ijk(int node,int *ijk);
extern double Dielec_X;
#define DIELEC_WF_PORE     2
extern int Type_dielec;
#define FALSE 0
#if !defined(_CON_CONST_H_)
#define _CON_CONST_H_
#endif
#if !defined(FALSE) && !defined(_CON_CONST_H_)
#define FALSE 0
#endif
#define TRUE  1
#if !defined(TRUE) && !defined(_CON_CONST_H_)
#define TRUE  1
#endif
typedef struct SurfaceGeom_Struct SurfaceGeom_Struct;
extern struct SurfaceGeom_Struct *SGeom;
#define NWALL_MAX 600 
extern int Lwedge_cutout[NWALL_MAX];
#define NWALL_MAX_TYPE 50 
extern int Lperiodic_overlay[NWALL_MAX_TYPE];
extern int Nperiodic_overlay[NWALL_MAX_TYPE];
#define NPERIODIC_MAX 4
extern int Llinear_overlay[NWALL_MAX_TYPE];
extern int Nlinear_overlay[NWALL_MAX_TYPE];
struct SurfaceGeom_Struct {
  int       surfaceTypeID;    /* ID of the type of surface */
  int       orientation;  /* orientation of the surface */
  double    *halfwidth;   /* planar surface params given in halfwidth */
  double    radius;       /* radius of spherical or cylindrical surface */
  double    halflength;   /* length of finite cylinders and pores */
  double    radius2;      /* a second radius for tapered pores or cylinders */
  int       Lwedge_cutout;    /* TRUE or FALSE for wedge cutout from basic surfac */
  double    angle_wedge_start;    /* angle as measured from x0 axis */
  double    angle_wedge_end;    /* angle as measured from x0 axis */
  int       Lrough_surface;    /* TRUE or FALSE for surface roughness */
  double    roughness;          /* maximum roughness amplitude */
  double    roughness_length;    /* lengthscale for the rougness */
  int       Lperiodic_overlay;    /* TRUE or FALSE for periodic function added to surface */
  int       Nperiodic_overlay;     /* The number of periodic functions to apply */
  double    orientation_periodic[NPERIODIC_MAX];    /* maximum amplitude for a cosine wave superimposed on a surface */
  double    amplitude[NPERIODIC_MAX];    /* maximum amplitude for a cosine wave superimposed on a surface */
  double    wavelength[NPERIODIC_MAX];    /* desired wavelength of cosine wave superimposed on a surface */
  double    origin_PeriodicFunc[NPERIODIC_MAX];     /* The origin of periodic functions to apply */
  int       Llinear_overlay;    /* TRUE or FALSE for linear function added to surface */
  int       Nlinear_overlay;     /* The number of linear functions to apply */
  double    orientation_linear[NPERIODIC_MAX];    /* maximum amplitude for a linear function superimposed on a surface */
  double    slope[NPERIODIC_MAX];    /* maximum amplitude for a linear function superimposed on a surface */
  double    origin_LinearFunc[NPERIODIC_MAX];     /* The origin of linear functions to apply */
  double    endpoint_LinearFunc[NPERIODIC_MAX];     /* The end point of linear functions to apply */
  int    *ReflectionsAreIndependent;  /* TRUE or FALSE for treating special boundary conditions */
};
void surface_planar_inSurfaceTest(int iwall,int iwall_type,int loc_inode,int flag_setup_Xwall,double *fluidEl_center,double **image_pos,double dist_adjustments,double *delx,int *logical_inwall,int *logical_nearWallDielec);