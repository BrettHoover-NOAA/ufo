/*
 * (C) Copyright 2019 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "ufo/filters/obsfunctions/ObsFunctionBase.h"

#include <map>
#include <string>

#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"

namespace ufo {

// -----------------------------------------------------------------------------

ObsFunctionFactory::ObsFunctionFactory(const std::string & name) {
  if (getMakers().find(name) != getMakers().end()) {
    oops::Log::error() << name << " already registered in ufo::ObsFunctionFactory." << std::endl;
    ABORT("Element already registered in ufo::ObsFunctionFactory.");
  }
  getMakers()[name] = this;
}

// -----------------------------------------------------------------------------

ObsFunctionBase * ObsFunctionFactory::create(const Variable & var) {
  oops::Log::trace() << "ObsFunctionBase::create starting" << std::endl;
  typename std::map<std::string, ObsFunctionFactory*>::iterator jloc =
     getMakers().find(var.variable());
  if (jloc == getMakers().end()) {
    oops::Log::error() << var.variable() << " does not exist in ufo::ObsFunctionFactory."
                       << std::endl;
    ABORT("Element does not exist in ufo::ObsFunctionFactory.");
  }
  ObsFunctionBase * ptr = jloc->second->make(var.options());
  oops::Log::trace() << "ObsFunctionBase::create done" << std::endl;
  return ptr;
}

// -----------------------------------------------------------------------------

bool ObsFunctionFactory::functionExists(const std::string & name) {
  return (getMakers().find(name) != getMakers().end());
}

// -----------------------------------------------------------------------------

}  // namespace ufo
