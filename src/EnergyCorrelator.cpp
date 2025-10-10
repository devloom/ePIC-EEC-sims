#include <sstream> 
#include <cmath>
#include "EnergyCorrelator.h"


EnergyCorrelator::EnergyCorrelator(const Jet& jet) : jet(jet) {}

EnergyCorrelator::~EnergyCorrelator() {}

void EnergyCorrelator::FillHistograms() const
{
  // jet consituents
  const auto& constituents = jet.getConstituents();

  // jet energy
  float jetE = jet.get4vec().E();
  if (jetE <= 0){return;}
  float jetE2 = pow(jetE*jetE,2);
  float jetE3 = pow(jetE*jetE*jetE,2);

  // jet pt
  float jetPt = jet.get4vec().Pt();
  int ptBinIndex = HistoManager::GetPtBinIndex(jetPt);
  if (ptBinIndex < 0){return;}
  if (HistoManager::histograms.count("jetCounts")) 
  {
    ((TH1I*)HistoManager::histograms["jetCounts"])->Fill(ptBinIndex);
  }

  
  for (size_t i = 0; i < constituents.size(); ++i)
  {
    for (size_t j = 0; j < constituents.size(); ++j)
    {
      for (size_t k = 0; k < constituents.size(); ++k)
      {
        float deltaR_E3C_ij = ROOT::Math::VectorUtil::DeltaR(constituents[i].get4vec(), constituents[j].get4vec());
        float deltaR_E3C_jk = ROOT::Math::VectorUtil::DeltaR(constituents[j].get4vec(), constituents[k].get4vec());
        float deltaR_E3C_ik = ROOT::Math::VectorUtil::DeltaR(constituents[i].get4vec(), constituents[k].get4vec());
        
        float deltaR_E3C=std::max({deltaR_E3C_ij,deltaR_E3C_jk,deltaR_E3C_ik});

        float theta_E3C_ij = ROOT::Math::VectorUtil::Angle(constituents[i].get4vec(), constituents[j].get4vec());
        float theta_E3C_jk = ROOT::Math::VectorUtil::Angle(constituents[j].get4vec(), constituents[k].get4vec());
        float theta_E3C_ik = ROOT::Math::VectorUtil::Angle(constituents[i].get4vec(), constituents[k].get4vec());
        
        float theta_E3C = std::max({theta_E3C_ij,theta_E3C_jk,theta_E3C_ik});

        float weight_E3C = pow(constituents[i].get4vec().E()*constituents[j].get4vec().E()*constituents[k].get4vec().E(),2)/jetE3;

        if (HistoManager::histograms.count("deltaR_vs_weight_E3C")) 
        {
          ((TH2D*)HistoManager::histograms["deltaR_vs_weight_E3C"])->Fill(deltaR_E3C, weight_E3C);
        }

        if (HistoManager::histograms.count("weight_E3C")) 
        {
          ((TH1D*)HistoManager::histograms["weight_E3C"])->Fill(weight_E3C);
        }
        
        if (HistoManager::histograms.count("deltaR_E3C")) 
        {
          ((TH1D*)HistoManager::histograms["deltaR_E3C"])->Fill(deltaR_E3C);
        }
        if (ptBinIndex >= 0) 
        {
          // std::ostringstream key_E3C;
          // key_E3C << "E3C_pt_bin_" << HistoManager::ptBinEdges[ptBinIndex] << "_" << HistoManager::ptBinEdges[ptBinIndex + 1];
          std::ostringstream key_E3C;
          key_E3C << "E3C_pt_bin_" << static_cast<int>(HistoManager::ptBinEdges[ptBinIndex]) 
          << "_" << static_cast<int>(HistoManager::ptBinEdges[ptBinIndex + 1]);

          if (HistoManager::histograms.count(key_E3C.str())) 
          {
            ((TH1D*)HistoManager::histograms[key_E3C.str()])->Fill(deltaR_E3C, weight_E3C);
          }

          std::ostringstream key_theta_E3C;
          key_theta_E3C << "E3C_theta_pt_bin_" << static_cast<int>(HistoManager::ptBinEdges[ptBinIndex]) 
          << "_" << static_cast<int>(HistoManager::ptBinEdges[ptBinIndex + 1]);

          if (HistoManager::histograms.count(key_theta_E3C.str())) 
          {
            ((TH1D*)HistoManager::histograms[key_theta_E3C.str()])->Fill(theta_E3C, weight_E3C);
          }
        }
      }
      
      float deltaR = ROOT::Math::VectorUtil::DeltaR(constituents[i].get4vec(), constituents[j].get4vec());
      float theta = ROOT::Math::VectorUtil::Angle(constituents[i].get4vec(), constituents[j].get4vec());

      float weight = pow(constituents[i].get4vec().E()*constituents[j].get4vec().E(),2)/jetE2;

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

        std::ostringstream key_theta;
        key_theta << "eec_theta_pt_bin_" << HistoManager::ptBinEdges[ptBinIndex] << "_" << HistoManager::ptBinEdges[ptBinIndex + 1];
        if (HistoManager::histograms.count(key_theta.str())) 
        {
          ((TH1D*)HistoManager::histograms[key_theta.str()])->Fill(theta,weight);
          }
        }
    }
  }
}
