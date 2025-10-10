// plot_deltaR_vs_theta.C
#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <iostream>

void deltaR_vs_theta(
  const char* infile = "test_outfile_truth_truth_eAu_GenShd_1_R_1_n_05_1M.root",
  const char* hname  = "deltaR_vs_theta")
{
    gStyle->SetOptStat(0);
    gStyle->SetNumberContours(255);

    TFile* f = TFile::Open(infile, "READ");
    if (!f || f->IsZombie()) { std::cerr << "Can't open " << infile << "\n"; return; }

    TH2D* h = nullptr;
    f->GetObject(hname, h);
    if (!h) {
        std::cerr << "Histogram '" << hname << "' not found in " << infile << "\n";
        f->ls();
        f->Close();
        return;
    }

    TCanvas* c = new TCanvas("c", "#DeltaR vs #theta (eAu)", 950, 750);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.16);
    c->SetBottomMargin(0.12);

    h->SetTitle("");
    h->GetXaxis()->SetTitle("#Delta R");
    h->GetYaxis()->SetTitle("#theta");
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetXaxis()->SetLabelSize(0.045);
    h->GetYaxis()->SetLabelSize(0.045);

    // --- Set requested ranges and log scales ---
    h->GetXaxis()->SetRangeUser(0.05, 1.0);
    h->GetYaxis()->SetRangeUser(0.05, 1.0);
    h->SetMinimum(10);   // positive min so log Z works

    c->SetLogx();
    c->SetLogy();
    c->SetLogz();

    h->Draw("COLZ");

    TLatex txt;
    txt.SetNDC();
    txt.SetTextSize(0.045);
    txt.DrawLatex(0.14, 0.93, "1M eAu events, GenShd=1, R=1");

}
