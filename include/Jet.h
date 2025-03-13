#ifndef __JET_H__
#define __JET_H__

#include <vector>
#include "Particle.h"

class Jet 
{
public:

  using LorentzVector = ROOT::Math::PxPyPzE4D<double>;

  Jet();
  virtual ~Jet();
  
  void setConstituents(const std::vector<Particle>& v_part);
  void addConstituent(const Particle& part);

  float getR() const {return R;};
  const LorentzVector& get4vec() const {return jet4vec;};
  const std::vector<Particle>& getConstituents() const {return constituents;};
  int getNConstituents() const {return constituents.size();};
  
private:
    std::vector<Particle> constituents;
    const float R = 1.0;
    LorentzVector jet4vec;
};

#endif