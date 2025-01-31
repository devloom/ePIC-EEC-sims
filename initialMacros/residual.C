/// Author(s): Mingyang Gu (Shaun), Devon Loomis ///
#include <iostream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include "TH1.h"
#include "TFile.h"
#include <vector>
#include <cmath>
#include <TMath.h>
#include "eicsmear/smear/ParticleMCS.h"
#include <eicsmear/erhic/EventMC.h>



void residual() {

    // Open the ROOT file for truth data
    TFile *output = TFile::Open("output.root", "READ");
    TTree *truth_tree = (TTree*)output->Get("EICTree");
    erhic::EventMC* truth_event = nullptr;
    truth_tree->SetBranchAddress("event", &truth_event);
 
    
    
////////////////////////////////////////////////////////////////////////////////////////////////
    // Open the ROOT file for perfect data
    TFile *smearedperfect = TFile::Open("smearedperfect.root", "READ");
    TTree *Smeared = (TTree*)smearedperfect->Get("Smeared");
    // Declare a pointer to the particles.p data for smeared
    Smear::Event* eventS = nullptr;
    Smeared->SetBranchAddress("eventS", &eventS);
////////////////////////////////////////////////////////////////////////////////////////////

    int nEventsTruth = truth_tree->GetEntries();
    int nEventsperfect = Smeared->GetEntries();
    // Create a histogram for residuals
    TH1F* residual_perfect_hist = new TH1F("residual_hist", "Residuals (Perfect - Truth)", 200, -5,5);

    // event loop 
    for (int i = 0; i < nEventsTruth; ++i) {
        // Get the entry for both trees
        truth_tree->GetEntry(i);
        Smeared->GetEntry(i);

        // if (!eventS) {
        //     std::cerr << "Error: eventS is null at event " << i << std::endl;
        //     continue;
        // }

        // Check size consistency
        int nParticlesperfect = eventS->GetNTracks();
        int nParticlesTruth = truth_event->GetNTracks();
        // cout << "truth=" << nParticlesTruth << ", perfect=" << nParticlesperfect << endl;
        // if (nParticlesTruth != nParticlesperfect) {
        //     std::cerr << "Error: Mismatched vector sizes in event " << i << "." << std::endl;
        //     continue;
        // }

        //particle loop
        for (int j = 0; j < nParticlesperfect; ++j) {
            Smear::ParticleMCS* part_perfect = dynamic_cast<Smear::ParticleMCS*>(eventS->GetTrack(j));
            const erhic::ParticleMC* part_truth = truth_event->GetTrack(j); 

            if (!part_perfect) {
            // std::cerr << "Warning: part_perfect is null at event " << i << ", track " << j << std::endl;
            continue;
            }
            if (!part_truth) {
                // std::cerr << "Warning: part_truth is null at event " << i << ", track " << j << std::endl;
                continue;
            }
            

            double perfect_p = part_perfect->GetP();
            double truth_p = part_truth->GetP();
            // cout << "perfect=" << perfect_p << ", truth=" << truth_p << endl;
            // // if (perfect_p==0 || truth_p==0){continue;}
            bool kPSmeared = part_perfect->IsPSmeared();
            if (kPSmeared==0){continue;}
            // cout << "perfect=" << perfect_p << ", truth=" << truth_p << endl;

            double residual = perfect_p - truth_p;
            residual_perfect_hist->Fill(residual);
        }
    }


//-------------------------------------------------------------------------------------------------------------------------
  // Open the ROOT file for truth data
  
    TFile *smeared_eic = TFile::Open("smeared_EIC_spec.root", "READ");
    TTree *SmearedEIC = (TTree*)smeared_eic->Get("Smeared");
    // Declare a pointer to the particles.p data for smeared
    Smear::Event* eventS_eic = nullptr;
    SmearedEIC->SetBranchAddress("eventS", &eventS_eic);
///////////////////////////////////////////////////////////////////////////////////////////
    int nEvents_eic = SmearedEIC->GetEntries();
    // Create a histogram for residuals
    TH1F* residual_smeared_hist = new TH1F("residual_hist", "Residuals (Smeared - Truth)", 200, -5,5);

    // event loop 
    for (int i = 0; i < nEventsTruth; ++i) {
        // Get the entry for both trees
        truth_tree->GetEntry(i);
        SmearedEIC->GetEntry(i);

        // if (!eventS) {
        //     std::cerr << "Error: eventS is null at event " << i << std::endl;
        //     continue;
        // }

        // Check size consistency
        int nParticles_eic = eventS_eic->GetNTracks();
        int nParticlesTruth = truth_event->GetNTracks();
        // if (nParticlesTruth != nParticlesperfect) {
        //     std::cerr << "Error: Mismatched vector sizes in event " << i << "." << std::endl;
        //     continue;
        // }

        //particle loop
        for (int j = 0; j < nParticles_eic; ++j) {
            Smear::ParticleMCS* part_smear = dynamic_cast<Smear::ParticleMCS*>(eventS_eic->GetTrack(j));
            const erhic::ParticleMC* part_truth = truth_event->GetTrack(j); 

            if (!part_smear) {
            // std::cerr << "Warning: part_smear is null at event " << i << ", track " << j << std::endl;
            continue;
            }
            if (!part_truth) {
                // std::cerr << "Warning: part_truth is null at event " << i << ", track " << j << std::endl;
                continue;
            }

            double smear_p = part_smear->GetP();
            double truth_p = part_truth->GetP();
            bool kPSmeared_smear = part_smear->IsPSmeared();
            if (kPSmeared_smear==0){continue;}
            cout << "smear=" << smear_p << ", truth=" << truth_p << endl;


            double residual = smear_p - truth_p;
            residual_smeared_hist->Fill(residual);
        }
    }
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Create canvas and plot the residual histogram
    TCanvas* c1 = new TCanvas("c1", "Residuals Histogram", 1600, 1000);
    gPad->SetLeftMargin(0.15);

    // residual_perfect_hist->SetLineColor(kBlue);  // Set color for perfect residuals
    // residual_perfect_hist->Draw("HIST");
    residual_smeared_hist->SetLineColor(kRed);  // Set color for perfect residuals
    residual_smeared_hist->Draw("HIST SAME");
    residual_smeared_hist->SetTitle("Perfect detector vs smeared detector Residual Plot");
    residual_smeared_hist->GetXaxis()->SetTitle("Residual (P, GeV)");
    residual_smeared_hist->GetYaxis()->SetTitle("Count");
    // residual_smeared_hist->GetYaxis()->SetRangeUser(0, 120000);

    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  // Position the legend
    // legend->AddEntry(residual_perfect_hist, "Perfect - Truth", "l");
    legend->AddEntry(residual_smeared_hist, "Smeared - Truth", "l");
    legend->Draw();

    // gPad->SetLogy();

    // Save the plot
    c1->SaveAs("plots/residual.pdf");

    // Write the histogram to a new file
    

    // Close the input files
    output->Close();
    smearedperfect->Close();
}
