//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================
#ifndef __dax_math_Compare_h
#define __dax_math_Compare_h

// This header file defines math functions that do comparisons.

#include <dax/internal/MathSystemFunctions.h>

#if _WIN32 && !defined DAX_CUDA_COMPILATION
#define DAX_USE_BOOST_MATH
#endif

namespace dax {
namespace math {


//-----------------------------------------------------------------------------
/// Returns \p x or \p y, whichever is larger.
///
DAX_EXEC_CONT_EXPORT dax::Scalar Max(dax::Scalar x, dax::Scalar y)
{
#ifdef DAX_USE_BOOST_MATH
  return (y > x)? y : x;
#else
  return DAX_SYS_MATH_FUNCTION(fmax)(x,y);
#endif
}
DAX_EXEC_CONT_EXPORT dax::Vector2 Max(dax::Vector2 x, dax::Vector2 y)
{
  return dax::make_Vector2(Max(x[0], y[0]), Max(x[1], y[1]));
}
DAX_EXEC_CONT_EXPORT dax::Vector3 Max(dax::Vector3 x, dax::Vector3 y)
{
  return dax::make_Vector3(Max(x[0], y[0]), Max(x[1], y[1]), Max(x[2], y[2]));
}
DAX_EXEC_CONT_EXPORT dax::Vector4 Max(dax::Vector4 x, dax::Vector4 y)
{
  return dax::make_Vector4(Max(x[0], y[0]),
                           Max(x[1], y[1]),
                           Max(x[2], y[2]),
                           Max(x[3], y[3]));
}
DAX_EXEC_CONT_EXPORT dax::Id Max(dax::Id x, dax::Id y)
{
  return (x < y) ? y : x;
}
DAX_EXEC_CONT_EXPORT dax::Id3 Max(dax::Id3 x, dax::Id3 y)
{
  return dax::make_Id3(Max(x[0], y[0]), Max(x[1], y[1]), Max(x[2], y[2]));
}

//-----------------------------------------------------------------------------
/// Returns \p x or \p y, whichever is smaller.
///
DAX_EXEC_CONT_EXPORT dax::Scalar Min(dax::Scalar x, dax::Scalar y)
{
#ifdef DAX_USE_BOOST_MATH
  return (x < y)? y : x;
#else
  return DAX_SYS_MATH_FUNCTION(fmin)(x,y);
#endif
}
DAX_EXEC_CONT_EXPORT dax::Vector2 Min(dax::Vector2 x, dax::Vector2 y)
{
  return dax::make_Vector2(Min(x[0], y[0]), Min(x[1], y[1]));
}
DAX_EXEC_CONT_EXPORT dax::Vector3 Min(dax::Vector3 x, dax::Vector3 y)
{
  return dax::make_Vector3(Min(x[0], y[0]), Min(x[1], y[1]), Min(x[2], y[2]));
}
DAX_EXEC_CONT_EXPORT dax::Vector4 Min(dax::Vector4 x, dax::Vector4 y)
{
  return dax::make_Vector4(Min(x[0], y[0]),
                           Min(x[1], y[1]),
                           Min(x[2], y[2]),
                           Min(x[3], y[3]));
}
DAX_EXEC_CONT_EXPORT dax::Id Min(dax::Id x, dax::Id y)
{
  return (x < y) ? x : y;
}
DAX_EXEC_CONT_EXPORT dax::Id3 Min(dax::Id3 x, dax::Id3 y)
{
  return dax::make_Id3(Min(x[0], y[0]), Min(x[1], y[1]), Min(x[2], y[2]));
}

}
} // namespace dax::math

#endif //__dax_math_Compare_h
