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
#ifndef __dax_cont_arg_FieldArrayHandleConstant_h
#define __dax_cont_arg_FieldArrayHandleConstant_h

#include <dax/cont/arg/FieldArrayHandle.h>
#include <dax/cont/ArrayHandleConstant.h>


namespace dax { namespace cont { namespace arg {

/// \headerfile FieldArrayHandle.h dax/cont/arg/FieldArrayHandle.h
/// \brief Map array of a single constant value to \c Field worklet parameters.
template <typename Tags, typename T, typename Device>
class ConceptMap< Field(Tags), dax::cont::ArrayHandleConstant<T, Device> >
{
  typedef dax::cont::ArrayHandleConstant<T, Device> HandleType;
  typedef typename HandleType::PortalConstExecution  PortalType;

public:
  // Arrays are generally used for all types of fields.
  typedef dax::cont::sig::AnyDomain DomainTag;
  typedef dax::exec::arg::FieldPortal<T,Tags,PortalType> ExecArg;

  ConceptMap(HandleType handle):
    Handle(handle),
    Portal()
    {}

  DAX_CONT_EXPORT ExecArg GetExecArg()
    {
    return ExecArg(this->Portal);
    }

  DAX_CONT_EXPORT void ToExecution(dax::Id, boost::true_type,  boost::false_type)
    { /* Input  */
    this->Portal = this->Handle.PrepareForInput();
    }

  //we need to pass the number of elements to allocate
  DAX_CONT_EXPORT void ToExecution(dax::Id size)
    {
    ToExecution(size,typename Tags::template Has<dax::cont::sig::In>(),
           typename Tags::template Has<dax::cont::sig::Out>());
    }

  DAX_CONT_EXPORT dax::Id GetDomainLength(sig::Domain) const
    {
    //determine the proper work count be seing if we are being used
    //as input or output
    return this->Handle.GetNumberOfValues();
    }

private:
  HandleType Handle;
  PortalType Portal;
};

/// \headerfile FieldArrayHandle.h dax/cont/arg/FieldArrayHandle.h
/// \brief Map array of a single constant value to \c Field worklet parameters.
template <typename Tags, typename T, typename Device>
class ConceptMap< Field(Tags), const dax::cont::ArrayHandleConstant<T, Device> >
{
  typedef dax::cont::ArrayHandleConstant<T, Device> HandleType;
  typedef typename HandleType::PortalConstExecution  PortalType;

public:
  // Arrays are generally used for all types of fields.
  typedef dax::cont::sig::AnyDomain DomainTag;
  typedef dax::exec::arg::FieldPortal<T,Tags,PortalType> ExecArg;

  ConceptMap(HandleType handle):
    Handle(handle),
    Portal()
    {}

  DAX_CONT_EXPORT ExecArg GetExecArg()
    {
    return ExecArg(this->Portal);
    }

  DAX_CONT_EXPORT void ToExecution(dax::Id, boost::true_type,  boost::false_type)
    { /* Input  */
    this->Portal = this->Handle.PrepareForInput();
    }

  //we need to pass the number of elements to allocate
  DAX_CONT_EXPORT void ToExecution(dax::Id size)
    {
    ToExecution(size,typename Tags::template Has<dax::cont::sig::In>(),
           typename Tags::template Has<dax::cont::sig::Out>());
    }

  DAX_CONT_EXPORT dax::Id GetDomainLength(sig::Domain) const
    {
    //determine the proper work count be seing if we are being used
    //as input or output
    return this->Handle.GetNumberOfValues();
    }

private:
  HandleType Handle;
  PortalType Portal;
};

} } } //namespace dax::cont::arg

#endif //__dax_cont_arg_FieldArrayHandleConstant_h
