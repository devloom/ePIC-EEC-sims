#include "Jet.h"

Jet::Jet() : jet4vec(0.0, 0.0, 0.0, 0.0) {}

Jet::~Jet() {}

void Jet::setConstituents(const std::vector<Particle>& v_part)
{
  constituents = v_part;
  jet4vec = LorentzVector(0.0, 0.0, 0.0, 0.0);
  for (const auto& part : v_part) 
  {
    jet4vec.SetPxPyPzE(jet4vec.Px()+part.get4vec().Px(),jet4vec.Py()+part.get4vec().Py(),
                jet4vec.Pz()+part.get4vec().Pz(),jet4vec.E()+part.get4vec().E());
  }
}

void Jet::addConstituent(const Particle& part)
{
  constituents.push_back(part);
  jet4vec.SetPxPyPzE(jet4vec.Px()+part.get4vec().Px(),jet4vec.Py()+part.get4vec().Py(),
                jet4vec.Pz()+part.get4vec().Pz(),jet4vec.E()+part.get4vec().E());
}

