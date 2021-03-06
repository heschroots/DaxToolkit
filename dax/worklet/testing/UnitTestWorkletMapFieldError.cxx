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

#include <dax/worklet/testing/FieldMapError.h>

#include <dax/VectorTraits.h>

#include <dax/cont/ArrayContainerControl.h>
#include <dax/cont/DeviceAdapter.h>
#include <dax/cont/DispatcherMapField.h>
#include <dax/cont/ErrorExecution.h>
#include <dax/cont/UniformGrid.h>

#include <dax/cont/testing/Testing.h>

#include <vector>

namespace {

const dax::Id DIM = 8;

//-----------------------------------------------------------------------------
static void TestFieldMapError()
{
  std::vector<dax::Scalar> array(DIM);
  dax::cont::ArrayHandle<dax::Scalar> arrayHandle =
      dax::cont::make_ArrayHandle(array);

  std::cout << "Running field map worklet that errors" << std::endl;
  bool gotError = false;
  try
    {
    dax::cont::DispatcherMapField<
        dax::worklet::testing::FieldMapError >().Invoke( arrayHandle );
    }
  catch (dax::cont::ErrorExecution error)
    {
    std::cout << "Got expected ErrorExecution object." << std::endl;
    std::cout << error.GetMessage() << std::endl;
    gotError = true;
    }

  DAX_TEST_ASSERT(gotError, "Never got the error thrown.");
}

} // Anonymous namespace

//-----------------------------------------------------------------------------
int UnitTestWorkletMapFieldError(int, char *[])
{
  return dax::cont::testing::Testing::Run(TestFieldMapError);
}
