#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void run_eec()
{
    // =============== Eventually we can just store Jet objects into our trees directly from Pythia ========= //
    // ========== For now just read in the vector branches and we will manually make the jet objects ========== //
    // read pythia jets from file
    // std::string inputPythia = "pythia8_inclusivejets.root";
    std::string inputPythia = "../../simulations/truth_jets.root";
    TFile *infilePythia = new TFile(inputPythia.c_str());
    if (!infilePythia || infilePythia->IsZombie()){std::cerr << "Could not open file." << std::endl;return;}

    // read jet tree
    TTree *jettree = nullptr;
    infilePythia->GetObject("jets", jettree);
    if (!jettree){std::cerr << "Could not find tree." << std::endl;infilePythia->Close();return;}

    int nConstituents;
	double jPx, jPy, jPz, jE;
    std::vector<int>* dtr_PID = nullptr;
    std::vector<double> *dtr_Px = nullptr, *dtr_Py = nullptr, *dtr_Pz = nullptr,  *dtr_E = nullptr;
	jettree->SetBranchAddress("nConstituents", &nConstituents); 
	jettree->SetBranchAddress("dtr_Px" , &dtr_Px);
	jettree->SetBranchAddress("dtr_Py" , &dtr_Py);
	jettree->SetBranchAddress("dtr_Pz" , &dtr_Pz);
	jettree->SetBranchAddress("dtr_E" , &dtr_E);
	jettree->SetBranchAddress("dtr_PID" , &dtr_PID);
    // ================================================= //

    // Create histograms
    std::vector<float> ptbins = {5,10,15,20,25,30,35,40};
    HistoManager::InitializeHistograms(ptbins);

    for (int i = 0; i < jettree->GetEntries(); ++i)
    {
      if (i % 100000 == 0){std::cout << "Processed " << i << " jets." << std::endl;}
      jettree->GetEntry(i);
      if (nConstituents < 3){continue;}
      Jet *jet = new Jet();
      for (int j = 0; j < nConstituents; j++)
      {
        Particle *part = new Particle((*dtr_PID)[j],(*dtr_Px)[j],(*dtr_Py)[j],(*dtr_Pz)[j],(*dtr_E)[j]);
        jet->addConstituent(*part);
      }
      EnergyCorrelator *eec = new EnergyCorrelator(*jet);
      eec->FillHistograms();
    }

    TFile *outfile = new TFile("test_outfile.root","RECREATE");
    HistoManager::WriteHistogramsToFile();
    outfile->Close();

    HistoManager::Cleanup();
    delete infilePythia;
    delete outfile;

}