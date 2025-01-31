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

void EEC_ratio()
{	
	int bins=100; // 90 for parked turbo line
	TH1F* EEC_ratio[7];
	TH1D* h2eec[7];
	TH1D* h3eec[7];
	for (int i = 0; i < 7; ++i) {
        EEC_ratio[i] = new TH1F(Form("EEC_ratio_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), bins, 0.02, 0.2);
		TFile *ifile3 = new TFile("E3C.root");
		h3eec[i] = (TH1D*)ifile3->Get(Form("EEC_hist_%d",i));
		TFile *ifile2 = new TFile("E2C.root");
		h2eec[i] = (TH1D*)ifile2->Get(Form("EEC_hist_%d",i));
	
    
    }

	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
	c1->Divide(4, 2);
	int pTarray[8]={5,10,15,20,25,30,35,40};

for (int i = 0; i<7; i++){
	//  // histogram
    c1->cd(i+1);
	gPad->SetLeftMargin(0.15);

	// EEC_ratio->Scale(1./Numjets);
	EEC_ratio[i]->Divide(h3eec[i], h2eec[i]);
    EEC_ratio[i]->Draw("PE"); // Draw the histogram
	EEC_ratio[i]->SetTitle("E3C:E2C ratio"); // Title
	EEC_ratio[i]->GetXaxis()->SetTitle("x_{L}"); // X-Axis title (Do it for the first histogram we plot)	
	EEC_ratio[i]->GetYaxis()->SetTitle("#frac{d #sigma^{[3]}/N_{jet} dx_{L}}{d #sigma^{[2]}/N_{jet} dx_{L}}");
	EEC_ratio[i]->SetMaximum(1.6); // Set y-axis maximum
   	EEC_ratio[i]->GetXaxis()->SetRangeUser(0.02, 0.2); // Set x-axis maximum


    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.03);
    latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV",pTarray[i],pTarray[i+1]));
	gPad->SetLogx();
	}

    // // EEC_hist->GetYaxis()->SetRangeUser(1, 1000);

    // EEC_ratio->SetMaximum(0.1); // Set y-axis maximum
    // EEC_ratio->SetMinimum(0.00001); // Set y-axis maximum
	c1->SaveAs("plots/EEC ratio.pdf"); // Save the histogram as a PDF file

}