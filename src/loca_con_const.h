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
 * $Name$
 *====================================================================*/
#ifndef _CON_CONST_H_
#define _CON_CONST_H_

/*
#ifndef lint
static char *cvs_conconsth_id =
  "$Id$";
#endif
*/

/*****************************************************************************/
/*                       DEFINE STATEMENTS                                   */
/*****************************************************************************/
#define TRUE  1
#define FALSE 0

/* Choices for continuation method */
#define ZERO_ORDER_CONTINUATION        0
#define FIRST_ORDER_CONTINUATION       1
#define ARC_LENGTH_CONTINUATION        2
#define TURNING_POINT_CONTINUATION     3
#define PITCHFORK_CONTINUATION         4
#define HOPF_CONTINUATION              5
#define PHASE_TRANSITION_CONTINUATION  6
#define AUGMENTING_CONDITION           7

/* Choices for matrix fill -- what quantities to calculate */
#define  RHS_ONLY        100
#define  MATRIX_ONLY     101
#define  RHS_MATRIX      102

/* Choices for linear solve about the state of the Jacobian matrix*/
#define  NEW_JACOBIAN                200
#define  OLD_JACOBIAN                201
#define  SAME_BUT_UNSCALED_JACOBIAN  202
#define  OLD_JACOBIAN_DESTROY        203

/* Internal flags for rhs calculation for continuation linear solves*/
#define  CONT_TANGENT      300
#define  ARC_CONT_SOL2     301
#define  TP_CONT_SOL2      302
#define  TP_CONT_SOL3      303
#define  TP_CONT_SOL4      304
#define  HP_CONT_SOL3      305
#define  HP_CONT_DMDX      306
#define  HP_CONT_DMDPARAM  307

/*****************************************************************************/
/*                       STRUCTURE DEFINITIONS                               */
/*****************************************************************************/

/*
 * The first structures are sub-structures of con_struct, the
 * structure containing all he user-supplied info.
 */

struct general_info_struct
{
  int method;			/* Solution strategy: see method choices above      */
  double param;			/* value of continuation parameter                  */
  double *x;			/* current solution vector                          */
  int numUnks;			/* Num unknowns on this Proc (including externals)  */
  int numOwnedUnks;		/* Num unknowns on this Proc (NOT incl. externals)  */
  int printproc;		/* Logical indicating if this Proc prints to stdout */
};

struct stepping_info_struct
{
  double first_step;		/* Initial step size                                 */
  int max_steps;		/* Maximum # of continuation steps                   */
  double max_param;		/* parameter value to stop at                        */
  double max_delta_p;		/* continuation parameter step limit                 */
  double step_ctrl;		/* step aggressiveness -- 0.0 for constant step      */
  int max_newton_its;		/* Max # Newton steps, used only for step control  */
};

struct arclength_info_struct
{
  double dp_ds2_goal;		/* Square of target dp_ds value for rescaling
				   (desired solution contribution to arc length)     */
  double dp_ds_max;		/* High dp_ds value at which to rescale              */
  double tang_exp;		/* Power to which tang_factor is raised              */
  double tang_step_limit;	/* Minimum value of tang_factor between steps    */
};

struct turning_point_info_struct
{
  double bif_param;		/* Initial guess of bifurcation parameter     */
};

struct pitchfork_info_struct
{
  double bif_param;		/* Initial guess of bifurcation parameter     */
  double *psi;			/* Antisymmetry vector (also init guess for   */
  /* the null vector, y_vec                     */
};

struct hopf_info_struct
{
  double bif_param;		/* Initial guess of bifurcation parameter     */
  double omega;			/* Initial guess of Hopf frequency            */
  double *y_vec;		/* Initial guess of null vector (real)        */
  double *z_vec;		/* Initial guess of null vector (imag)        */
  int mass_flag;
};

struct phase_transition_info_struct
{
  double bif_param;		/* Initial guess of bifurcation parameter     */
  double *x2;			/* Initial guess of second_solution_vector    */
};

struct eigen_info_struct
{
  int Num_Eigenvalues;		/* Number of Eigenvalues to Calculate          */
  double Shift_Point[3];	/* Point for shift and invert (Real, Imaginary) */
  /* and for cayley delta                        */
  int Arnoldi;			/* Arnoldi Space Size                          */
  double Residual_Tol[2];	/* Convergence Tolerance for the Eigenvalue    *
				 * Residual Equation, and linear solver tol    */
  int Max_Iter;			/* Maximum number of iterations of eigensolver */
  int Every_n_Steps;		/* Allow for eigenvalue calc every n steps along
				   a continuation run                          */
};

struct private_info_struct
{
  int mass_x;			/* flag that turns on dM/dx in komplex solves           */
  int mass_param;		/* flag that turns on dM/d(param) in komplex solves     */
  int first_iter;		/* flag for first Newton iter of each solve             */
  int step_num;			/* Current continuation step number                     */
  double param_old;		/* old value of continuation parameter                  */
  double arc_step;		/* step size of arc length variable                     */
  double dp_ds;			/* derivative of parameter w.r.t. arc length            */
  double *x_old;		/* previous solution vector                             */
  double *x_tang;		/* tangent to the solution vector w.r.t. parameter      */
  double *scale_vec;		/* scaling vector for better arclength conditioning     */
};

/*
 * con_struct: structure of parameters for controllong
 *                 the continuation routines
 *      Most of these are info to be set by the user
 *      The private_info stuct contains internal info not
 *      intended to be accessed or set by the user.
 */

struct con_struct
{
  struct general_info_struct general_info;
  struct stepping_info_struct stepping_info;
  struct arclength_info_struct arclength_info;
  struct turning_point_info_struct turning_point_info;
  struct pitchfork_info_struct pitchfork_info;
  struct hopf_info_struct hopf_info;
  struct phase_transition_info_struct phase_transition_info;
  struct eigen_info_struct eigen_info;

  struct private_info_struct private_info;
};

/*****************************************************************************/

extern int con_lib (struct con_struct *con);
extern int arc_length_bordering_alg (double *x, double *delta_x,
				     struct con_struct *con,
				     double reltol, double abstol);
extern int turning_point_alg (double *x, double *delta_x,
			      struct con_struct *con,
			      double reltol, double abstol);
extern int nonlinear_solver_conwrap (double *x, void *con, int step_num);
extern void assign_parameter_conwrap (double param);
extern void assign_bif_parameter_conwrap (double bif_param);
extern int linear_solver_conwrap (double *x, int jac_flag, double *tmp);
extern int komplex_linear_solver_conwrap (double *x, double *y,
					  int jac_flag, double *omega,
					  double *tmp);
extern void calc_scale_vec_conwrap (double *x, double *scale_vec,
				    int numUnks);
extern double gsum_double_conwrap (double sum);
extern int gmax_int_conwrap (int sum);
extern void perturb_solution_conwrap (double *x, double *p, double *s, int n);
extern void solution_output_conwrap (int type, double *x, double param,
				     double *x2, double param2,
				     double *x3, double param3, int stp,
				     int nits, struct con_struct *con);
extern void calc_rhs_continuation (int rhs_type, double *x, double *a,
				   double *x_dot, double *scale_vec,
				   double *x_tmp, double con_par,
				   double *r_vec, int num_total_unknowns,
				   int num_owned_unks);
extern void matrix_residual_fill_conwrap (double *x, double *rhs,
					  int matflag);
extern void mass_matrix_fill_conwrap (double *x, double *rhs);
extern void matvec_mult_conwrap (double *x, double *y);
extern void random_vector_conwrap (double *x, int n);
extern double free_energy_diff_conwrap (double *x, double *x2);
extern void get_scale_vec_conwrap (int iteration, int flag,
				   double *x, double *scale_vec);
extern void mass_matvec_mult_conwrap (double *x, double *y);
extern void shifted_matrix_fill_conwrap (double sigma);
extern void create_shifted_matrix_conwrap ();
extern void shifted_linear_solver_conwrap (double *x, double *y,
					   int jac_flag, double tol);
extern void destroy_shifted_matrix_conwrap ();
#endif