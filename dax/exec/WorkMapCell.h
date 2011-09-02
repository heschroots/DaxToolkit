/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/
#ifndef __dax_exec_WorkMapCell_h
#define __dax_exec_WorkMapCell_h

#include <dax/Types.h>
#include <dax/exec/Cell.h>
#include <dax/exec/Field.h>

#include <dax/internal/GridStructures.h>
#include <dax/exec/internal/FieldAccess.h>

namespace dax {
namespace exec {

///----------------------------------------------------------------------------
// Work for worklets that map points to cell. Use this work when the worklets
// need "CellArray" information i.e. information about what points form a cell.
// There are different versions for different cell types, which might have
// different constructors because they identify topology differently.
template<class CellType> class WorkMapCell;

template<>
class WorkMapCell<dax::exec::CellVoxel>
{
private:
  dax::exec::CellVoxel Cell;

public:
  typedef CellVoxel CellType;

  __device__ WorkMapCell(const dax::internal::StructureUniformGrid &gridStructure,
                         dax::Id cellIndex)
    : Cell(gridStructure, cellIndex) { }

  __device__ const dax::exec::CellVoxel GetCell() const { return this->Cell; }

  template<typename T>
  __device__ const T &GetFieldValue(const dax::exec::FieldCell<T> &field) const
  {
    return dax::exec::internal::fieldAccessNormalGet(field,
                                                     this->GetCellIndex());
  }

  template<typename T>
  __device__ void SetFieldValue(dax::exec::FieldCell<T> &field, const T &value)
  {
    dax::exec::internal::fieldAccessNormalSet(field,
                                              this->GetCellIndex(),
                                              value);
  }

  template<typename T>
  __device__ const T &GetFieldValue(const dax::exec::FieldPoint<T> &field,
                                    dax::Id vertexIndex) const
  {
    dax::Id pointIndex = this->GetCell().GetPointIndex(vertexIndex);
    return dax::exec::internal::fieldAccessNormalGet(field, pointIndex);
  }

  __device__ dax::Vector3 GetFieldValue(
    const dax::exec::FieldCoordinates &, dax::Id vertexIndex) const
  {
    dax::Id pointIndex = this->GetCell().GetPointIndex(vertexIndex);
    const dax::internal::StructureUniformGrid &gridStructure
        = this->GetCell().GetGridStructure();
    return
        dax::exec::internal::fieldAccessUniformCoordinatesGet(gridStructure,
                                                              pointIndex);
  }

  __device__ dax::Id GetCellIndex() const { return this->Cell.GetIndex(); }

  __device__ void SetCellIndex(dax::Id cellIndex)
  {
    this->Cell.SetIndex(cellIndex);
  }
};


}
}

#endif //__dax_exec_WorkMapCell_h