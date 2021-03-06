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
#ifndef __dax_cont_ErrorControlOutOfMemory_h
#define __dax_cont_ErrorControlOutOfMemory_h

#include <dax/cont/ErrorControl.h>

namespace dax {
namespace cont {

/// This class is thrown when a Dax function or method tries to allocate an
/// array and fails.
///
class ErrorControlOutOfMemory : public ErrorControl
{
public:
  ErrorControlOutOfMemory(const std::string &message)
    : ErrorControl(message) { }
};

}
} // namespace dax::cont

#endif //__dax_cont_ErrorControlOutOfMemory_h
