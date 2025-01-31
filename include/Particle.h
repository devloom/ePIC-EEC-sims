#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Math/PxPyPzE4D.h"

class Particle 
{
public:

  using LorentzVector = ROOT::Math::PxPyPzE4D<double>;

  Particle();
  Particle(int pid, const LorentzVector& part4vec);
  Particle(int pid, float px, float py, float pz, float E);
  virtual ~Particle();

  int getPID() const {return pid;};
  const LorentzVector& get4vec() const {return part4vec;};
private:
    int pid;
    LorentzVector part4vec;
};

#endif