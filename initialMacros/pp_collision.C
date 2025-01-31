/// Jet hadronization studies with Pythia8 + FASTJET for B+ in jet correlation studies ///
/// Author(s): Mingyang Gu (Shaun), Devon Loomis ///
/// original/source Author(s): Dillon Fitzgerald, Nicole Kuchta, Julia Marchese, Jose Marco Arias (Markiito) ///


#include "TSystem.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include "fastjet/ClusterSequence.hh"
#include <iostream>
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>
#include <cstdlib>


using namespace fastjet;
using namespace std;

class MyInfo: public
PseudoJet::UserInfoBase {
 public:
   MyInfo(int id) : _pdg_id(id){}
   int pdg_id() const {return _pdg_id;}
   int _pdg_id;
   };


int verbosity = 0;
void pp_collision(Int_t nev  = 1000000 , Int_t ndeb = 2)
{


 R__LOAD_LIBRARY(/home/aidalagroup/fastjet/fastjet-install/lib/libfastjet.so);
 R__ADD_INCLUDE_PATH(/home/aidalagroup/fastjet/fastjet-3.4.0/include);


 double ptHatMin = 60.0;
 //double ptHatMax = 50.0; //
 TString ptHatMin_str = "PhaseSpace:pTHatMin = ";
 //TString ptHatMax_str = "PhaseSpace:pTHatMax = "; //
 ptHatMin_str += ptHatMin;
 //ptHatMax_str += ptHatMax;//


 TString outFileName = "pp_collision.root";
 //outFileName += ptHatMin;
 //outFileName += "_";
 //outFileName += ptHatMax;
 //outFileName += ".root";


 TFile *outFile = new TFile(outFileName,"RECREATE");
 // Load libraries //
 gSystem->Load("libEG");
 gSystem->Load("libEGPythia8");


 // Setup histograms //
 TH1D *ptHat_hist = new TH1D("ptHat", ";p_{T} [GeV/c];", 100, 0.0, 100.0);


 // Set up jet tree //
 int  nConstituents_i,nConstituents_j, eventNum;
 double jE_i,jE_j,jPt_i,jEta_i,jPhi_i,jPt_j,jEta_j,jPhi_j; 
 int cPid_i,cPid_j;


 std::vector<double> dtr_Px_i,dtr_Py_i,dtr_Pz_i,dtr_E_i,dtr_Px_j,dtr_Py_j,dtr_Pz_j,dtr_E_j;
//  std::vector<double> dtr_Phi,dtr_Eta;
 std::vector<int>dtr_PID_i,dtr_PID_j;


 TTree* jettree;
 jettree = new TTree("jets","A tree with jet info");                   //Jets tree///
 jettree->Branch("nConstituents_i", &nConstituents_i, "nConstituents_i/I");
 jettree->Branch("nConstituents_j", &nConstituents_j, "nConstituents_j/I");

 jettree->Branch("jPt_i" , &jPt_i,  "jPt_i/D" );
 jettree->Branch("jEta_i" , &jEta_i, "jEta_i/D");
 jettree->Branch("jPhi_i" , &jPhi_i, "jPhi_i/D");
 jettree->Branch("jPt_j" , &jPt_j,  "jPt_j/D" );
 jettree->Branch("jEta_j" , &jEta_j, "jEta_j/D");
 jettree->Branch("jPhi_j" , &jPhi_j, "jPhi_j/D");
 jettree->Branch("jE_i" , &jE_i, "jE_i/D");
 jettree->Branch("jE_j" , &jE_j, "jE_j/D");


 jettree->Branch("eventNum" , &eventNum,  "eventNum/I");
 jettree->Branch("dtr_Px_i" , &dtr_Px_i);
 jettree->Branch("dtr_Py_i" , &dtr_Py_i);
 jettree->Branch("dtr_Pz_i" , &dtr_Pz_i);
 jettree->Branch("dtr_E_i" , &dtr_E_i);
 jettree->Branch("dtr_PID_i",&dtr_PID_i);
 jettree->Branch("dtr_Px_j" , &dtr_Px_j);
 jettree->Branch("dtr_Py_j" , &dtr_Py_j);
 jettree->Branch("dtr_Pz_j" , &dtr_Pz_j);
 jettree->Branch("dtr_E_j" , &dtr_E_j);
 jettree->Branch("dtr_PID_j",&dtr_PID_j);




 // choose a jet definition //
 double R = 0.4;                 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 JetDefinition jet_def(antikt_algorithm, R);
 vector<PseudoJet> parts;


 // Array of particles //
 TClonesArray* particles = new TClonesArray("TParticle", 1000000); 
 // Create pythia8 object //           
 TPythia8* pythia8 = new TPythia8();


 #if PYTHIA_VERSION_INTEGER == 8235
   // Pythia 8.235 is known to cause crashes: //
   printf("ABORTING PYTHIA8 TUTORIAL!\n");
   printf("The version of Pythia you use is known to case crashes due to memory errors.\n");
   printf("They have been reported to the authors; the Pythia versions 8.1... are known to work.\n");
   return;
 #endif


 // Configure
 pythia8->ReadString("HardQCD:all = on");
//  pythia8->ReadString("WeakBosonExchange:all = on");
 pythia8->ReadString("Random:setSeed = on");
//  pythia8->ReadString("ParticleDecays:allowPhotonRadiation = on");

  // use a reproducible seed: always the same results for the tutorial.
 pythia8->ReadString("Random:seed = 52"); //make this random per event
  // Here is the pT hat cut... //
 pythia8->ReadString(ptHatMin_str);
 //pythia8->ReadString(ptHatMax_str);  //



 // LHC energy //
 pythia8->Initialize(2212 /* p */, 2212 /* p */, 13000. /* GeV */);                        //13.7 TeV Max achieved thie July!


 // Event loop
 for (Int_t iev = 0; iev < nev; iev++)
 {
   eventNum = iev;
   pythia8->GenerateEvent();

  //  cout<< "Event Number: "<< eventNum << endl;

   if (verbosity > 0)
     pythia8->EventListing();


   pythia8->ImportParticles(particles,"All");
   Int_t np = particles->GetEntriesFast();
   parts.clear();

  // float electron_pt = 0;
  // float electron_eta = 0;
  // float electron_phi = 0;



   // Particle loop (1)
   for (Int_t ip = 0; ip < np; ip++)
   {

     TParticle* part = (TParticle*) particles->At(ip);
    //  if (ip==5)
    // {
    //   electron_pt = part->Pz();
    //   electron_eta = part->Eta();
    //   electron_phi = part->Phi();

    // }
      //std::cout<<part->Pz()<< " "<<part->GetPdgCode()<< std::endl;}
     Int_t ist = part->GetStatusCode();
     Int_t pdg = part->GetPdgCode();
    //  cout<< "Particle PID: "<< pdg << endl;
         
         
    

     // EIC acceptance cut... //
     if (part->Eta() < -2.1 || part->Eta() > 2.1)  {continue;}
     if (ist <= 0) continue;
     if (part->Pt() < 0.2) continue;
     if (abs(pdg) == 12 || abs(pdg) == 14 || abs(pdg) == 16) continue;
     parts.push_back(PseudoJet(part->Px(), part->Py(), part->Pz(), part->Energy() ));
     parts.back().set_user_info(new MyInfo(pdg));
    
   } // end particle loop (1)


   // Cluster the jets //
   ClusterSequence cs(parts, jet_def);
   vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());


   // Jet loop //
   for (unsigned i = 0; i < jets.size(); i++)
   {
    for (unsigned j = i+1; j < jets.size(); j++)
        {
        dtr_Px_i.clear();
        dtr_Py_i.clear();
        dtr_Pz_i.clear();
        dtr_E_i.clear();
        dtr_PID_i.clear();
        dtr_Px_j.clear();
        dtr_Py_j.clear();
        dtr_Pz_j.clear();
        dtr_E_j.clear();
        dtr_PID_j.clear();

        if (jets[i].pt() < 30.0 || jets[j].pt() < 30.0) continue;
        if (jets[i].eta() < -2.1 || jets[i].eta() > 2.1)  {continue;}
        PseudoJet jet_i = jets[i];
        PseudoJet jet_j = jets[j];
        jPt_i = jet_i.pt();
        jEta_i = jet_i.eta();
        jPhi_i=jet_i.phi();
        jPt_j = jet_j.pt();
        jEta_j = jet_j.eta();
        jPhi_j=jet_j.phi();
        
        vector<PseudoJet> constituents_i = jet_i.constituents();
        nConstituents_i = constituents_i.size();
        vector<PseudoJet> constituents_j = jet_j.constituents();
        nConstituents_j = constituents_j.size();

        jE_i  = jet_i.E();
        jE_j  = jet_j.E();
            for (unsigned k = 0; k < constituents_i.size(); k++)
            {PseudoJet con_i = constituents_i[k];
            // TVector3 cons_3vec_i(con_i.px() , con_i.py(), con_i.pz());
            
            cPid_i = con_i.user_info<MyInfo>().pdg_id();
            if (cPid_i != 11 && cPid_i != 211 && cPid_i != -211) {continue;}
            dtr_PID_i.push_back(cPid_i);
            dtr_Px_i.push_back(con_i.px());
            dtr_Py_i.push_back(con_i.py());
            dtr_Pz_i.push_back(con_i.pz());
            dtr_E_i.push_back(con_i.E());
            }
            for (unsigned k = 0; k < constituents_j.size(); k++)
            {PseudoJet con_j = constituents_j[k];
            // TVector3 cons_3vec_j(con_j.px() , con_j.py(), con_j.pz());

            cPid_j = con_j.user_info<MyInfo>().pdg_id();
            if (cPid_j != 11 && cPid_j != 211 && cPid_j != -211) {continue;}
            dtr_PID_j.push_back(cPid_j);
            dtr_Px_j.push_back(con_j.px());
            dtr_Py_j.push_back(con_j.py());
            dtr_Pz_j.push_back(con_j.pz());
            dtr_E_j.push_back(con_j.E());
            }
        double delta_jPhi= fabs(jPhi_i - jPhi_j);
        if (delta_jPhi > 3.1415926){delta_jPhi-=(2*3.1415926);}
        if (fabs(delta_jPhi) < 3.0) continue;
        

        jettree->Fill();
   }
   }
 
//    }
//    else {continue;}
   // end jet loop //
   
 }
 // end event loop //
 pythia8->PrintStatistics();
 outFile->Write();
 outFile->Close();
}
