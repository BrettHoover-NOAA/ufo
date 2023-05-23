/*
 * (C) Copyright 2023 NOAA/NWS/NCEP/EMC
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_FILTERS_OBSFUNCTIONS_AIRCRAFTBIASCORRECTIONCONSTANT_H_
#define UFO_FILTERS_OBSFUNCTIONS_AIRCRAFTBIASCORRECTIONCONSTANT_H_

#include <string>

#include "oops/util/parameters/Parameter.h"

#include "ufo/filters/obsfunctions/ObsFunctionBase.h"
#include "ufo/filters/Variables.h"

namespace ufo {
//BTH//
///
/// \brief An optional parameter to override the source of HofX wind components,
///        and an optional parameter for minimum wind components (default=0.5 m/s).
///
class AircraftBiasCorrectionConstantParameters : public oops::Parameters {
  OOPS_CONCRETE_PARAMETERS(AircraftBiasCorrectionConstantParameters, Parameters)
//BTH//
//BTH// public:
//BTH//  /// Name of the HofX group used to replace the default group (default is HofX)
//BTH//  oops::Parameter<std::string> test_hofx{"test_hofx", "HofX", this};
//BTH//  oops::Parameter<float> minimum_uv{"minimum_uv", 0.5, this};
};

// -----------------------------------------------------------------------------

/// \brief Compute the constant predictor for aircraft bias correction, which is
///        equal to 1.0 * <bias correction coefficient>. This can be combined with
///        the aircraft airTemperature and other predictors through use of a
///        LinearCombination filter to bias correct the airTemperature observations.
///
/// ~~~
///
/// ### Sample YAML configuration
///     - filter: Variable Assignment
///       filter variables:
///       - name: airTemperature
///       assignments:
///       - name: BiasCorrectionPredictor/constantPredictor
///         type: float
///       function:
///         name: ObsFunction/AircraftBiasCorrectionConstant
///
class AircraftBiasCorrectionConstant : public ObsFunctionBase<float> {
 public:
  static const std::string classname() {return "AircraftBiasCorrectionConstant";}

  explicit AircraftBiasCorrectionConstant(const eckit::LocalConfiguration &
                                 = eckit::LocalConfiguration());
  ~AircraftBiasCorrectionConstant();

  void compute(const ObsFilterData &, ioda::ObsDataVector<float> &) const;
  const ufo::Variables & requiredVariables() const;
 private:
  ufo::Variables invars_;
  AircraftBiasCorrectionConstantParameters options_;
};

// -----------------------------------------------------------------------------

}  // namespace ufo

#endif  // UFO_FILTERS_OBSFUNCTIONS_AIRCRAFTBIASCORRECTIONCONSTANT_H_
