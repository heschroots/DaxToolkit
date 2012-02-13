/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <dax/internal/GridTopologys.h>
#include <dax/exec/Cell.h>

#include <dax/internal/Testing.h>
#include <vector>

namespace {

template<typename Grid>
static void TestGridSize(const Grid &gridstruct,
                         dax::Id numPoints,
                         dax::Id numCells)
{
  dax::Id computedNumPoints = dax::internal::numberOfPoints(gridstruct);
  DAX_TEST_ASSERT(computedNumPoints == numPoints,
                  "Structured grid returned wrong number of points");

  dax::Id computedNumCells = dax::internal::numberOfCells(gridstruct);
  DAX_TEST_ASSERT(computedNumCells == numCells,
                  "Structured grid return wrong number of cells");
}

static void TestUniformGrid()
{
  std::cout << "Testing Structured grid size." << std::endl;

  dax::internal::TopologyUniform gridstruct;
  gridstruct.Origin = dax::make_Vector3(0.0, 0.0, 0.0);
  gridstruct.Spacing = dax::make_Vector3(1.0, 1.0, 1.0);

  gridstruct.Extent.Min = dax::make_Id3(0, 0, 0);
  gridstruct.Extent.Max = dax::make_Id3(10, 10, 10);
  TestGridSize(gridstruct, 1331, 1000);

  gridstruct.Extent.Min = dax::make_Id3(5, -9, 3);
  gridstruct.Extent.Max = dax::make_Id3(15, 6, 13);
  TestGridSize(gridstruct, 1936, 1500);
}

static void TestUnstructuredGrid()
{
  std::cout << "Testing Unstructured grid size." << std::endl;
  {
    dax::internal::TopologyUnstructured<dax::exec::CellHexahedron> ugrid;
    TestGridSize(ugrid,0,0);
  }


  //to simplify the process of creating a hexahedron unstrucutured
  //grid I am going to copy the ids and points from a uniform grid.
  dax::internal::TopologyUniform uniform;
  uniform.Origin = dax::make_Vector3(0.0, 0.0, 0.0);
  uniform.Spacing = dax::make_Vector3(1.0, 1.0, 0.0);

  //make the grid only contain 8 cells
  uniform.Extent.Min = dax::make_Id3(0, 0, 0);
  uniform.Extent.Max = dax::make_Id3(2, 2, 1);
  TestGridSize(uniform,18,4);

  //copy the point info over to the unstructured grid
  std::vector<dax::Vector3> points;
  for(dax::Id i=0; i <dax::internal::numberOfPoints(uniform); ++i)
    {
    points.push_back(dax::internal::pointCoordiantes(uniform,i));
    }

  //copy the cell topology information over
  std::vector<dax::Id> topology;
  for(dax::Id i=0; i <dax::internal::numberOfCells(uniform); ++i)
    {
    dax::exec::CellVoxel vox(uniform,i);
    for(dax::Id j=0; j < vox.GetNumberOfPoints(); ++j)
      {
      topology.push_back(vox.GetPointIndex(j));
      }
    }

  dax::internal::DataArray<dax::Vector3> rawPoints(&points[0],points.size());
  dax::internal::DataArray<dax::Id> rawTopo(&topology[0],topology.size());
  dax::internal::TopologyUnstructured<dax::exec::CellHexahedron> ugrid(
        rawPoints,rawTopo);
  TestGridSize(ugrid,18,4);
}

static void TestGridSizes()
{
  TestUniformGrid();
  TestUnstructuredGrid();
}

}

int UnitTestGridTopologys(int, char *[])
{
  return dax::internal::Testing::Run(TestGridSizes);
}