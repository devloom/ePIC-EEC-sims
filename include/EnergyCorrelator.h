#ifndef __ENERGYCORRELATOR_H__
#define __ENERGYCORRELATOR_H__

#include <vector>

#include "Math/PxPyPzE4D.h"
#include "Math/VectorUtil.h"
#include "TH1D.h"

#include "Particle.h"
#include "Jet.h"
#include "HistoManager.h"

class Jet;

class EnergyCorrelator 
{
public:

  EnergyCorrelator(const Jet &jet);
  virtual ~EnergyCorrelator();
  void FillHistograms() const;

private:
  const Jet& jet;

};

#endif