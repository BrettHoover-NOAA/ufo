/*
 * (C) Copyright 2017-2021 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_RTTOVCPP_OBSRADIANCERTTOVCPPTLAD_H_
#define UFO_RTTOVCPP_OBSRADIANCERTTOVCPPTLAD_H_

#include <ostream>
#include <string>
#include <vector>

#include "oops/base/Variables.h"
#include "oops/util/Logger.h"
#include "oops/util/ObjectCounter.h"
#include "ufo/LinearObsOperatorBase.h"

#include "rttov/wrapper/RttovSafe.h"

namespace eckit {
  class Configuration;
  class LocalConfiguration;
}

namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;
  class ObsDiagnostics;

// -----------------------------------------------------------------------------
/// RadianceRTTOV TL/AD observation operator class
class ObsRadianceRTTOVCPPTLAD : public LinearObsOperatorBase,
                   private util::ObjectCounter<ObsRadianceRTTOVCPPTLAD> {
 public:
  static const std::string classname() {return "ufo::ObsRadianceRTTOVCPPTLAD";}

  ObsRadianceRTTOVCPPTLAD(const ioda::ObsSpace &, const eckit::Configuration &);
  virtual ~ObsRadianceRTTOVCPPTLAD();

  // Calculate Jacobian H(x_g) of obs operator
  void setTrajectory(const GeoVaLs &, ObsDiagnostics &) override;
  // Calculate dy = H dx
  void simulateObsTL(const GeoVaLs &, ioda::ObsVector &) const override;
  // Calculate H^T dy
  void simulateObsAD(GeoVaLs &, const ioda::ObsVector &) const override;

// Other: declare variable function with return type of oops:Variables
  const oops::Variables & requiredVars() const override {return varin_;}

 private:
  void print(std::ostream &) const override;
  oops::Variables varin_;
  std::string        CoefFileName;
  std::vector<int>   channels_;
  mutable std::size_t        nlevels;  // need this in order to allocate dx
  std::vector<bool>  skip_profile;

// Declare a RttovSafe object for one single sensor
  mutable rttov::RttovSafe  aRttov_ = rttov::RttovSafe();
};

// -----------------------------------------------------------------------------

}  // namespace ufo
#endif  // UFO_RTTOVCPP_OBSRADIANCERTTOVCPPTLAD_H_
