#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void pretty_plots()
{	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);  // <--- This hides the display of titles in plots

	double xmax = 1;
	double xmin =0.0005;
	int bins_low=100;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;

	int pTarray[7]={5,10,15,20,25,30,35};


    TFile *file_truth = TFile::Open("test_outfile_truth.root", "READ");
	// TFile *file_smeared = TFile::Open("test_outfile_smeared.root", "READ");
    TFile *file_smeared = TFile::Open("test_outfile_smeared.root", "READ");
	



	TCanvas* cE2C = new TCanvas("cE2C", "Histogram Canvas",1700,1700); // 800 and 600 tell the program the size of the histogram
	cE2C->Divide(4,2);
    
	TCanvas* cE3C = new TCanvas("cE3C", "Histogram Canvas", 1700, 1700); // 800 and 600 tell the program the size of the histogram
	cE3C->Divide(4,2);

	TCanvas* cRatio = new TCanvas("cRatio", "EEEC/EEC Ratio", 1700, 1700);
    cRatio->Divide(4,2);

	TH1I *hjetcounts_truth = nullptr;
	file_truth->GetObject("jetCounts",hjetcounts_truth);

	TH1I *hjetcounts_smeared = nullptr;
	file_smeared->GetObject("jetCounts",hjetcounts_smeared);
  
	
    std::vector<TH1F*> EEC_hist_smeared(5);
	std::vector<TH1F*> EEC_hist_truth(5);

	std::vector<TH1D*> E3C_hist_smeared(5);
	std::vector<TH1D*> E3C_hist_truth(5);

	

    for (int i = 0; i < 5; ++i) 
	{                     
		// Get E2Cs
        std::string hist_name_smeared_E2C = Form("eec_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_E2C = Form("eec_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        EEC_hist_smeared[i] = (TH1F*)file_smeared->Get(hist_name_smeared_E2C.c_str());
        EEC_hist_truth[i] = (TH1F*)file_truth->Get(hist_name_truth_E2C.c_str());

		// Get E3Cs
		std::string hist_name_smeared_E3C = Form("E3C_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_E3C = Form("E3C_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        E3C_hist_smeared[i] = (TH1D*)file_smeared->Get(hist_name_smeared_E3C.c_str());
        E3C_hist_truth[i] = (TH1D*)file_truth->Get(hist_name_truth_E3C.c_str());


		// E2C plots
		cE2C->cd(i+1);
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}

        EEC_hist_smeared[i]->Scale(1. / (hjetcounts_smeared->GetBinContent(i+1)*binwidth));
		EEC_hist_truth[i]->Scale(1. / (hjetcounts_truth->GetBinContent(i+1)*binwidth));
		EEC_hist_smeared[i]->GetXaxis()->SetRangeUser(0.0005, 1);
		EEC_hist_smeared[i]->SetMaximum(100.); // Set y-axis maximum
    	EEC_hist_smeared[i]->SetMinimum(0.01); // Set y-axis maximum
		EEC_hist_smeared[i]->SetLineColor(kRed); 
		EEC_hist_smeared[i]->SetMarkerSize(0.5);
		EEC_hist_smeared[i]->SetMarkerStyle(21);
		EEC_hist_smeared[i]->SetMarkerColor(kRed);
		EEC_hist_smeared[i]->Draw("pe"); // Draw the histogram
		EEC_hist_smeared[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_smeared[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");

		EEC_hist_truth[i]->GetXaxis()->SetRangeUser(0.0005, 1);
		EEC_hist_truth[i]->SetMaximum(100.); // Set y-axis maximum
    	EEC_hist_truth[i]->SetMinimum(0.01); // Set y-axis maximum
        EEC_hist_truth[i]->SetLineColor(kGreen); 
		EEC_hist_truth[i]->SetMarkerSize(0.7);
		EEC_hist_truth[i]->SetMarkerStyle(22);
		EEC_hist_truth[i]->SetMarkerColor(kGreen);
		
		EEC_hist_truth[i]->Draw("pe SAME"); // Draw the histogram
		EEC_hist_truth[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_truth[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		
		
		TRatioPlot* rpE2C = new TRatioPlot(EEC_hist_truth[i], EEC_hist_smeared[i]);
        rpE2C->SetH1DrawOpt("pe");  
        rpE2C->SetH2DrawOpt("pe"); 
        rpE2C->Draw("nogrid"); 
		rpE2C->GetUpperPad()->SetLogx(true);
		rpE2C->GetUpperPad()->SetLogy(true);
		rpE2C->GetLowerPad()->SetLogx(true);
		rpE2C->GetLowerPad()->SetLogy(false);
        rpE2C->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 3);
		rpE2C->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");
		rpE2C->GetLowerRefGraph()->GetYaxis()->SetTitleOffset(rpE2C->GetLowerRefGraph()->GetYaxis()->GetTitleOffset()+1.7);
		rpE2C->GetUpperPad()->cd();

		
		
		TLatex ptLabel;
		ptLabel.SetNDC();
		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		
		TLegend *legendE2C = new TLegend(0.55, 0.2, 0.92, 0.3);  // Position the legend
		legendE2C->SetBorderSize(0); 
		legendE2C->SetFillStyle(0);
		legendE2C->SetTextSize(0.03);
		legendE2C->AddEntry(EEC_hist_smeared[0], "smeared", "p");
		legendE2C->AddEntry(EEC_hist_truth[0], "truth", "p");
		legendE2C->Draw();
		
		gPad->SetLogx();
		gPad->SetLogy();

		


		// E3C plots
		cE3C->cd(i+1);
		E3C_hist_smeared[i]->Scale(1. / (hjetcounts_smeared->GetBinContent(i+1)*binwidth));
		E3C_hist_truth[i]->Scale(1. / (hjetcounts_truth->GetBinContent(i+1)*binwidth));
		E3C_hist_smeared[i]->GetXaxis()->SetRangeUser(0.0005, 1);
		E3C_hist_truth[i]->GetXaxis()->SetRangeUser(0.0005, 1);
		E3C_hist_smeared[i]->SetMaximum(100.); // Set y-axis maximum
    	E3C_hist_smeared[i]->SetMinimum(0.01); // Set y-axis maximum
		E3C_hist_truth[i]->SetMaximum(100.); // Set y-axis maximum
    	E3C_hist_truth[i]->SetMinimum(0.01); // Set y-axis maximum
		E3C_hist_smeared[i]->SetLineColor(kRed); 
        E3C_hist_truth[i]->SetLineColor(kGreen); 
		E3C_hist_smeared[i]->SetMarkerStyle(21);
		E3C_hist_truth[i]->SetMarkerStyle(22);
		E3C_hist_smeared[i]->Draw("pe"); // Draw the histogram
		E3C_hist_truth[i]->Draw("pe SAME"); // Draw the histogram
		E3C_hist_smeared[i]->SetMarkerSize(0.5);
		E3C_hist_truth[i]->SetMarkerSize(0.7);
		E3C_hist_smeared[i]->SetMarkerColor(kRed);
		E3C_hist_truth[i]->SetMarkerColor(kGreen);
		E3C_hist_truth[i]->GetXaxis()->SetTitle("x_{L}");
		E3C_hist_truth[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");

		TRatioPlot* rpE3C = new TRatioPlot(E3C_hist_truth[i], E3C_hist_smeared[i]);
        rpE3C->SetH1DrawOpt("pe");  
        rpE3C->SetH2DrawOpt("pe"); 
        rpE3C->Draw("nogrid"); 
		rpE3C->GetUpperPad()->SetLogx(true);
		rpE3C->GetUpperPad()->SetLogy(true);
		rpE3C->GetLowerPad()->SetLogx(true);
		rpE3C->GetLowerPad()->SetLogy(false);
        rpE3C->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 3);
		rpE3C->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");
		rpE3C->GetLowerRefGraph()->GetYaxis()->SetTitleOffset(rpE3C->GetLowerRefGraph()->GetYaxis()->GetTitleOffset()+1.7);
		rpE3C->GetUpperPad()->cd();

		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		TLegend *legendE3C = new TLegend(0.55, 0.2, 0.92, 0.3);  // Position the legend
		legendE3C->SetBorderSize(0); 
		legendE3C->SetFillStyle(0);
		legendE3C->SetTextSize(0.03);
		legendE3C->AddEntry(EEC_hist_smeared[0], "smeared", "p");
		legendE3C->AddEntry(EEC_hist_truth[0], "truth", "p");
		legendE3C->Draw();
		gPad->SetLogx();
		gPad->SetLogy();



	
		cRatio->cd(i+1);
		// gPad->SetLeftMargin(0.3);




		TH1F* ratio_truth = new TH1F(Form("ratio_pt_bin_%d_%d", pTarray[i], pTarray[i+1]), "Energy correlator ratio", EEC_hist_truth[i]->GetNbinsX(),0.0005,1);
		ratio_truth->Divide(E3C_hist_truth[i],EEC_hist_truth[i]); // EEC_hist_truth holds your 2-point (E2C) histogram for truth
		TH1F* ratio_smeared = new TH1F(Form("ratio_pt_bin_%d_%d", pTarray[i], pTarray[i+1]), "Energy correlator ratio", EEC_hist_smeared[i]->GetNbinsX(),0.0005,1);
		ratio_smeared->Divide(E3C_hist_smeared[i],EEC_hist_smeared[i]); // EEC_hist_smeared holds your 2-point (E2C) histogram for smeared
		ratio_truth->SetMarkerStyle(22);
		ratio_smeared->SetMarkerStyle(21);
		ratio_truth->SetMarkerSize(0.7);
		ratio_smeared->SetMarkerSize(0.5);
		ratio_truth->SetMarkerColor(kGreen);
		ratio_smeared->SetMarkerColor(kRed);
		ratio_truth->Draw("pe"); // Draw truth ratio with points and error bars
		ratio_smeared->Draw("pe SAME"); // Overlay smeared ratio
		ratio_truth->GetXaxis()->SetRangeUser(0.0005, 1);
		ratio_truth->GetXaxis()->SetTitle("x_{L}");
		ratio_truth->GetYaxis()->SetTitle("EEEC / EEC");
		ratio_truth->SetTitle(Form("ratio"));
		ratio_truth->SetLineColor(kGreen); 
        ratio_smeared->SetLineColor(kRed); 
		
	
		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		TLegend *legendratio = new TLegend(0.25, 0.5, 0.72, 0.6);  // Position the legend
		legendratio->SetBorderSize(0); 
		legendratio->SetFillStyle(0);
		legendratio->SetTextSize(0.03);
		legendratio->AddEntry(EEC_hist_smeared[0], "smeared", "p");
		legendratio->AddEntry(EEC_hist_truth[0], "truth", "p");
		legendratio->Draw();
		gPad->SetLogx();

	}

	

	cE2C->SaveAs("../../simulations/plots/E2C_pretty_plot_1M.pdf"); // Save the histogram as a PDF file

	cE3C->SaveAs("../../simulations/plots/E3C_pretty_plot_1M.pdf"); // Save the histogram as a PDF file

	cRatio->SaveAs("../../simulations/plots/EEC_ratio_pretty_plot_1M.pdf");
}