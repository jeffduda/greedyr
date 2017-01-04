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
typename itk::Transform<double, ImageType::ImageDimension, ImageType::ImageDimension>::Pointer
greedyregistration( typename ImageType::Pointer fixed, typename ImageType::Pointer moving )
{
  //Rcpp::Rcout << "Setup the GreedyrApproach" << std::endl;

  GreedyParameters params;

  GreedyParameters::SetToDefaults(params);

  params.dim = ImageType::ImageDimension;
  params.metric = GreedyParameters::MetricType::NCC;
  params.mode = GreedyParameters::Mode::AFFINE;

  std::vector<int> metric_radius = {2, 2};
  params.metric_radius = metric_radius;

  std::vector<int> iter_per_level = {100,50,10};
  params.iter_per_level = iter_per_level;

  typedef typename ImageType::InternalPixelType PixelType;
  const unsigned int Dimension = ImageType::ImageDimension;

  typedef GreedyrApproach<Dimension, PixelType> GreedyType;
  GreedyType greedyrapi;

  typedef typename GreedyType::LinearTransformType TransformType;
  typedef itk::MatrixOffsetTransformBase<double, ImageType::ImageDimension, ImageType::ImageDimension> CachedTransformType;

  //typedef typename itk::Matri
  typename CachedTransformType::Pointer affineTransform = CachedTransformType::New();
  std::string affineName("AFFINE-0");
  greedyrapi.AddCachedInputObject(affineName, affineTransform);
  TransformSpec transformSpec;
  transformSpec.filename = affineName;
  transformSpec.exponent = 1.0;

  //Rcpp::Rcout << "Set input names" << std::endl;

  std::string fixedName("FIXED-0");
  std::string movingName("MOVING-0");

  ImagePairSpec ip;
  ip.weight = 1.0;
  ip.fixed = fixedName;
  ip.moving = movingName;
  params.inputs.push_back(ip);

  params.output = affineName.c_str();

  //Rcpp::Rcout << "Set input pointers" << std::endl;

  greedyrapi.AddCachedInputObject(fixedName, fixed);
  greedyrapi.AddCachedInputObject(movingName, moving);

  //Rcpp::Rcout << "Run()" << std::endl;
  int flag = greedyrapi.Run(params);
  //Rcpp::Rcout << "Done" << std::endl;

  //CachedTransformPointer mat = greedyrapi.GetAffineMatrixViaCache(transformSpec);
  typename GreedyType::BaseTransformPointer mat = greedyrapi.GetAffineMatrixViaCache(transformSpec);
  //std::cout << mat << std::endl;
  //std::cout << "return and wrap" << std::endl;
  //Rcpp::Rcout << mat << std::endl;

  return mat;
}

RcppExport SEXP greedyregistration( SEXP fixed_r, SEXP moving_r )
{
try
{

  Rcpp::S4 fixed_image( fixed_r );
  std::string fixed_pixeltype = Rcpp::as< std::string >( fixed_image.slot( "pixeltype" ) ) ;
  unsigned int fixed_dimension = Rcpp::as< unsigned int >( fixed_image.slot( "dimension" ) ) ;

  Rcpp::S4 moving_image( moving_r );
  std::string moving_pixeltype = Rcpp::as< std::string >( fixed_image.slot( "pixeltype" ) ) ;
  unsigned int moving_dimension = Rcpp::as< unsigned int >( fixed_image.slot( "dimension" ) ) ;

  if ( fixed_pixeltype != moving_pixeltype ) {
    Rcpp::stop( "Images must have same pixeltype" );
  }
  if ( fixed_dimension != moving_dimension ) {
        Rcpp::stop( "Images must have same dimension" );
  }

  if ( fixed_pixeltype == std::string( "double" ) ) {
    //Rcpp::Rcout << "Running as double precision" << std::endl;

    if ( fixed_dimension == 2) {
      typedef itk::VectorImage<double,2> ImageType;
      typedef ImageType::Pointer ImagePointerType;

      return Rcpp::wrap( greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r) ) );
    }
    else if ( fixed_dimension == 3 ) {

    }
    else if ( fixed_dimension == 4 ) {

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

      return Rcpp::wrap( greedyregistration<ImageType>(Rcpp::as<ImagePointerType>(fixed_r), Rcpp::as<ImagePointerType>(moving_r) ) );
    }
    else if ( fixed_dimension == 3 ) {

    }
    else if ( fixed_dimension == 4 ) {

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
