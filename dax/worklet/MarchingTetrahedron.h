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

#ifndef __MarchingTetrahedron_worklet_
#define __MarchingTetrahedron_worklet_

#include <dax/CellTag.h>
#include <dax/CellTraits.h>
#include <dax/exec/CellField.h>
#include <dax/exec/CellVertices.h>
#include <dax/exec/InterpolatedCellPoints.h>
#include <dax/exec/WorkletInterpolatedCell.h>
#include <dax/exec/WorkletMapCell.h>

#include <dax/worklet/internal/MarchingTetrahedronTable.h>

namespace dax{
namespace worklet{

namespace internal{
namespace marchingtetrahedron{
//------------------------------------------------------------------------------
template<typename T, typename U>
DAX_EXEC_EXPORT
int GetTetrahedronClassification(const T isovalue, const U& values)
{
    //TO DO: return unique tetrahedron configuration
    return;
}
}
}

//------------------------------------------------------------------------------
class MarchingTetrahedronCount : public dax::exec::WorkletMapCell
{
    //TO DO: Control and execution signatures

    DAX_CONT_EXPORT MarchingTetrahedronCount(dax::Scalar isoValue)
        : IsoValue(isoValue) {  }

    template<class CellTag>
    DAX_EXEC_EXPORT
    dax::Id operator()(
            const dax::exec::CellField<dax::Scalar, CellTag> &values) const
    {
        //TODO: define return value
        return;
    }

private:
    dax::Scalar IsoValue;
};

//------------------------------------------------------------------------------
class MarchingTetrahedronGenerate : public dax::exec::WorkletInterpolatedCell
{
    //TODO: define execution and control signatures
    DAX_CONT_EXPORT MarchingTetrahedronGenerate(dax::Scalar isoValue)
        : IsoValue(isoValue) {   }

    template<class CellTag>
    DAX_EXEC_EXPORT
    void operator()(//TODO: Define input parameters
                    )
    {

    }

private:
    dax::Scalar IsoValue;
};
}
}
#endif // __MarchingTetrahedron_worklet_
