#include "HistoManager.h"

namespace HistoManager {

  inline std::map<std::string, TH1*> histograms;
  inline std::vector<float> ptBinEdges;
  
  void InitializeHistograms(const std::vector<float>& edges) 
  {
    int bins[7]={200,100,75,50,30,6,35};
    ptBinEdges = edges;
    histograms["jetCounts"] = new TH1I("jetCounts","jetCounts",ptBinEdges.size(),0,ptBinEdges.size());
    histograms["weight"] = new TH1D("weight", "weight", 100, 0, 0.25);
    histograms["weight_E3C"] = new TH1D("weight_E3C", "weight_E3C", 100, 0, 0.25);
    histograms["deltaR"] = new TH1D("deltaR", "DeltaR", 100, 0, 1);
    histograms["deltaR_E3C"] = new TH1D("deltaR_E3C", "DeltaR_E3C", 100, 0, 1);
    histograms["deltaR_vs_weight"] = new TH2D("deltaR_vs_weight", "DeltaR vs Weight", 100, 0, 1, 100, 0, 0.25);
    histograms["deltaR_vs_weight_E3C"] = new TH2D("deltaR_vs_weight_E3C", "DeltaR vs Weight_E3C", 100, 0, 1, 100, 0, 0.25);
   
    for (size_t i = 0; i < ptBinEdges.size() - 1; ++i) {

      std::ostringstream histName;
      histName << "eec_pt_bin_" << ptBinEdges[i] << "_" << ptBinEdges[i + 1];
      histograms[histName.str()] = new TH1D(histName.str().c_str(), histName.str().c_str(), bins[i], 0.0005, 1);

      std::ostringstream histName_E3C;
      histName_E3C << "E3C_pt_bin_" << static_cast<int>(ptBinEdges[i]) << "_" << static_cast<int>(ptBinEdges[i + 1]);
      histograms[histName_E3C.str()] = new TH1D(histName_E3C.str().c_str(), histName_E3C.str().c_str(), bins[i], 0.0005, 1);

    }
  }

  int GetPtBinIndex(float pt) 
  {
    for (size_t i = 0; i < ptBinEdges.size() - 1; ++i) 
    {
      if (pt >= ptBinEdges[i] && pt < ptBinEdges[i + 1]) 
      {
        return i;
      }
    }
    return -1;
  }

  void WriteHistogramsToFile() 
  {
    for (auto& [name, hist] : histograms) 
    {
      hist->Write();
    }
  }
  
  void Cleanup() 
  {
    for (auto& [name, hist] : histograms) 
    {
        delete hist;
    }
    histograms.clear();
  }
}
