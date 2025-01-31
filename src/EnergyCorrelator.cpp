#include <sstream> 

#include "EnergyCorrelator.h"


EnergyCorrelator::EnergyCorrelator(const Jet& jet) : jet(jet) {}

EnergyCorrelator::~EnergyCorrelator() {}

void EnergyCorrelator::FillHistograms() const
{
  // jet consituents
  const auto& constituents = jet.getConstituents();

  // jet energy
  float jetE = jet.get4vec().E();
  float jetE2 = jetE*jetE;

  // jet pt
  float jetPt = jet.get4vec().Pt();
  int ptBinIndex = HistoManager::GetPtBinIndex(jetPt);
  
  for (size_t i = 0; i < constituents.size(); ++i)
  {
    for (size_t j = i+1; j < constituents.size(); ++j)
    {
      float deltaR = ROOT::Math::VectorUtil::DeltaR(constituents[i].get4vec(), constituents[j].get4vec());
      float weight = (constituents[i].get4vec().E()*constituents[j].get4vec().E())/jetE2;

      if (HistoManager::histograms.count("deltaR_vs_weight")) 
      {
        ((TH2D*)HistoManager::histograms["deltaR_vs_weight"])->Fill(deltaR, weight);
      }
      if (HistoManager::histograms.count("deltaR")) 
      {
        ((TH1D*)HistoManager::histograms["deltaR"])->Fill(deltaR);
      }
      if (HistoManager::histograms.count("weight")) 
      {
        ((TH1D*)HistoManager::histograms["weight"])->Fill(weight);
      }
      if (ptBinIndex >= 0) 
      {
        std::ostringstream key;
        key << "eec_pt_bin_" << HistoManager::ptBinEdges[ptBinIndex] << "_" << HistoManager::ptBinEdges[ptBinIndex + 1];
        if (HistoManager::histograms.count(key.str())) 
        {
          ((TH1D*)HistoManager::histograms[key.str()])->Fill(deltaR, weight);
        }
      }
    }
  }
}