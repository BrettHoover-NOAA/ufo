/*
 * (C) Copyright 2023 NOAA/NWS/NCEP/EMC
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "ufo/filters/obsfunctions/AircraftBiasCorrectionConstant.h"

#include <algorithm>
#include <cmath>
#include <valarray>
#include <vector>

#include "ioda/ObsDataVector.h"
#include "oops/util/missingValues.h"
#include "ufo/filters/ObsFilterData.h"
#include "ufo/filters/Variable.h"
#include "ufo/utils/Constants.h"

namespace ufo {

static ObsFunctionMaker<AircraftBiasCorrectionConstant> makerObsFuncAircraftBiasCorrectionConstant_("AircraftBiasCorrectionConstant");

// -----------------------------------------------------------------------------

AircraftBiasCorrectionConstant::AircraftBiasCorrectionConstant(const eckit::LocalConfiguration & conf)
  : invars_() {
  oops::Log::debug() << "AircraftBiasCorrectionConstant: config = " << conf << std::endl;
  // Initialize options
  options_.deserialize(conf);

  // We need to retrieve the aircraft bias correction constant coefficient
  invars_ += Variable("BiasCoefficientValue/constantPredictor");

  // Typical use would be HofX group, but during testing, we include option for GsiHofX
  //BTH//std::string test_hofx = options_.test_hofx.value();
  //BTH//invars_ += Variable(test_hofx + "/windEastward");
  //BTH//invars_ += Variable(test_hofx + "/windNorthward");

  // TODO(gthompsn): Need to include a check that whatever HofX group name used actually exists.
}

// -----------------------------------------------------------------------------

AircraftBiasCorrectionConstant::~AircraftBiasCorrectionConstant() {}

// -----------------------------------------------------------------------------

void AircraftBiasCorrectionConstant::compute(const ObsFilterData & in,
                                  ioda::ObsDataVector<float> & out) const {
  const size_t nlocs = in.nlocs();
  const float missing = util::missingValue(missing);
  //BTH//const double deg = Constants::rad2deg;

  // Ensure that only one output variable is expected.
  ASSERT(out.nvars() == 1);

  // Retrieve minimum_uv value and assure it is sensible.
  //BTH//const float min_uv = std::max(0.0001f, options_.minimum_uv.value());

  // Retrieve observation aircraft bias correction constant coefficient
  std::vector<float> BCcoeff
  in.get(Variable("BiasCoefficientValue/constantPredictor"), BCcoeff);
  // Retrieve Model HofX wind components
  //BTH//std::string test_hofx = options_.test_hofx.value();
  //BTH//std::vector<float> um, vm;
  //BTH//in.get(Variable(test_hofx + "/windEastward"), um);
  //BTH//in.get(Variable(test_hofx + "/windNorthward"), vm);

  for (size_t jj = 0; jj < nlocs; ++jj) {
    if (BCcoeff[jj] != missing) {
      // constant predictor = 1.0*BCcoeff = BCcoeff
      out[0][jj] = BCcoeff[jj];
    } else {
      out[0][jj] = missing;
    }
  }
}

// -----------------------------------------------------------------------------

const ufo::Variables & AircraftBiasCorrectionConstant::requiredVariables() const {
  return invars_;
}

// -----------------------------------------------------------------------------

}  // namespace ufo
