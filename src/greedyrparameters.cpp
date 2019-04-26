#include "greedyrparameters.h"

Rcpp::List greedyParametersToR( GreedyParameters param )
{
try
{
  //enum MetricType { SSD = 0, NCC, MI, NMI, MAHALANOBIS };
  //enum TimeStepMode { CONSTANT=0, SCALE, SCALEDOWN };
  //enum Mode { GREEDY=0, AFFINE, BRUTE, RESLICE, INVERT_WARP, ROOT_WARP, JACOBIAN_WARP, MOMENTS };

  std::string mode = "";
  std::string metric = "";
  std::string time_step_mode="";

  switch(param.mode) {
    case GreedyParameters::GREEDY : mode="GREEDY";
         break;
    case GreedyParameters::AFFINE : mode="AFFINE";
         break;
    case GreedyParameters::BRUTE : mode="BRUTE";
         break;
    default : Rcpp::stop("Unsupported 'mode'");
         break;
  }

  switch(param.metric) {
    case GreedyParameters::SSD : metric="SSD";
         break;
    case GreedyParameters::NCC : metric="NCC";
         break;
    case GreedyParameters::MI : metric="MI";
         break;
    case GreedyParameters::NMI : metric="NMI";
        break;
    case GreedyParameters::MAHALANOBIS : metric="MAHALANOBIS";
        break;
    default : Rcpp::stop("Unsupported 'metric'");
         break;
  }

  switch(param.time_step_mode) {
    case GreedyParameters::CONSTANT : time_step_mode="CONSTANT";
         break;
    case GreedyParameters::SCALE : time_step_mode="SCALE";
         break;
    case GreedyParameters::SCALEDOWN:time_step_mode="SCALEDOWN";
         break;
    default : Rcpp::stop("Unsupported 'time_step_mode'");
         break;
  }

  Rcpp::NumericVector epsilon_per_level( param.epsilon_per_level.begin(), param.epsilon_per_level.end() );
  Rcpp::NumericVector iter_per_level( param.iter_per_level.begin(), param.iter_per_level.end() );

  Rcpp::List sigma_pre = Rcpp::List::create(
    Rcpp::Named("sigma") = param.sigma_pre.sigma,
    Rcpp::Named("physical_units") = param.sigma_pre.physical_units
  );

  Rcpp::List sigma_post = Rcpp::List::create(
    Rcpp::Named("sigma") = param.sigma_post.sigma,
    Rcpp::Named("physical_units") = param.sigma_post.physical_units
  );

  // FIXME - do to limited Rcpp::List size, may need
  // to make multiple lists and merge after return to R
  Rcpp::List rparam = Rcpp::List::create(
      Rcpp::Named("dim")                = param.dim,
      Rcpp::Named("mode")               = mode,
      Rcpp::Named("metric")             = metric,
      Rcpp::Named("affine_dof")         = (int) param.affine_dof,
      Rcpp::Named("epsilon_per_level")  = epsilon_per_level,
      Rcpp::Named("iter_per_level")     = iter_per_level,
      Rcpp::Named("background")         = param.background,
      Rcpp::Named("flag_float_math")    = param.flag_float_math,
      Rcpp::Named("deriv_epsilon")      = param.deriv_epsilon,
      Rcpp::Named("time_step_mode")     = time_step_mode,
      Rcpp::Named("sigma_pre")          = sigma_pre,
      Rcpp::Named("sigma_post")         = sigma_post,
      Rcpp::Named("warp_exponent")      = param.warp_exponent,
      Rcpp::Named("warp_precision")     = param.warp_precision,
      Rcpp::Named("ncc_noise_factor")   = param.ncc_noise_factor,
      Rcpp::Named("threads")            = param.threads
    );

/* $=implemented, X=ignore here
  $ param.dim = 2;
  $ param.mode = GreedyParameters::GREEDY;
  X param.flag_dump_moving = false;
  X param.flag_debug_deriv = false;
  X param.flag_debug_aff_obj = false;
  X param.dump_frequency = 1;
  $ param.epsilon_per_level = std::vector<double>(1, 1.0);
  $ param.sigma_pre.sigma = sqrt(3.0);
  $ param.sigma_pre.physical_units = false;
  $ param.sigma_post.sigma = sqrt(0.5);
  $ param.sigma_post.physical_units = false;
  $ param.threads = 0;
  $ param.metric = GreedyParameters::SSD;
  $ param.time_step_mode = GreedyParameters::SCALE;
  $ param.deriv_epsilon = 1e-4;
  X param.flag_powell = false;
  $ param.warp_exponent = 6;
  $ param.warp_precision = 0.1;
  $ param.ncc_noise_factor = 0.001;
  param.affine_init_mode = VOX_IDENTITY;
  $ param.affine_dof = GreedyParameters::DOF_AFFINE;
  param.affine_jitter = 0.5;
  $ param.flag_float_math = false;
  param.flag_stationary_velocity_mode = false;
  param.flag_stationary_velocity_mode_use_lie_bracket = false;
  $ param.background = 0.0;
  param.current_weight = 1.0;

  $ param.iter_per_level.push_back(100);
  $ param.iter_per_level.push_back(100);

  // Moments of inertia parameters
  param.moments_flip_determinant = 0;
  param.flag_moments_id_covariance = false;
  param.moments_order = 1;
  */

  return( rparam );

}
catch( itk::ExceptionObject & err )
  {
  Rcpp::Rcout << "ITK ExceptionObject caught !" << std::endl;
  forward_exception_to_r( err );
  }
catch( const std::exception& exc )
  {
  Rcpp::Rcout << "STD ExceptionObject caught !" << std::endl;
  forward_exception_to_r( exc );
  }
catch(...)
  {
	Rcpp::stop("c++ exception (unknown reason)");
  }
return Rcpp::wrap(NA_REAL); //not reached
}

GreedyParameters greedyParametersFromR( Rcpp::List rparam )
{
  GreedyParameters param;



  std::string rmode = Rcpp::as<std::string>( rparam["mode"] );
  std::string rmetric = Rcpp::as<std::string>( rparam["metric"] );
  std::string rtime_step_mode = Rcpp::as<std::string>( rparam["time_step_mode"] );
  int raffine_dof = Rcpp::as<int>( rparam["affine_dof"] );

  if ( rmode == "GREEDY" ) {
    param.mode = GreedyParameters::GREEDY;
  }
  else if ( rmode == "AFFINE") {
    param.mode = GreedyParameters::AFFINE;
  }
  else if ( rmode == "BRUTE" ) {
    param.mode = GreedyParameters::BRUTE;
  }
  else {
    Rcpp::stop("Unsupported mode: " + rmode);
  }

  if ( rmetric == "SSD" ) {
    param.metric = GreedyParameters::SSD;
  }
  else if ( rmetric == "MI" ) {
    param.metric = GreedyParameters::MI;
  }
  else if ( rmetric == "NMI" ) {
    param.metric = GreedyParameters::NMI;
  }
  else if ( rmetric == "NCC" ) {
    param.metric = GreedyParameters::NCC;
  }
  else if ( rmetric == "MAHALANOBIS" ) {
    param.metric = GreedyParameters::MAHALANOBIS;
  }
  else {
    Rcpp::stop("Invalid metric: " + rmetric);
  }

  if ( rtime_step_mode == "CONSTANT" ) {
    param.time_step_mode = GreedyParameters::CONSTANT;
  }
  else if ( rtime_step_mode == "SCALE") {
    param.time_step_mode = GreedyParameters::SCALE;
  }
  else if ( rtime_step_mode == "SCALEDOWN" ) {
    param.time_step_mode = GreedyParameters::SCALEDOWN;
  }
  else {
    Rcpp::stop("Unsupported time_step_mode");
  }

  if ( raffine_dof == 6 ) {
    param.affine_dof = GreedyParameters::DOF_RIGID;
  }
  else if (raffine_dof == 7 ) {
    param.affine_dof = GreedyParameters::DOF_SIMILARITY;
  }
  else if (raffine_dof == 12 ) {
    param.affine_dof = GreedyParameters::DOF_AFFINE;
  }
  else {
    Rcpp::stop("Invalid affine_dof");
  }

  Rcpp::NumericVector riter_per_level = rparam["iter_per_level"];
  std::vector<int> iter_per_level( riter_per_level.begin(), riter_per_level.end() );

  Rcpp::NumericVector repsilon_per_level = rparam["epsilon_per_level"];
  std::vector<double> epsilon_per_level( repsilon_per_level.begin(), repsilon_per_level.end() );

  Rcpp::List rsigma_pre = rparam["sigma_pre"];
  SmoothingParameters sigma_pre;
  sigma_pre.sigma = Rcpp::as<double>( rsigma_pre["sigma"]);
  sigma_pre.physical_units = Rcpp::as<bool>( rsigma_pre["physical_units"] );

  Rcpp::List rsigma_post = rparam["sigma_pre"];
  SmoothingParameters sigma_post;
  sigma_post.sigma = Rcpp::as<double>( rsigma_post["sigma"]);
  sigma_post.physical_units = Rcpp::as<bool>( rsigma_post["physical_units"] );

  param.dim               = Rcpp::as<unsigned int>( rparam["dim"] );
  param.threads           = Rcpp::as<unsigned int>( rparam["threads"] );
  param.background        = Rcpp::as<double>( rparam["background"] );
  param.warp_exponent     = Rcpp::as<int>( rparam["warp_exponent"] );
  param.warp_precision    = Rcpp::as<double>( rparam["warp_precision"] );
  param.flag_float_math   = Rcpp::as<bool>( rparam["flag_float_math"] );
  param.deriv_epsilon     = Rcpp::as<double>( rparam["deriv_epsilon"] );
  param.ncc_noise_factor  = Rcpp::as<double>( rparam["ncc_noise_factor"] );
  param.sigma_pre         = sigma_pre;
  param.sigma_post        = sigma_post;
  param.epsilon_per_level = epsilon_per_level;

  return(param);

}

GreedyParameters greedyParametersFromR( SEXP rparam )
{
  Rcpp::List listParam( rparam );
  return(greedyParametersFromR(listParam));
}

RcppExport SEXP defaultGreedyParameters( )
{
  GreedyParameters param;
  GreedyParameters::SetToDefaults( param );
  return Rcpp::wrap( greedyParametersToR( param ));
}
