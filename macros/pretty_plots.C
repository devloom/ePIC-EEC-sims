#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void pretty_plots()
{
	double xmax = 1;
	double xmin =0.0005;
	int bins_low=100;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;

	int pTarray[7]={5,10,15,20,25,30,35};


    TFile *file_truth = TFile::Open("test_outfile_truth.root", "READ");
    TFile *file_smeared = TFile::Open("test_outfile_smeared.root", "READ");

	TCanvas* cE2C = new TCanvas("cE2C", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
	cE2C->Divide(4,2);
    
	TCanvas* cE3C = new TCanvas("cE3C", "Histogram Canvas", 3000, 2000); // 800 and 600 tell the program the size of the histogram
	cE3C->Divide(4,2);

	TH1I *hjetcounts_truth = nullptr;
	file_truth->GetObject("jetCounts",hjetcounts_truth);
	
	TH1I *hjetcounts_smeared = nullptr;
	file_smeared->GetObject("jetCounts",hjetcounts_smeared);
  
	
    std::vector<TH1F*> EEC_hist_smeared(6);
	std::vector<TH1F*> EEC_hist_truth(6);

	std::vector<TH1D*> E3C_hist_smeared(6);
	std::vector<TH1D*> E3C_hist_truth(6);
    for (int i = 0; i < 6; ++i) 
	{                     
		// Get E2Cs
        std::string hist_name_smeared_E2C = Form("eec_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);
        std::string hist_name_truth_E2C = Form("eec_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);

        EEC_hist_smeared[i] = (TH1F*)file_smeared->Get(hist_name_smeared_E2C.c_str());
        EEC_hist_truth[i] = (TH1F*)file_truth->Get(hist_name_truth_E2C.c_str());

		// Get E3Cs
		std::string hist_name_smeared_E3C = Form("E3C_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);
        std::string hist_name_truth_E3C = Form("E3C_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);

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

		EEC_hist_smeared[i]->GetXaxis()->SetRangeUser(0.0005, 10);
		EEC_hist_smeared[i]->SetMaximum(10.); // Set y-axis maximum
    	EEC_hist_smeared[i]->SetMinimum(0.001); // Set y-axis maximum
		EEC_hist_smeared[i]->SetLineColor(kRed); 
		EEC_hist_smeared[i]->Draw("Pe"); // Draw the histogram
		EEC_hist_smeared[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_smeared[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");

		EEC_hist_truth[i]->GetXaxis()->SetRangeUser(0.0005, 10);
		EEC_hist_truth[i]->SetMaximum(10.); // Set y-axis maximum
    	EEC_hist_truth[i]->SetMinimum(0.001); // Set y-axis maximum
        EEC_hist_truth[i]->SetLineColor(kGreen); 
		EEC_hist_truth[i]->Draw("Pe SAME"); // Draw the histogram
		EEC_hist_truth[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_truth[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		
		TRatioPlot* rpE2C = new TRatioPlot(EEC_hist_truth[i], EEC_hist_smeared[i]);
        rpE2C->SetH1DrawOpt("PE");  
        rpE2C->SetH2DrawOpt("PE"); 
        rpE2C->Draw("nogrid"); 
		rpE2C->GetUpperPad()->SetLogx(true);
		rpE2C->GetUpperPad()->SetLogy(true);
		rpE2C->GetLowerPad()->SetLogx(true);
		rpE2C->GetLowerPad()->SetLogy(false);
        rpE2C->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 3);
		rpE2C->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");
		

		EEC_hist_smeared[i]->SetTitle("2-point EEC"); // Title
		gPad->SetLogx();
		gPad->SetLogy();

		TLatex latexE2C;
    	latexE2C.SetNDC();
    	latexE2C.SetTextSize(0.05);

        
		gPad->Update();


		// E3C plots
		cE3C->cd(i+1);
		E3C_hist_smeared[i]->Scale(1. / (hjetcounts_smeared->GetBinContent(i+1)*binwidth));
		E3C_hist_truth[i]->Scale(1. / (hjetcounts_truth->GetBinContent(i+1)*binwidth));

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

		TRatioPlot* rpE3C = new TRatioPlot(E3C_hist_truth[i], E3C_hist_smeared[i]);
        rpE3C->SetH1DrawOpt("PE");  
        rpE3C->SetH2DrawOpt("PE"); 
        rpE3C->Draw("nogrid"); 
		rpE3C->GetUpperPad()->SetLogx(true);
		rpE3C->GetUpperPad()->SetLogy(true);
		rpE3C->GetLowerPad()->SetLogx(true);
		rpE3C->GetLowerPad()->SetLogy(false);
        rpE3C->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 3);
		rpE3C->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");
		rpE3C->GetLowerRefGraph()->GetYaxis()->SetTitleOffset(rpE3C->GetLowerRefGraph()->GetYaxis()->GetTitleOffset()+1.5);

		E3C_hist_smeared[i]->SetTitle("2-point E3C"); // Title
		gPad->SetLogx();
		gPad->SetLogy();

		TLatex latexE3C;
    	latexE3C.SetNDC();
    	latexE3C.SetTextSize(0.05);

		gPad->Update();

	}

	

	cE2C->cd(7);
	TLatex latexE2C;
	latexE2C.SetNDC();
    latexE2C.SetTextSize(0.05);
	latexE2C.DrawLatex(0.5, 0.85, "1000000 events");
    latexE2C.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
	latexE2C.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
	latexE2C.DrawLatex(0.5, 0.70, "jet radius= 1.0");
	TLegend *legendE2C = new TLegend(0.5, 0.5, 0.72, 0.6);  // Position the legend
	legendE2C->SetBorderSize(0); 
	legendE2C->SetFillStyle(0);
	legendE2C->SetTextSize(0.05);
    legendE2C->AddEntry(EEC_hist_smeared[0], "smeared detector", "l");
    legendE2C->AddEntry(EEC_hist_truth[0], "perfect detector", "l");
	legendE2C->Draw();

	cE2C->Update();
	cE2C->SaveAs("../../simulations/plots/E2C_pretty_plot.pdf"); // Save the histogram as a PDF file

	cE3C->cd(7);
	TLatex latexE3C;
	latexE3C.SetNDC();
    latexE3C.SetTextSize(0.05);
	latexE3C.DrawLatex(0.5, 0.85, "1000000 events");
    latexE3C.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
	latexE3C.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
	latexE3C.DrawLatex(0.5, 0.70, "jet radius= 1.0");
	TLegend *legendE3C = new TLegend(0.5, 0.5, 0.72, 0.6);  // Position the legend
	legendE3C->SetBorderSize(0); 
	legendE3C->SetFillStyle(0);
	legendE3C->SetTextSize(0.05);
    legendE3C->AddEntry(EEC_hist_smeared[0], "smeared detector", "l");
    legendE3C->AddEntry(EEC_hist_truth[0], "perfect detector", "l");
	legendE3C->Draw();

	cE3C->Update();
	cE3C->SaveAs("../../simulations/plots/E3C_pretty_plot.pdf"); // Save the histogram as a PDF file

}