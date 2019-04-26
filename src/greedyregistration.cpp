#include <exception>
#include <vector>
#include <string>
#include <RcppANTsR.h>
#include "itkImage.h"
#include "itkVectorImage.h"
#include "GreedyrAPI.h"
#include "GreedyParameters.h"

template< class ImageType >
//typename ImageType::Pointer
//typename itk::Transform<double, ImageType::ImageDimension, ImageType::ImageDimension>::Pointer
SEXP
greedyregistration( typename ImageType::Pointer fixed, typename ImageType::Pointer moving, GreedyParameters params )
{
  Rcpp::Rcout << "Setup the GreedyrApproach" << std::endl;

  using PixelType = typename ImageType::InternalPixelType;
  using GreedyType = GreedyrApproach<ImageType::ImageDimension, PixelType>;
  using CachedTransformType = itk::MatrixOffsetTransformBase<double, ImageType::ImageDimension, ImageType::ImageDimension>;
  using CachedTransformPointerType = typename CachedTransformType::Pointer;

  GreedyType greedyrapi;

  TransformSpec transformSpec;
  CachedTransformPointerType affineTransform = CachedTransformType::New();

  if ( params.mode == GreedyParameters::Mode::AFFINE ) {

    std::string affineName("AFFINE-0");
    greedyrapi.AddCachedInputObject(affineName, affineTransform);

    transformSpec.filename = affineName;
    transformSpec.exponent = 1.0;

    params.output = affineName.c_str();
  }
  if ( params.mode == GreedyParameters::Mode::GREEDY ) {
    // can cache the warp?
  }

  Rcpp::Rcout << "Set input names" << std::endl;

  std::string fixedName("FIXED-0");
  std::string movingName("MOVING-0");

  ImagePairSpec ip;
  ip.weight = 1.0;
  ip.fixed = fixedName;
  ip.moving = movingName;
  params.inputs.push_back(ip);

  Rcpp::Rcout << "Set input pointers" << std::endl;

  greedyrapi.AddCachedInputObject(fixedName, fixed);
  greedyrapi.AddCachedInputObject(movingName, moving);

  Rcpp::Rcout << "Run()" << std::endl;
  int flag = greedyrapi.Run(params);
  if ( flag ) {
    Rcpp::Rcout << "WARNING: greedy API returned error code: " << flag << std::endl;
  }
  Rcpp::Rcout << "Done" << std::endl;

  //CachedTransformPointer mat = greedyrapi.GetAffineMatrixViaCache(transformSpec);

  //std::cout << mat << std::endl;
  //std::cout << "return and wrap" << std::endl;
  //Rcpp::Rcout << mat << std::endl;

  if ( params.mode == GreedyParameters::Mode::AFFINE ) {
    typename GreedyType::BaseTransformPointer mat = greedyrapi.GetAffineMatrixViaCache(transformSpec);
    return(Rcpp::wrap(mat));
  }
  if ( params.mode == GreedyParameters::Mode::GREEDY ) {
    return( Rcpp::wrap(params.output) );
  }

  // never reached
  return Rcpp::wrap(1);
}

RcppExport SEXP greedyregistration( SEXP fixed_r, SEXP moving_r, SEXP metric_r, SEXP mode_r, SEXP ofile_r )
{
try
{

  Rcpp::S4 fixed_image( fixed_r );
  std::string fixed_pixeltype = Rcpp::as< std::string >( fixed_image.slot( "pixeltype" ) ) ;
  unsigned int fixed_dimension = Rcpp::as< unsigned int >( fixed_image.slot( "dimension" ) ) ;

  Rcpp::S4 moving_image( moving_r );
  std::string moving_pixeltype = Rcpp::as< std::string >( fixed_image.slot( "pixeltype" ) ) ;
  unsigned int moving_dimension = Rcpp::as< unsigned int >( fixed_image.slot( "dimension" ) ) ;

  std::string metric = Rcpp::as<std::string>( metric_r );
  std::string mode = Rcpp::as<std::string>( mode_r );
  std::string ofile = Rcpp::as<std::string>( ofile_r );


  if ( fixed_pixeltype != moving_pixeltype ) {
    Rcpp::stop( "Images must have same pixeltype" );
  }
  if ( fixed_dimension != moving_dimension ) {
        Rcpp::stop( "Images must have same dimension" );
  }

  GreedyParameters params;
  GreedyParameters::SetToDefaults(params);

  if ( metric == "SSD" ) {
    params.metric = GreedyParameters::MetricType::SSD;
  }
  else if ( metric == "MI" ) {
    params.metric = GreedyParameters::MetricType::MI;
  }
  else if ( metric == "NMI") {
    params.metric = GreedyParameters::MetricType::NMI;
  }
  else if ( metric == "NCC") {
    params.metric = GreedyParameters::MetricType::NCC;
  }
  else if ( metric == "MAHAL") {
    params.metric = GreedyParameters::MetricType::MAHALANOBIS;
  }
  else {
    Rcpp::stop("Invalid metric: " + metric);
  }

  if ( mode == "GREEDY" ) {
    params.mode = GreedyParameters::Mode::GREEDY;
    params.output = ofile;
  }
  else if ( mode == "AFFINE") {
    params.mode = GreedyParameters::Mode::AFFINE;
  }
  else {
    Rcpp::stop("Unsupported mode");
  }

  std::vector<int> metric_radius = {2, 2};
  params.metric_radius = metric_radius;

  std::vector<int> iter_per_level = {100,50,10};
  params.iter_per_level = iter_per_level;



  if ( fixed_pixeltype == std::string( "double" ) ) {
    //Rcpp::Rcout << "Running as double precision" << std::endl;

    if ( fixed_dimension == 2) {
      typedef itk::VectorImage<double,2> ImageType;
      typedef ImageType::Pointer ImagePointerType;
      params.dim = 2;
      return greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r), params);
    }
    else if ( fixed_dimension == 3 ) {
      typedef itk::VectorImage<double,3> ImageType;
      typedef ImageType::Pointer ImagePointerType;
      params.dim = 3;
      return greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r), params);

    }
    else if ( fixed_dimension == 4 ) {
      params.dim = 4;
    }
    else {
      Rcpp::stop( "Images must have dimension of 2,3,or 4" );
    }

  }
  else if ( fixed_pixeltype == std::string( "float" ) ) {
    //Rcpp::Rcout << "Running as float precision" << std::endl;

    if ( fixed_dimension == 2) {
      typedef itk::VectorImage<float,2> ImageType;
      typedef ImageType::Pointer ImagePointerType;
      params.dim = 2;
      return greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r), params);
    }
    else if ( fixed_dimension == 3 ) {
      typedef itk::VectorImage<float,3> ImageType;
      typedef ImageType::Pointer ImagePointerType;
      params.dim = 3;
      return greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r), params);
    }
    else if ( fixed_dimension == 4 ) {
      params.dim = 4;
    }
    else {
      Rcpp::stop( "Images must have dimension of 2,3,or 4" );
    }

  }
  else {
    Rcpp::stop( "Images must pixeltype of 'float' or 'double'" );
  }

  GreedyApproach<2,float> greedy;
  return Rcpp::wrap(0);

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
