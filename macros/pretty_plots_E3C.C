#include "../include/HistoManager.h"
#include <cmath>
#include <iostream>

R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void pretty_plots_E3C()
{

    TFile *file_truth = TFile::Open("test_outfile_truth_E3C.root", "READ");
    TFile *file_smeared = TFile::Open("test_outfile_smeared_E3C.root", "READ");
   

	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 3000, 2000); // 800 and 600 tell the program the size of the histogram
	c1->Divide(4,2);
    double xmax = 1;
	double xmin =0.0005;
	int bins_low=100;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;
    int pTarray[7]={5,10,15,20,25,30,35};
	

    TTree *jetCountTree_truth = nullptr;
    TTree *jetCountTree_smeared = nullptr;
	file_smeared->GetObject("jetCountTree_smeared", jetCountTree_smeared);
    file_truth->GetObject("jetCountTree_truth", jetCountTree_truth);
	double jetCounts_truth[7] = {0};
	double jetCounts_smeared[7] = {0};
	jetCountTree_truth->SetBranchAddress("jetCounts_truth", jetCounts_truth);
    jetCountTree_smeared->SetBranchAddress("jetCounts_smeared", jetCounts_smeared);
    jetCountTree_truth->GetEntry(0);
    jetCountTree_smeared->GetEntry(0);
  
	
    std::vector<TH1D*> E3C_hist_smeared(6), E3C_hist_truth(6);
    for (int i = 0; i < 6; ++i) {                                      //get the histogram from root outfile
        std::string hist_name_smeared = Form("E3C_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);
        std::string hist_name_truth = Form("E3C_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);

        E3C_hist_smeared[i] = (TH1D*)file_smeared->Get(hist_name_smeared.c_str());
        E3C_hist_truth[i] = (TH1D*)file_truth->Get(hist_name_truth.c_str());}


	for (int i=0; i<6; ++i){			//drawing histograms
		c1->cd(i+1);
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}
        E3C_hist_smeared[i]->Scale(1. / (jetCounts_smeared[i]*binwidth));
		E3C_hist_truth[i]->Scale(1. / (jetCounts_truth[i]*binwidth));
		



		E3C_hist_smeared[i]->GetXaxis()->SetRangeUser(0.0005, 10);
		E3C_hist_truth[i]->GetXaxis()->SetRangeUser(0.0005, 10);
		E3C_hist_smeared[i]->SetMaximum(1.); // Set y-axis maximum
    	E3C_hist_smeared[i]->SetMinimum(0.00001); // Set y-axis maximum
		E3C_hist_truth[i]->SetMaximum(1.); // Set y-axis maximum
    	E3C_hist_truth[i]->SetMinimum(0.00001); // Set y-axis maximum
		E3C_hist_smeared[i]->SetLineColor(kRed); 
        E3C_hist_truth[i]->SetLineColor(kGreen); 
		E3C_hist_smeared[i]->Draw("Pe"); // Draw the histogram
		E3C_hist_truth[i]->Draw("Pe SAME"); // Draw the histogram
		
		E3C_hist_truth[i]->GetXaxis()->SetTitle("x_{L}");
		E3C_hist_truth[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");
		
	


		TRatioPlot* rp = new TRatioPlot(E3C_hist_truth[i], E3C_hist_smeared[i]);
        rp->SetH1DrawOpt("PE");  
        rp->SetH2DrawOpt("PE"); 
        rp->Draw("nogrid"); 
		rp->GetUpperPad()->SetLogx(true);
		rp->GetUpperPad()->SetLogy(true);
		rp->GetLowerPad()->SetLogx(true);
		rp->GetLowerPad()->SetLogy(false);
        rp->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 3);
		rp->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");
		rp->GetLowerRefGraph()->GetYaxis()->SetTitleOffset(rp->GetLowerRefGraph()->GetYaxis()->GetTitleOffset()+1.5);


		// E3C_hist_smeared[i]->SetTitle("2-point E3C"); // Title
		gPad->SetLogx();
		gPad->SetLogy();

		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.05);

        
		gPad->Update();
		}

	

	c1->cd(7);
	TLatex latex;
	latex.SetNDC();
    latex.SetTextSize(0.05);
	latex.DrawLatex(0.5, 0.85, "1000000 events");
    latex.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
	latex.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
	latex.DrawLatex(0.5, 0.70, "jet radius= 1.0");
	TLegend *legend = new TLegend(0.5, 0.5, 0.72, 0.6);  // Position the legend
	legend->SetBorderSize(0); 
	legend->SetFillStyle(0);
	legend->SetTextSize(0.05);
    legend->AddEntry(E3C_hist_smeared[0], "smeared detector", "l");
    legend->AddEntry(E3C_hist_truth[0], "perfect detector", "l");
	legend->Draw();

    c1->Update();
	c1->SaveAs("../../simulations/plots/E3C_pretty_plot.pdf"); // Save the histogram as a PDF file

}