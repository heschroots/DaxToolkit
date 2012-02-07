/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/
#ifndef __dax_exec_CellVoxel_h
#define __dax_exec_CellVoxel_h

#include <dax/Types.h>
#include <dax/internal/GridTopologys.h>

#include <dax/exec/Field.h>

namespace dax { namespace exec {

/// All cell objects are expected to have the following methods defined:
///   Cell<type>(work);
///   GetNumberOfPoints() const;
///   GetPoint(index) const;
///   GetPoint(index, field) const;

/// A cell in a regular structured grid.
class CellVoxel
{
public:
  typedef dax::internal::TopologyUniform TopologyType;

private:
  const TopologyType GridTopology;
  dax::Id CellIndex;

public:
  /// Create a cell for the given work.
  DAX_EXEC_CONT_EXPORT CellVoxel(const TopologyType &gs,
                                 dax::Id index)
    : GridTopology(gs), CellIndex(index) { }

  /// Get the number of points in the cell.
  DAX_EXEC_EXPORT dax::Id GetNumberOfPoints() const
  {
    return 8;
  }

  /// Given a vertex index for a point (0 to GetNumberOfPoints() - 1), returns
  /// the index for the point in point space.
  DAX_EXEC_EXPORT dax::Id GetPointIndex(const dax::Id vertexIndex) const
  {
    dax::Id3 ijkCell = dax::flatIndexToIndex3Cell(
          this->GetIndex(),
          this->GetGridTopology().Extent);

    const dax::Id3 cellVertexToPointIndex[8] = {
      dax::make_Id3(0, 0, 0),
      dax::make_Id3(1, 0, 0),
      dax::make_Id3(1, 1, 0),
      dax::make_Id3(0, 1, 0),
      dax::make_Id3(0, 0, 1),
      dax::make_Id3(1, 0, 1),
      dax::make_Id3(1, 1, 1),
      dax::make_Id3(0, 1, 1)
    };

    dax::Id3 ijkPoint = ijkCell + cellVertexToPointIndex[vertexIndex];

    dax::Id pointIndex = index3ToFlatIndex(ijkPoint,
                                           this->GetGridTopology().Extent);

    return pointIndex;
  }

  /// Get the origin (the location of the point at grid coordinates 0,0,0).
  DAX_EXEC_EXPORT const dax::Vector3 &GetOrigin() const
  {
    return this->GetGridTopology().Origin;
  }

  /// Get the spacing (the distance between grid points in each dimension).
  DAX_EXEC_EXPORT const dax::Vector3 &GetSpacing() const
  {
    return this->GetGridTopology().Spacing;
  }

  /// Get the extent of the grid in which this cell resides.
  DAX_EXEC_EXPORT const dax::Extent3 &GetExtent() const
  {
    return this->GetGridTopology().Extent;
  }

  /// Get the cell index.  Probably only useful internally.
  DAX_EXEC_EXPORT dax::Id GetIndex() const { return this->CellIndex; }

  /// Change the cell id.  (Used internally.)
  DAX_EXEC_EXPORT void SetIndex(dax::Id cellIndex)
  {
    this->CellIndex = cellIndex;
  }

  /// Get the grid structure details.  Only useful internally.
  DAX_EXEC_EXPORT
  const TopologyType &GetGridTopology() const
  {
    return this->GridTopology;
  }
};

}}
#endif
