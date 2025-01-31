#include "Particle.h"

Particle::Particle() : pid(0), part4vec(0.0, 0.0, 0.0, 0.0) {}

Particle::Particle(int pid, const LorentzVector& part4vec)
    : pid(pid), part4vec(part4vec) {}

Particle::Particle(int pid, float px, float py, float pz, float E)
    : pid(pid), part4vec(px, py, pz, E) {}

Particle::~Particle() {}