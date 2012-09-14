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
#ifndef __dax_openmp_cont_internal_DeviceAdapterAlgorithmOpenMP_h
#define __dax_openmp_cont_internal_DeviceAdapterAlgorithmOpenMP_h

#include <dax/openmp/cont/internal/SetThrustForOpenMP.h>

#include <dax/openmp/cont/internal/DeviceAdapterTagOpenMP.h>
#include <dax/openmp/cont/internal/ArrayManagerExecutionOpenMP.h>

// Here are the actual implementation of the algorithms.
#include <dax/thrust/cont/internal/DeviceAdapterAlgorithmThrust.h>

namespace dax {
namespace cont {
namespace internal {

namespace detail {

template<class FunctorType>
class ScheduleKernelOpenMP
{
public:
  DAX_CONT_EXPORT ScheduleKernelOpenMP(const FunctorType &functor)
    : Functor(functor)
  {  }

  DAX_CONT_EXPORT void SetErrorMessageBuffer(
      const dax::exec::internal::ErrorMessageBuffer &errorMessage)
  {
    this->ErrorMessage = errorMessage;
    this->Functor.SetErrorMessageBuffer(errorMessage);
  }

  DAX_EXEC_EXPORT void operator()(dax::Id index) const {
    // The OpenMP device adapter causes array classes to be shared between
    // control and execution environment. This means that it is possible for an
    // exception to be thrown even though this is typically not allowed.
    // Throwing an exception from here is bad because there are several
    // simultaneous threads running. Get around the problem by catching the
    // error and setting the message buffer as expected.
    try
      {
      this->Functor(index);
      }
    catch (dax::cont::Error error)
      {
      this->ErrorMessage.RaiseError(error.GetMessage().c_str());
      }
    catch (...)
      {
      this->ErrorMessage.RaiseError(
          "Unexpected error in execution environment.");
      }
  }

private:
  FunctorType Functor;
  dax::exec::internal::ErrorMessageBuffer ErrorMessage;
};

} // namespace detail

// Override the thrust version of Schedule to handle exceptions that can occur
// because we are running on a CPU.
template<class FunctorType>
DAX_CONT_EXPORT void LegacySchedule(
    FunctorType functor,
    dax::Id numInstances,
    dax::openmp::cont::DeviceAdapterTagOpenMP)
{
  dax::cont::internal::LegacySchedule(detail::ScheduleKernelOpenMP<FunctorType>(functor),
           numInstances,
           dax::thrust::cont::internal::DeviceAdapterTagThrust());
}

}
}
} // namespace dax::cont::internal

#endif //__dax_openmp_cont_internal_DeviceAdapterAlgorithmOpenMP_h
