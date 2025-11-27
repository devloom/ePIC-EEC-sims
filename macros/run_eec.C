#include "../include/HistoManager.h"
#include <sstream>
#include <cmath> 

R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

// 0 for truth, 1 for smeared
void run_eec(int runtype = 1)
{
    // =============== Eventually we can just store Jet objects into our trees directly from Pythia ========= //
    // ========== For now just read in the vector branches and we will manually make the jet objects ========== //
    // read pythia jets from file

    std::string s_runtype[6] = {"truth_ep","smeared_ep","truth_eD","smeared_eD","truth_eAu","smeared_eAu"};

    std::string inputPythia;
    if (runtype == 0)
    {
      inputPythia = "../../simulations/truth_ep_jets_5M_xB_genshd1.root";
    }
    else if (runtype == 1)
    {
      inputPythia = "../../simulations/smeared_ep_jets_5M_xB_genshd1.root";
    }
    else if (runtype == 2)
    {
      inputPythia = "../../simulations/truth_eD_jets.root";
    }
    else if (runtype == 3)
    {
      inputPythia = "../../simulations/smeared_jets_eD.root";
    }
    else if (runtype == 4)
    {
      inputPythia = "../../simulations/truth_eAu_jets_5M_xB_genshd1.root";
    }
    else if (runtype == 5)
    {
      inputPythia = "../../simulations/smeared_eAu_jets_5M_xB_genshd1.root";
    }
    else 
    {
      std::cout << "Please input 0 (truth) or 1 (smeared) for runtype. Exiting." << std::endl;
      return;
    }

    TFile *infilePythia = new TFile(inputPythia.c_str());
    if (!infilePythia || infilePythia->IsZombie()){std::cerr << "Could not open file." << std::endl;return;}

    // read jet tree
    TTree *jettree = nullptr;
    infilePythia->GetObject("jets", jettree);
    if (!jettree){std::cerr << "Could not find tree." << std::endl;infilePythia->Close();return;}

    int nConstituents;
    double jPx, jPy, jPz, jE, jPt;
    std::vector<int>* dtr_PID = nullptr;
    std::vector<double> *dtr_Px = nullptr, *dtr_Py = nullptr, *dtr_Pz = nullptr,  *dtr_E = nullptr;
	  jettree->SetBranchAddress("nConstituents", &nConstituents); 
	  jettree->SetBranchAddress("dtr_Px" , &dtr_Px);
	  jettree->SetBranchAddress("dtr_Py" , &dtr_Py);
	  jettree->SetBranchAddress("dtr_Pz" , &dtr_Pz);
	  jettree->SetBranchAddress("dtr_E" , &dtr_E);
	  jettree->SetBranchAddress("dtr_PID" , &dtr_PID);
    jettree->SetBranchAddress("jPt" , &jPt);
    // ================================================= //

    // Create histograms (output file must be declared before InitializeHistograms)
    TFile *outfile = new TFile(Form("test_outfile_%s_GenShd_1_R_1_n_05_5M.root",s_runtype[runtype].c_str()),"RECREATE");
    std::vector<float> ptbins = {5,10,15,20,30,30,35,40};
    HistoManager::InitializeHistograms(ptbins);


    int nentries = jettree->GetEntries();
    // nentries = 1000;
    double jetsKept = 0;
    double jets_5_10 = 0;
    double jets_10_15 = 0;
    double jets_15_20 = 0;
    double jets_20_25 = 0;
    double jets_25_30 = 0;


    for (int i = 0; i < nentries; ++i)
    {
      jettree->GetEntry(i);
      if (i % 100000 == 0){std::cout << "Processed " << i << " jets."  << std::endl;}  
            
      if (nConstituents<3) {continue;}


      Jet *jet = new Jet();
      for (int j = 0; j < nConstituents; j++)
      {
        Particle *part = new Particle((*dtr_PID)[j],(*dtr_Px)[j],(*dtr_Py)[j],(*dtr_Pz)[j],(*dtr_E)[j]);
        jet->addConstituent(*part);
        delete part;
      }
      if (jet->getNConstituents() == 0) { delete jet; continue; }
      
      // Fill jetCounts using hadron-only pT from the jet's four-vector
      double pt_had = jet->jPt(); // same as jet->get4vec().Pt()
      int bin = HistoManager::GetPtBinIndex(pt_had);
      if (bin >= 0 && HistoManager::histograms.count("jetCounts")) {
        ((TH1I*)HistoManager::histograms["jetCounts"])->Fill(bin);
      }

      // if (jet->getNConstituents() >= 2) {
      //   const auto& cons = jet->getConstituents();
      //   int pid0 = cons[0].getPID();
      //   int pid1 = cons[1].getPID();
      //   std::cout << "E2C pair PIDs: (" << pid0 << ", " << pid1 << ")\n";
      // }

      
      if (jet->jPt()<5){delete jet; continue;}
      if (jet->jPt()>5 && jet->jPt()<10){jets_5_10++;}
      if (jet->jPt()>10 && jet->jPt()<15){jets_10_15++;}
      if (jet->jPt()>15 && jet->jPt()<20){jets_15_20++;}
      if (jet->jPt()>20 && jet->jPt()<25){jets_20_25++;}
      if (jet->jPt()>25 && jet->jPt()<30){jets_25_30++;}

      jetsKept++;

      // fill EEC histograms 
      EnergyCorrelator *eec = new EnergyCorrelator(*jet);
      eec->FillHistograms();
      delete eec;   // <--
      delete jet;
      // std::cout << "Total jets processed: " << nentries << std::endl;
      // // std::cout << "Jets after filter: " << jetsKept << std::endl;
      // std::cout << "jets yield:" << (double) jetsKept/nentries << std::endl;
      // std::cout << "jet yield of jPt=5-10GeV: " << jets_5_10/nentries <<std::endl;
      // std::cout << "jet yield of jPt=10-15GeV: " << jets_10_15/nentries <<std::endl;
      // std::cout << "jet yield of jPt=15-20GeV: " << jets_15_20/nentries <<std::endl;
      // std::cout << "jet yield of jPt=20-25GeV: " << jets_20_25/nentries <<std::endl;
      // std::cout << "jet yield of jPt=25-30GeV: " << jets_25_30/nentries <<std::endl;
      // std::cout << "progress=" << ((i + 1) * 100.0 / nentries) << "%" << std::endl;



      
      

    }

    HistoManager::WriteHistogramsToFile(); 
    HistoManager::Cleanup();
    outfile->Close();

    delete infilePythia;
    delete outfile;


}