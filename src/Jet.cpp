#include "Jet.h"

Jet::Jet() : jet4vec(0.0, 0.0, 0.0, 0.0) {}

Jet::~Jet() {}

void Jet::setConstituents(const std::vector<Particle>& v_part)
{
  constituents = v_part;
  jet4vec = LorentzVector(0.0, 0.0, 0.0, 0.0);
  for (const auto& part : v_part) 
  {
    // only hadronic constituents
    int pid = part.getPID();
    if (pid == 11 || pid == 22|| fabs(pid) == 2212 || fabs(pid) == 2112 || pid == 0)
    {
      continue;
    }
    jet4vec.SetPxPyPzE(jet4vec.Px()+part.get4vec().Px(),jet4vec.Py()+part.get4vec().Py(),
                jet4vec.Pz()+part.get4vec().Pz(),jet4vec.E()+part.get4vec().E());
  }
}

void Jet::addConstituent(const Particle& part)
{
  // only hadronic constituents
  int pid = part.getPID();
  if (pid == 11 || pid == 22|| fabs(pid) == 2212 || fabs(pid) == 2112 || pid == 0)
  {
    return;
  }
  constituents.push_back(part);
  jet4vec.SetPxPyPzE(jet4vec.Px()+part.get4vec().Px(),jet4vec.Py()+part.get4vec().Py(),
                jet4vec.Pz()+part.get4vec().Pz(),jet4vec.E()+part.get4vec().E());
}



