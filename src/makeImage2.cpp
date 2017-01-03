#include <exception>
#include <vector>
#include <string>
#include <RcppANTsR.h>
#include "itkImage.h"
#include "GreedyAPI.h"

RcppExport SEXP makeImage2(  )
{
try
{

  typedef itk::Image<float, 2> ImageType;

  ImageType::Pointer img = ImageType::New();

  return Rcpp::wrap(1);

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


/*
RcppExport SEXP makeImage3(  )
{
try
{

  Rcpp::NumericVector size(2);
  Rcpp::NumericVector spacing(2);
  Rcpp::NumericVector origin(2);
  Rcpp::NumericMatrix direction(2,2);
  unsigned int components = 1;

  size[0]=64;
  size[1]=64;
  origin[0]=0;
  origin[1]=0;
  spacing[0]=3;
  spacing[1]=3;
  direction(0,0)=1;
  direction(1,1)=1;
  direction(0,1)=0;
  direction(1,0)=0;

  SEXP img = makeImage( Rcpp::wrap("float"), Rcpp::wrap(size), Rcpp::wrap(spacing), Rcpp::wrap(origin), Rcpp::wrap(direction), Rcpp::wrap(components));

  return img;

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
*/


RcppExport SEXP makeImage4(  )
{
try
{
  typedef itk::Image<float,2>               ImageType;
  typedef ImageType::Pointer       ImagePointerType;

  ImageType::RegionType region;

  for ( unsigned int i=0; i<2; i++)
    {
    region.SetSize(i, 64);
    }

  ImageType::SpacingType   itk_spacing;
  ImageType::PointType     itk_origin;
  ImageType::DirectionType itk_direction;

  for ( unsigned int i=0; i<ImageType::ImageDimension; i++)
    {
    itk_spacing[i] = 1;
    itk_origin[i] = 4;
    for ( unsigned int j=0; j<ImageType::ImageDimension; j++)
      {
        if (i==j) {
          itk_direction(i,j) = 1;
        } else {
          itk_direction(i,j) = 0;
        }

      }
    }

  ImagePointerType image = ImageType::New();
  image->SetRegions( region );
  image->SetNumberOfComponentsPerPixel( 1 );
  image->SetSpacing( itk_spacing );
  image->SetOrigin( itk_origin );
  image->SetDirection( itk_direction );
  image->Allocate();
  image->FillBuffer(0);

  return Rcpp::wrap( image );

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


RcppExport SEXP greedyr(  )
{
try
{
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
