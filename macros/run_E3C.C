#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void run_E3C()
{
    // =============== Eventually we can just store Jet objects into our trees directly from Pythia ========= //
    // ========== For now just read in the vector branches and we will manually make the jet objects ========== //
    // read pythia jets from file
    // std::string inputPythia = "pythia8_inclusivejets.root";
    // std::string inputPythia = "../../simulations/truth_jets.root";
    std::string inputPythia = "../../simulations/0_2_smeared_cluster_into_jets.root";
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
    double jetCounts_smeared[7] = {0};
    TFile *outfile = new TFile("test_outfile_smeared_E3C.root","RECREATE");

    TTree *jetCountTree_smeared = new TTree("jetCountTree_smeared","a tree with jet counts");           //creating an extra tree for jetcounts for scaling
    jetCountTree_smeared->Branch("jetCounts_smeared", jetCounts_smeared, "jetCounts_smeared[7]/D");
    HistoManager::InitializeHistograms(ptbins);


    for (int i = 0; i < jettree->GetEntries(); ++i)
      {jettree->GetEntry(i);
      
      if (i % 100000 == 0){std::cout << "Processed " << i << " jets."  << std::endl;}
      
      if (nConstituents < 3){continue;}    
      int count=0;                                      //exclusions and cut
      for (int j = 0; j < nConstituents; j++)
			  // {if (dtr_PID->at(j) == 11 ||dtr_PID->at(j) == 22|| fabs(dtr_PID->at(j)) == 2212 ||fabs(dtr_PID->at(j)) == 2112) {
        {if (dtr_PID->at(j) == 0) {
							count++;}}
					if (count==nConstituents)
						{continue;}

            
      for (int k = 0; k < 7; ++k) {
            if (jPt > ptbins[k] && jPt < ptbins[k + 1]) {
                jetCounts_smeared[k]++;  

            }
        }
            

      Jet *jet = new Jet();
      for (int j = 0; j < nConstituents; j++)
      {
        
        Particle *part = new Particle((*dtr_PID)[j],(*dtr_Px)[j],(*dtr_Py)[j],(*dtr_Pz)[j],(*dtr_E)[j]);
        jet->addConstituent(*part);
      }
      EnergyCorrelator *E3C = new EnergyCorrelator(*jet);
      E3C->FillHistograms();
    }
        
    jetCountTree_smeared->Fill();
    jetCountTree_smeared->Write();

    
    HistoManager::WriteHistogramsToFile(); 
    HistoManager::Cleanup();
    outfile->Close();

    delete infilePythia;
    delete outfile;


}