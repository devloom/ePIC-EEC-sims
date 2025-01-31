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

void EEC_ratio_pp()
{	int bins=100;
	TH1F* EEC_ratio[8];
	TH1D* h2eec[8];
	TH1D* h3eec[8];
	for (int i = 0; i < 8; ++i) {
        EEC_ratio[i] = new TH1F(Form("EEC_ratio_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), bins, 0.02, 0.3);
		TFile *ifile3 = new TFile("E3C_pp.root");
		h3eec[i] = (TH1D*)ifile3->Get(Form("EEC_hist_%d",i));
		TFile *ifile2 = new TFile("E2C_pp.root");
		h2eec[i] = (TH1D*)ifile2->Get(Form("EEC_hist_%d",i));
	
    
    }

	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
	c1->Divide(4, 2);
	int pTarray[9]={97,220,330,468,638,846,1101,1410,1784};

for (int i = 0; i<8; i++){
	//  // histogram
    c1->cd(i+1);
	gPad->SetLeftMargin(0.15);
	// EEC_ratio->Scale(1./Numjets);
	EEC_ratio[i]->Divide(h3eec[i], h2eec[i]);
    EEC_ratio[i]->Draw("PE"); // Draw the histogram
	EEC_ratio[i]->SetTitle("E3C:E2C ratio pp"); // Title
	EEC_ratio[i]->GetXaxis()->SetTitle("x_{L}"); // X-Axis title (Do it for the first histogram we plot)	
	EEC_ratio[i]->GetYaxis()->SetTitle("#frac{d #sigma^{[3]}/N_{jet} dx_{L}}{d #sigma^{[2]}/N_{jet} dx_{L}}");
	EEC_ratio[i]->SetMaximum(1.6); // Set y-axis maximum
	EEC_ratio[i]->SetMinimum(1.1); // Set y-axis maximum
   	EEC_ratio[i]->GetXaxis()->SetRangeUser(0.02, 0.3); // Set x-axis maximum
    

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.03);
    latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV",pTarray[i],pTarray[i+1]));
	gPad->SetLogx();
	}

    // // EEC_hist->GetYaxis()->SetRangeUser(1, 1000);

    // EEC_ratio->SetMaximum(0.1); // Set y-axis maximum
    // EEC_ratio->SetMinimum(0.00001); // Set y-axis maximum
	c1->SaveAs("plots/EEC ratio pp.pdf"); // Save the histogram as a PDF file

}