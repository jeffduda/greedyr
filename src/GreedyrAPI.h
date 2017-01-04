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
#ifndef GREEDYRAPI_H
#define GREEDYRAPI_H


#include "GreedyAPI.h"
#include "itkTransform.h"

/**
 * This is the top level class for the greedy software. It contains methods
 * for deformable and affine registration.
 */
template <unsigned int VDim, typename TReal = double>
class GreedyrApproach : public GreedyApproach<VDim, TReal>
{
public:

  typedef GreedyrApproach<VDim, TReal> Self;
  typedef GreedyApproach<VDim, TReal>  SuperClass;

  typedef typename SuperClass::LinearTransformType LinearTransformType;

  typedef itk::MatrixOffsetTransformBase<double, VDim, VDim> CachedTransformType;
  typedef typename CachedTransformType::Pointer CachedTransformPointer;

  typedef itk::Transform<double, VDim, VDim> BaseTransformType;
  typedef typename BaseTransformType::Pointer BaseTransformPointer;

  BaseTransformPointer GetAffineMatrixViaCache( const TransformSpec &ts );

protected:

  typedef typename SuperClass::ImageCache ImageCache;

};

#endif // GREEDYAPI_H
