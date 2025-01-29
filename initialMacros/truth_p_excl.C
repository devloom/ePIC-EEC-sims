#include "fastjet/ClusterSequence.hh"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <vector>
#include <iostream>
#include "eicsmear/smear/ParticleMCS.h"
#include "eicsmear/erhic/EventMC.h"

using namespace std;

void truth_p_excl() {
    // Open input file
    TFile *inputFile = TFile::Open("output.root", "READ");
    if (!inputFile || inputFile->IsZombie()) {
        cout << "Error: could not open input file 'output.root'" << endl;
        return;
    }

    TTree *inputTree = (TTree*)inputFile->Get("EICTree");
    if (!inputTree) {
        cout << "Error: could not find the input TTree 'EICTree'" << endl;
        inputFile->Close();
        return;
    }

    erhic::EventMC* truth_event = nullptr;
    inputTree->SetBranchAddress("event", &truth_event);

    // Create output file
    TFile *outFile = new TFile("truth_p_excl.root", "RECREATE");
    TTree *outputTree = inputTree->CloneTree(0); // Clone structure without data

    vector<float> px, py, pz, E, eta, phi;
    vector<int> id, status;

    outputTree->Branch("filtered_px", &px);
    outputTree->Branch("filtered_py", &py);
    outputTree->Branch("filtered_pz", &pz);
    outputTree->Branch("filtered_E", &E);
    outputTree->Branch("filtered_eta", &eta);
    outputTree->Branch("filtered_phi", &phi);
    outputTree->Branch("filtered_id", &id);
    outputTree->Branch("filtered_status", &status);

    int nEntries = inputTree->GetEntries();

    for (int iEntry = 0; iEntry < nEntries; ++iEntry) {
        inputTree->GetEntry(iEntry);

        // Clear previous event's stored data
        px.clear();
        py.clear();
        pz.clear();
        E.clear();
        eta.clear();
        phi.clear();
        id.clear();
        status.clear();

        int nParticles = truth_event->GetNTracks();
        for (int i = 0; i < nParticles; ++i) {
            const erhic::ParticleMC* part = truth_event->GetTrack(i);
            if (!part) continue;

            int pid = part->Id();
            if (pid == 2212 || pid == -2212 || pid == 2112 || pid == -2112) {
                continue; // Skip protons and neutrons
            }

            // Store only the filtered particles
            px.push_back(part->GetPx());
            py.push_back(part->GetPy());
            pz.push_back(part->GetPz());
            E.push_back(part->GetE());
            eta.push_back(part->GetEta());
            phi.push_back(part->GetPhi());
            id.push_back(part->Id());
            status.push_back(part->GetStatus());
        }

        outputTree->Fill(); // Fill event with filtered tracks
    }

    outFile->cd();
    outputTree->Write();
    outFile->Close();
    inputFile->Close();
}
