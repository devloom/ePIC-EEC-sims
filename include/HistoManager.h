#ifndef __HISTO_MANAGER_H__
#define __HISTO_MANAGER_H__

#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <map>
#include <string>
#include <vector>
#include <sstream>

namespace HistoManager 
{
  extern std::map<std::string, TH1*> histograms;
  extern std::vector<float> ptBinEdges;

  void InitializeHistograms(const std::vector<float>& edges);
  int GetPtBinIndex(float pt);
  void WriteHistogramsToFile();
  void Cleanup();
}

#endif