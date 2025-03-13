#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

// 0 for truth, 1 for smeared
void run_eec(int runtype = 1)
{
    // =============== Eventually we can just store Jet objects into our trees directly from Pythia ========= //
    // ========== For now just read in the vector branches and we will manually make the jet objects ========== //
    // read pythia jets from file

    std::string s_runtype[2] = {"truth","smeared"};

    std::string inputPythia;
    if (runtype == 0)
    {
      inputPythia = "../../rootfiles/truth_jets.root";
    }
    else if (runtype == 1)
    {
      inputPythia = "../../rootfiles/0_2_smeared_cluster_into_jets.root";
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
    TFile *outfile = new TFile(Form("test_outfile_%s.root",s_runtype[runtype].c_str()),"RECREATE");
    std::vector<float> ptbins = {5,10,15,20,25,30,35,40};
    HistoManager::InitializeHistograms(ptbins);


    int nentries = jettree->GetEntries();
    // nentries = 1000;
    for (int i = 0; i < nentries; ++i)
    {
      jettree->GetEntry(i);
      
      if (i % 100000 == 0){std::cout << "Processed " << i << " jets."  << std::endl;}  
            
      Jet *jet = new Jet();
      for (int j = 0; j < nConstituents; j++)
      {
        Particle *part = new Particle((*dtr_PID)[j],(*dtr_Px)[j],(*dtr_Py)[j],(*dtr_Pz)[j],(*dtr_E)[j]);
        jet->addConstituent(*part);
      }

      // remove jets that have less than 3 hadronic constituents
      if (jet->getNConstituents() < 3){continue;} 

      // fill EEC histograms 
      EnergyCorrelator *eec = new EnergyCorrelator(*jet);
      eec->FillHistograms();
    }
    
    HistoManager::WriteHistogramsToFile(); 
    HistoManager::Cleanup();
    outfile->Close();

    delete infilePythia;
    delete outfile;


}