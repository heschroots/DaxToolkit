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

#include <dax/cont/testing/TestingGridGenerator.h>
#include <dax/cont/testing/Testing.h>

#include <dax/worklet/MarchingTetrahedra.h>

#include <iostream>
//temporary includes
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <dax/CellTag.h>
#include <dax/CellTraits.h>
#include <dax/TypeTraits.h>

#include <dax/cont/ArrayHandle.h>
#include <dax/cont/DispatcherGenerateInterpolatedCells.h>
#include <dax/cont/DispatcherMapCell.h>
#include <dax/cont/UniformGrid.h>
#include <dax/cont/UnstructuredGrid.h>

#include <vector>

namespace {
const dax::Id DIM = 26;
const dax::Id ISOVALUE = 70;

//-----------------------------------------------------------------------------
struct TestMarchingTetrahedraWorklet
{
  typedef dax::cont::ArrayContainerControlTagBasic ArrayContainer;
  typedef DAX_DEFAULT_DEVICE_ADAPTER_TAG DeviceAdapter;

  typedef dax::CellTagTriangle CellType;

  typedef dax::cont::UnstructuredGrid<
      CellType,ArrayContainer,ArrayContainer,DeviceAdapter>
      UnstructuredGridType;

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  template<class InputGridType>
  DAX_CONT_EXPORT
  void operator()(const InputGridType&) const
    {
    dax::cont::testing::TestGrid<InputGridType,ArrayContainer,DeviceAdapter>
        inGrid(DIM);
    UnstructuredGridType outGrid;

    dax::Vector3 trueGradient = dax::make_Vector3(1.0, 1.0, 1.0);
    dax::Id numPoints = inGrid->GetNumberOfPoints();
    std::vector<dax::Scalar> field(numPoints);
    for (dax::Id pointIndex = 0; pointIndex < numPoints; pointIndex++)
      {
      dax::Vector3 coordinates = inGrid.GetPointCoordinates(pointIndex);
      field[pointIndex] = dax::dot(coordinates, trueGradient);
      }

    dax::cont::ArrayHandle<dax::Scalar,ArrayContainer,DeviceAdapter>
        fieldHandle = dax::cont::make_ArrayHandle(field,
                                                  ArrayContainer(),
                                                  DeviceAdapter());

    const dax::Scalar isoValue = ISOVALUE;

    try
      {
      typedef dax::cont::ArrayHandle<dax::Id, ArrayContainer, DeviceAdapter>
                    CountHandleType;

      //construct the two dispatcher that will be used to do the marching cubes
      typedef  dax::cont::DispatcherMapCell<
                          dax::worklet::MarchingTetrahedraCount > CellDispatcher;
      typedef  dax::cont::DispatcherGenerateInterpolatedCells<
                  dax::worklet::MarchingTetrahedraGenerate > InterpolatedDispatcher;

      //run the first step
      CountHandleType count; //array handle for the first step count
      CellDispatcher cellDispatcher( (dax::worklet::MarchingTetrahedraCount(isoValue)) );
      cellDispatcher.Invoke( inGrid.GetRealGrid(),
                             fieldHandle,
                             count);

      //construct the topology generation worklet
      InterpolatedDispatcher interpDispatcher( count,
                              dax::worklet::MarchingTetrahedraGenerate(isoValue) );

      interpDispatcher.SetRemoveDuplicatePoints(false);
      //so we can use the count again
      interpDispatcher.SetReleaseCount(false);

      //run the second step
      interpDispatcher.Invoke(inGrid.GetRealGrid(),
                              outGrid,
                              fieldHandle);

      const dax::Id valid_num_points =
            dax::CellTraits<CellType>::NUM_VERTICES * outGrid.GetNumberOfCells();
      DAX_TEST_ASSERT(valid_num_points==outGrid.GetNumberOfPoints(),
                      "Incorrect number of points in the output grid when not merging");

      interpDispatcher.SetRemoveDuplicatePoints(true);
      //run the second step again with point merging

      UnstructuredGridType secondOutGrid;
      interpDispatcher.Invoke(inGrid.GetRealGrid(),
                              secondOutGrid,
                              fieldHandle);

      std::cout << "Number of ponits is " << secondOutGrid.GetNumberOfPoints() << std::endl;
      std::cout << "valid_num_points = " << valid_num_points << std::endl;

      //151 is the number I got from running the algorithm by hand and seeing
      //the number of unique points
      const dax::Id NumberOfUniquePoints = 151;
      DAX_TEST_ASSERT(NumberOfUniquePoints == secondOutGrid.GetNumberOfPoints() &&
                      NumberOfUniquePoints != valid_num_points,
          "We didn't merge to the correct number of points");
      }
    catch (dax::cont::ErrorControl error)
      {
      std::cout << "Got error: " << error.GetMessage() << std::endl;
      DAX_TEST_ASSERT(true==false,error.GetMessage());
      }
    }
};


//-----------------------------------------------------------------------------
void TestMarchingTetrahedra()
  {
  dax::cont::testing::GridTesting::TryAllGridTypes(
        TestMarchingTetrahedraWorklet(),
        dax::testing::Testing::CellCheckTetrahedron());
  }
} // Anonymous namespace

//-----------------------------------------------------------------------------
int UnitTestWorkletMarchingTetrahedra(int, char *[])
{
  return dax::cont::testing::Testing::Run(TestMarchingTetrahedra);
}
