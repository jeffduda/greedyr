/*=========================================================================

  Program:   ALFABIS fast medical image registration programs
  Language:  C++
  Website:   github.com/pyushkevich/greedy
  Copyright (c) Paul Yushkevich, University of Pennsylvania. All rights reserved.

  This program is part of ALFABIS: Adaptive Large-Scale Framework for
  Automatic Biomedical Image Segmentation.

  ALFABIS development is funded by the NIH grant R01 EB017255.

  ALFABIS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ALFABIS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ALFABIS.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================*/
#include "GreedyrAPI.h"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

#include "lddmm_common.h"
#include "lddmm_data.h"

#include <itkImageFileReader.h>
#include <itkAffineTransform.h>
#include <itkTransformFactory.h>
#include <itkTimeProbe.h>

#include "MultiImageRegistrationHelper.h"
#include "FastWarpCompositeImageFilter.h"
#include <vnl/algo/vnl_powell.h>
#include <vnl/algo/vnl_svd.h>
#include <vnl/vnl_trace.h>

#include "itkTransformFileReader.h"

template <unsigned int VDim, typename TReal>
typename GreedyrApproach<VDim, TReal>::BaseTransformPointer
GreedyrApproach<VDim, TReal>
::GetAffineMatrixViaCache(const TransformSpec &ts )
{

  // Physical (RAS) space transform matrix
  vnl_matrix<double> Qp(VDim+1, VDim+1);  Qp.set_identity();

  // An ITK-style transform - forced to floating point here
  //typedef itk::MatrixOffsetTransformBase<double, VDim, VDim> CachedTransformType;
  typename CachedTransformType::Pointer itk_tran;
  //typename LinearTransformType::Pointer itk_tran;

  // See if a transform is already stored in the cache
  typename ImageCache::const_iterator itCache = this->m_ImageCache.find(ts.filename);
  if(itCache != this->m_ImageCache.end())
    {
    CachedTransformType * cached = dynamic_cast<CachedTransformType *>(itCache->second.target);
    if(!cached)
      throw GreedyException("Cached transform %s is of the wrong type (%s, but should be %s)",
                            ts.filename.c_str(), typeid(*itCache->second.target).name(),
                            typeid(CachedTransformType).name());

    itk_tran = cached;
    }
  else
    {
    throw GreedyException("Unable to find transform %s", ts.filename.c_str());
    }

  typedef typename CachedTransformType::MatrixType MatrixType;
  typedef typename CachedTransformType::OffsetType OffsetType;
  MatrixType mat = itk_tran->GetMatrix();
  OffsetType offset = itk_tran->GetOffset();

  if ( VDim == 2 ) {
    offset[0] *= -1;
    offset[1] *= -1;
  }
  /*
  else if( VDim == 3 )
    {
    std::cout << "3d correct" << std::endl;
    offset[0] *= -1;
    offset[1] *= -1;

    mat(2,0) *= -1; mat(2,1) *= -1;
    mat(0,2) *= -1; mat(1,2) *= -1;
    }
*/

  itk_tran->SetMatrix(mat);
  itk_tran->SetOffset(offset);

  typename BaseTransformType::Pointer return_trans = dynamic_cast<BaseTransformType *>( itk_tran.GetPointer() );

  if (ts.exponent == -1.0)
    {
    return_trans = itk_tran->GetInverseTransform();
    }

  return return_trans;
  //return Qp;
}


template class GreedyrApproach<2, float>;
template class GreedyrApproach<3, float>;
template class GreedyrApproach<4, float>;
template class GreedyrApproach<2, double>;
template class GreedyrApproach<3, double>;
template class GreedyrApproach<4, double>;
