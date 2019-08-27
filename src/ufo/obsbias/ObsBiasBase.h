/*
 * (C) Copyright 2017-2019 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_OBSBIAS_OBSBIASBASE_H_
#define UFO_OBSBIAS_OBSBIASBASE_H_

#include <map>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

#include "eckit/config/Configuration.h"

#include "oops/util/Printable.h"

#include "ufo/ObsBiasIncrement.h"

namespace oops {
  class Variables;
}

namespace ioda {
  class ObsVector;
  class ObsSpace;
}

namespace ufo {
  class GeoVaLs;

// -----------------------------------------------------------------------------
/// Base class for observation bias operators

class ObsBiasBase : public util::Printable,
                    private boost::noncopyable {
 public:
  explicit ObsBiasBase(const eckit::Configuration &) {}
  virtual ~ObsBiasBase() {}

/// I/O and diagnostics
  virtual void read(const eckit::Configuration &) = 0;
  virtual void write(const eckit::Configuration &) const = 0;
  virtual double norm() const = 0;
  virtual std::size_t size() const = 0;

/// Add increments
  virtual ObsBiasBase & operator+=(const ObsBiasIncrement &) = 0;

/// Bias model
  virtual void computeObsBias(const GeoVaLs &,
                              ioda::ObsVector &,
                              const ioda::ObsSpace &) const = 0;

/// Bias operator input required from Model
  virtual const oops::Variables & variables() const = 0;

/// Bias parameters interface
  virtual double & operator[](const unsigned int) = 0;
  virtual const double & operator[](const unsigned int) const = 0;

 private:
  virtual void print(std::ostream &) const = 0;
};

// -----------------------------------------------------------------------------

/// Observation bias operator Factory
class ObsBiasFactory {
 public:
  static ObsBiasBase * create(const eckit::Configuration &);
  virtual ~ObsBiasFactory() { getMakers().clear(); }

 protected:
  explicit ObsBiasFactory(const std::string &);

 private:
  virtual ObsBiasBase * make(const eckit::Configuration &) = 0;
  static std::map < std::string, ObsBiasFactory * > & getMakers() {
    static std::map < std::string, ObsBiasFactory * > makers_;
    return makers_;
  }
};

// -----------------------------------------------------------------------------

template<class T>
class ObsBiasMaker : public ObsBiasFactory {
  virtual ObsBiasBase * make(const eckit::Configuration & conf)
    { return new T(conf); }
 public:
  explicit ObsBiasMaker(const std::string & name) : ObsBiasFactory(name) {}
};

// -----------------------------------------------------------------------------

}  // namespace ufo

#endif  // UFO_OBSBIAS_OBSBIASBASE_H_