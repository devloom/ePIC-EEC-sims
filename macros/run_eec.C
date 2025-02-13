#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void run_eec()
{
    // =============== Eventually we can just store Jet objects into our trees directly from Pythia ========= //
    // ========== For now just read in the vector branches and we will manually make the jet objects ========== //
    // read pythia jets from file
    // std::string inputPythia = "pythia8_inclusivejets.root";
    std::string inputPythia = "../../simulations/smeared_p_excl_jets.root";
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

    // Create histograms
    std::vector<float> ptbins = {5,10,15,20,25,30,35,40};
    HistoManager::InitializeHistograms(ptbins);
    TH1F *jetPt = new TH1F("jetPt", "Jet pT Distribution; pT (GeV); Number of Jets", 40, 0, 200);  // 40 bins, 0 to 200 GeV

    for (int i = 0; i < jettree->GetEntries(); ++i)
    //for (int i = 0; i < jettree->GetEntries(); ++i)
    {
      if (i % 100000 == 0){std::cout << "Processed " << i << " jets." << std::endl;}
      jettree->GetEntry(i);
      
      int count=0;
      for (int i = 0; i < nConstituents; i++)
						{if (dtr_PID->at(i) == 0) {
							count++;}}
					if (count==nConstituents)
						{continue;}
      //std::cout << nConstituents << std::endl;
      jetPt->Fill(jPt);  //store jPt for jet count for scaling

      Jet *jet = new Jet();
      for (int j = 0; j < nConstituents; j++)
      {
        
        Particle *part = new Particle((*dtr_PID)[j],(*dtr_Px)[j],(*dtr_Py)[j],(*dtr_Pz)[j],(*dtr_E)[j]);
        jet->addConstituent(*part);
      }
      EnergyCorrelator *eec = new EnergyCorrelator(*jet);
      eec->FillHistograms();
    }

    TFile *outfile = new TFile("test_outfile_smeared.root","RECREATE");
    HistoManager::WriteHistogramsToFile();
    jetPt->Write(); 
    outfile->Close();

    HistoManager::Cleanup();
    delete infilePythia;
    delete outfile;

}