#include "fastjet/ClusterSequence.hh"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <vector>
#include <iostream>
// Include the header for Smear::ParticleMCS
#include "eicsmear/smear/ParticleMCS.h"
#include <eicsmear/erhic/EventMC.h>

using namespace fastjet;
using namespace std;


class MyInfo: public
PseudoJet::UserInfoBase {
 public:
   MyInfo(int id, int status) : _pdg_id(id), _status_code(status) {}
   //MyInfo(int id) : _pdg_id(id) {}
   int pdg_id() const {return _pdg_id;}
   int status_code() const {return _status_code;}
   int _pdg_id;
   int _status_code;
   };


void smeared_cluster_into_jets() {
    // Open the input file and get the smeared tree
    // TFile *inputFile = TFile::Open("smeared_EIC_spec.root", "READ");
    // if (!inputFile || inputFile->IsZombie()) {
    //     cout << "Error: could not open input file 'smeared_EIC_spec.root'" << endl;
    //     return;
    // }

    // // Access the tree "Smeared"
    // TTree *inputTree = (TTree*)inputFile->Get("Smeared");
    // if (!inputTree) {
    //     cout << "Error: could not find the input TTree 'Smeared' in 'smeared_EIC_spec.root'" << endl;
    //     return;
    // }

    TFile *inputFile = TFile::Open("output.root", "READ");
    TTree *inputTree = (TTree*)inputFile->Get("EICTree");
    erhic::EventMC* truth_event = nullptr;
    inputTree->SetBranchAddress("event", &truth_event);

    // // Declare a vector of Smear::ParticleMCS* to hold particles
    // Smear::Event* event = nullptr;
    erhic::EventMC* event = nullptr;
    // Set the branch address for "particles"
    inputTree->SetBranchAddress("eventS", &event);

    // Variables for jet tree
    int nConstituents, eventNum;;
    double jPt, jPx, jPy, jPz, jE, jeta;
    std::vector<double> dtr_Px, dtr_Py, dtr_Pz, dtr_E;
    std::vector<double> dtr_Eta;
    std::vector<double> dtr_Phi;
    std::vector<int>dtr_PID;
    std::vector<int>dtr_status;
    double pid;

    // Create output TTree for jets
    TFile *outFile = new TFile("truth_jets.root", "RECREATE");
    TTree *jettree = new TTree("jets", "A tree with jet info");
    jettree->Branch("eventNum" , &eventNum,  "eventNum/I");
    jettree->Branch("nConstituents", &nConstituents, "nConstituents/I");
    jettree->Branch("jPt", &jPt, "jPt/D");
    jettree->Branch("jPx", &jPx, "jPx/D");
    jettree->Branch("jPy", &jPy, "jPy/D");
    jettree->Branch("jPz", &jPz, "jPz/D");
    jettree->Branch("jE", &jE, "jE/D");
    jettree->Branch("jeta", &jeta, "jeta/D");
    jettree->Branch("dtr_Px", &dtr_Px);
    jettree->Branch("dtr_Py", &dtr_Py);
    jettree->Branch("dtr_Pz", &dtr_Pz);
    jettree->Branch("dtr_E", &dtr_E);
    jettree->Branch("dtr_Eta", &dtr_Eta);
    jettree->Branch("dtr_Phi", &dtr_Phi);
    jettree->Branch("dtr_PID", &dtr_PID);
    jettree->Branch("dtr_status", &dtr_status);

    // Choose jet definition (Anti-kT algorithm with radius 0.5)
    double R = 1.0;
    JetDefinition jet_def(antikt_algorithm, R);
    
    int nEntries = inputTree->GetEntries();

    //nEntries = 1;
    // event loop

    for (int iEntry = 0; iEntry < nEntries; ++iEntry) {
        int nParticlesTruth = truth_event->GetNTracks();
        // int nParticclesTruth = event->GetNTracks();
        // inputtree->GetEntry(iEntry);
        inputTree->GetEntry(iEntry);
        eventNum=iEntry;
        // Create a vector to store FastJet PseudoJets for each particle
        std::vector<PseudoJet> particlesForJets;
        particlesForJets.clear();
        // particle loop
        // int nParticles = event->GetNTracks();
        int nParticles = truth_event->GetNTracks();
        //std::cout << nParticles << std::endl;
        for (int i = 0; i < nParticles; ++i) 
        {
            
            const erhic::ParticleMC* part = truth_event->GetTrack(i); 
            // Smear::ParticleMCS* part = dynamic_cast<Smear::ParticleMCS*>(event->GetTrack(i));

            if (!part) {continue;}
            // bool kPxSmeared_smear = part->IsPxSmeared();
            // bool kPySmeared_smear = part->IsPySmeared();
            // bool kPzSmeared_smear = part->IsPzSmeared();
            // bool kESmeared_smear = part->IsESmeared();
            // if (kPxSmeared_smear==0||kPySmeared_smear==0||kPzSmeared_smear==0||kESmeared_smear==0){continue;}

            
            // only want final state particles
            if (part->GetStatus() != 1){continue;}


            Float_t eta = part->GetEta();    //pseudoripidity of a particular particle
            Float_t pt  = part->GetPt();
            //remove hard scattered electrons (this is done by asking if particle is electron or mother is electron)
                // EIC acceptance cut... //
            if (eta < -3.5 || eta > 3.5)  {continue;}
            if (pt < 0.2) {continue;}


            
            // Use Smear::ParticleMCS to get Px, Py, Pz, and Energy
            PseudoJet particle(part->GetPx(), part->GetPy(), part->GetPz(), part->GetE());
            //pid = part->Id();
            //std::cout << part->Id() << " " << part->GetStatus() << std::endl;
            particle.set_user_info(new MyInfo(part->Id(), part->GetStatus()));
            particlesForJets.push_back(particle);
                
            
        }

        // Perform jet clustering
        ClusterSequence cs(particlesForJets, jet_def);
        vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());

        // jet loop
        for (const auto& jet : jets) {
            jPt = jet.pt();
            jPx = jet.px();
            jPy = jet.py();
            jPz = jet.pz();
            jE = jet.E();
            jeta = jet.eta();

            // Get constituents of the jet
            vector<PseudoJet> constituents = jet.constituents();
            nConstituents = constituents.size();


            // Clear previous constituent data
            dtr_Px.clear();
            dtr_Py.clear();
            dtr_Pz.clear();
            dtr_E.clear();
            dtr_Eta.clear();
            dtr_Phi.clear();
            dtr_PID.clear();
            dtr_status.clear();
            // if (jet.pt() < 5.0) continue;

            // Loop over the constituents and store their properties
            for (const auto& constituent : constituents) {
                dtr_Px.push_back(constituent.px());
                dtr_Py.push_back(constituent.py());
                dtr_Pz.push_back(constituent.pz());
                dtr_E.push_back(constituent.E());
                dtr_Eta.push_back(constituent.eta());
                dtr_Phi.push_back(constituent.phi());
                //dtr_PID.push_back(pid);
                dtr_PID.push_back(constituent.user_info<MyInfo>().pdg_id());
                dtr_status.push_back(constituent.user_info<MyInfo>().status_code());
            }

            // Fill the jet tree
            jettree->Fill();
        }
    }

    // Write the jet tree to the output file
    outFile->cd();
    jettree->Write();
    outFile->Close();

    // Close the input file
    inputFile->Close();
}