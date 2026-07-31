// deltaR_vs_theta.C
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TRandom3.h>
#include <iostream>

void deltaR_vs_theta(
  const char* infile = "/home/miyagu/ePIC-EEC-sims/macros/great_lakes_data/smeared_EEC_eAu_high_pt_genshd_1_merged.root",
  bool use_E3C = false   // set to true if you want deltaR_E3C and theta_E3C instead
)
{
    gStyle->SetOptStat(0);
    gStyle->SetNumberContours(255);

    // --- Open file ---
    TFile* f = TFile::Open(infile, "READ");
    if (!f || f->IsZombie()) {
        std::cerr << "❌ Cannot open file: " << infile << std::endl;
        return;
    }

    // --- Load 1D histograms ---
    TH1D *hR = nullptr, *hTheta = nullptr;

    if (use_E3C) {
        f->GetObject("deltaR_E3C", hR);
        f->GetObject("theta_E3C",  hTheta);
    } else {
        f->GetObject("deltaR", hR);
        f->GetObject("theta",  hTheta);
    }

    if (!hR || !hTheta) {
        std::cerr << "❌ Could not find deltaR or theta histograms!\n";
        f->ls();
        f->Close();
        return;
    }

    // --- Create 2D histogram ---
    const int NBINS = 100;
    TH2D* h2 = new TH2D("deltaR_vs_theta", "#DeltaR vs #theta;#DeltaR;#theta",
                        NBINS, 0.01, 1.0, NBINS, 0.01, 1.0);

    // --- Use random sampling to approximate a correlation ---
    TRandom3 rng(42);
    int Nsamples = 200000; // adjust for smoother / faster plot

    for (int i = 0; i < Nsamples; ++i) {
        double deltaR = hR->GetRandom();     // random ΔR weighted by its distribution
        double theta  = hTheta->GetRandom(); // random θ weighted by its distribution
        h2->Fill(deltaR, theta);
    }

    // --- Draw ---
    TCanvas* c = new TCanvas("c", "#DeltaR vs #theta (synthetic correlation)", 950, 750);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.16);
    c->SetBottomMargin(0.12);

    c->SetLogx();
    c->SetLogy();
    c->SetLogz();

    h2->SetMinimum(1e-2);
    h2->Draw("COLZ");

    // --- Add label ---
    TLatex txt;
    txt.SetNDC();
    txt.SetTextSize(0.045);
    if (use_E3C)
        txt.DrawLatex(0.14, 0.93, "E3C: 100M eAu events, GenShd=1, R=1");
    else
        txt.DrawLatex(0.14, 0.93, "EEC: 100M eAu events, GenShd=1, R=1");

    c->SaveAs(use_E3C ? "deltaR_vs_theta_E3C.png" : "deltaR_vs_theta_EEC.png");
    std::cout << "✅ Saved as " << (use_E3C ? "deltaR_vs_theta_E3C.png" : "deltaR_vs_theta_EEC.png") << std::endl;

    f->Close();
}
