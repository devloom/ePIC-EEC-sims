#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void n_comparison()
{	TLatex etaLabel;  // global for all pads
	etaLabel.SetNDC();
	etaLabel.SetTextSize(0.05);
	
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);  // <--- This hides the display of titles in plots

	double xmax = 1;
	double xmin =0.05;
	int bins_low=100;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;

	int pTarray[7]={5,10,15,20,25,30,35};

    TFile *file_truth_ep_n_05 = TFile::Open("test_outfile_truth_ep_GenShd_1_R_1_n_05_5M.root", "READ");
    TFile *file_truth_eAu_n_05 = TFile::Open("test_outfile_truth_eAu_GenShd_1_R_1_n_05_5M.root", "READ");
    TFile *file_truth_ep_n_1 = TFile::Open("test_outfile_truth_ep_GenShd_1_R_1_n_1_5M.root", "READ");
    TFile *file_truth_eAu_n_1 = TFile::Open("test_outfile_truth_eAu_GenShd_1_R_1_n_1_5M.root", "READ");
    TFile *file_truth_ep_n_2 = TFile::Open("test_outfile_truth_ep_GenShd_1_R_1_n_2_5M.root", "READ");
    TFile *file_truth_eAu_n_2 = TFile::Open("test_outfile_truth_eAu_GenShd_1_R_1_n_2_5M.root", "READ");


    TCanvas* cE2C = new TCanvas("cE2C", "Histogram Canvas",1500, 1000); // 800 and 600 tell the program the size of the histogram
	cE2C->Divide(4,2);
    
	TCanvas* cE3C = new TCanvas("cE3C", "Histogram Canvas", 1500, 1000); // 800 and 600 tell the program the size of the histogram
	cE3C->Divide(4,2);

	TCanvas* cRatio = new TCanvas("cRatio", "(eAu-ep)/ep Ratio", 1500, 1000);
    cRatio->Divide(4,2);

	
	TH1I *jetcounts_truth_ep_n_05 = nullptr;
	file_truth_ep_n_05->GetObject("jetCounts",jetcounts_truth_ep_n_05);
	TH1I *jetcounts_truth_eAu_n_05 = nullptr;
	file_truth_eAu_n_05->GetObject("jetCounts",jetcounts_truth_eAu_n_05);
    TH1I *jetcounts_truth_ep_n_1 = nullptr;
	file_truth_ep_n_1->GetObject("jetCounts",jetcounts_truth_ep_n_1);
	TH1I *jetcounts_truth_eAu_n_1 = nullptr;
	file_truth_eAu_n_1->GetObject("jetCounts",jetcounts_truth_eAu_n_1);
    TH1I *jetcounts_truth_ep_n_2 = nullptr;
	file_truth_ep_n_2->GetObject("jetCounts",jetcounts_truth_ep_n_2);
	TH1I *jetcounts_truth_eAu_n_2 = nullptr;
	file_truth_eAu_n_2->GetObject("jetCounts",jetcounts_truth_eAu_n_2);
	

	
	std::vector<TH1D*> EEC_hist_truth_ep_n_05(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_n_05(5);
	std::vector<TH1D*> E3C_hist_truth_ep_n_05(5);
    std::vector<TH1D*> E3C_hist_truth_eAu_n_05(5);
    std::vector<TH1D*> EEC_hist_truth_ep_n_1(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_n_1(5);
	std::vector<TH1D*> E3C_hist_truth_ep_n_1(5);
    std::vector<TH1D*> E3C_hist_truth_eAu_n_1(5);
    std::vector<TH1D*> EEC_hist_truth_ep_n_2(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_n_2(5);
	std::vector<TH1D*> E3C_hist_truth_ep_n_2(5);
    std::vector<TH1D*> E3C_hist_truth_eAu_n_2(5);

	


	for (int i = 3; i < 4; ++i) 
	{       
		          
		// Get E2Cs
       
		std::string hist_name_truth_ep_E2C_n_05 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_n_05 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_ep_E2C_n_1 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_n_1 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_ep_E2C_n_2 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_n_2 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

		EEC_hist_truth_ep_n_05[i] = (TH1D*)file_truth_ep_n_05->Get(hist_name_truth_ep_E2C_n_05.c_str());
        EEC_hist_truth_eAu_n_05[i] = (TH1D*)file_truth_eAu_n_05->Get(hist_name_truth_eAu_E2C_n_05.c_str());
        EEC_hist_truth_ep_n_1[i] = (TH1D*)file_truth_ep_n_1->Get(hist_name_truth_ep_E2C_n_1.c_str());
        EEC_hist_truth_eAu_n_1[i] = (TH1D*)file_truth_eAu_n_1->Get(hist_name_truth_eAu_E2C_n_1.c_str());
        EEC_hist_truth_ep_n_2[i] = (TH1D*)file_truth_ep_n_2->Get(hist_name_truth_ep_E2C_n_2.c_str());
        EEC_hist_truth_eAu_n_2[i] = (TH1D*)file_truth_eAu_n_2->Get(hist_name_truth_eAu_E2C_n_2.c_str());

		// Get E3Cs
		
		std::string hist_name_truth_ep_E3C_n_05 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E3C_n_05 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_ep_E3C_n_1 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E3C_n_1 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_ep_E3C_n_2 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E3C_n_2 = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        
		E3C_hist_truth_ep_n_05[i] = (TH1D*)file_truth_ep_n_05->Get(hist_name_truth_ep_E3C_n_05.c_str());
        E3C_hist_truth_eAu_n_05[i] = (TH1D*)file_truth_eAu_n_05->Get(hist_name_truth_eAu_E3C_n_05.c_str());
        E3C_hist_truth_ep_n_1[i] = (TH1D*)file_truth_ep_n_1->Get(hist_name_truth_ep_E3C_n_1.c_str());
        E3C_hist_truth_eAu_n_1[i] = (TH1D*)file_truth_eAu_n_1->Get(hist_name_truth_eAu_E3C_n_1.c_str());
        E3C_hist_truth_ep_n_2[i] = (TH1D*)file_truth_ep_n_2->Get(hist_name_truth_ep_E3C_n_2.c_str());
        E3C_hist_truth_eAu_n_2[i] = (TH1D*)file_truth_eAu_n_2->Get(hist_name_truth_eAu_E3C_n_2.c_str());


		// E2C plots
		cE2C->cd();
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}


		EEC_hist_truth_ep_n_05[i]->Scale(1. / (jetcounts_truth_ep_n_05->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_n_05[i]->Scale(1. / (jetcounts_truth_eAu_n_05->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_ep_n_1[i]->Scale(1. / (jetcounts_truth_ep_n_1->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_n_1[i]->Scale(1. / (jetcounts_truth_eAu_n_1->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_ep_n_2[i]->Scale(1. / (jetcounts_truth_ep_n_2->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_n_2[i]->Scale(1. / (jetcounts_truth_eAu_n_2->GetBinContent(i+1)*binwidth));
		
		std::cout << jetcounts_truth_ep_n_05->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_05->GetBinContent(4) << std::endl;
        std::cout << jetcounts_truth_ep_n_1->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_1->GetBinContent(4) << std::endl;
        std::cout << jetcounts_truth_ep_n_2->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_2->GetBinContent(4) << std::endl;
        

		EEC_hist_truth_ep_n_05[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_ep_n_05[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_ep_n_05[i]->SetMinimum(0.03); // Set y-axis maximum
		EEC_hist_truth_ep_n_05[i]->SetLineColor(kGreen); 
		EEC_hist_truth_ep_n_05[i]->SetMarkerSize(0.5);
		EEC_hist_truth_ep_n_05[i]->SetMarkerStyle(20);
		EEC_hist_truth_ep_n_05[i]->SetMarkerColor(kGreen);
		EEC_hist_truth_ep_n_05[i]->Draw("pe"); // Draw the histogram
		// EEC_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		EEC_hist_truth_ep_n_05[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_ep_n_05[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		// EEC_hist_truth_eAu[i]->Rebin(2);
		// EEC_hist_truth_eAu[i]->Scale(0.5);
        EEC_hist_truth_eAu_n_05[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_eAu_n_05[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_eAu_n_05[i]->SetMinimum(0.03); // Set y-axis maximum
        EEC_hist_truth_eAu_n_05[i]->SetLineColor(kMagenta); 
		EEC_hist_truth_eAu_n_05[i]->SetMarkerSize(0.5);
		EEC_hist_truth_eAu_n_05[i]->SetMarkerStyle(20);
		EEC_hist_truth_eAu_n_05[i]->SetMarkerColor(kMagenta);
		EEC_hist_truth_eAu_n_05[i]->Draw("pe SAME"); // Draw the histogram
		EEC_hist_truth_eAu_n_05[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_eAu_n_05[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");

        EEC_hist_truth_ep_n_1[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_ep_n_1[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_ep_n_1[i]->SetMinimum(0.03); // Set y-axis maximum
		EEC_hist_truth_ep_n_1[i]->SetLineColor(kGreen); 
		EEC_hist_truth_ep_n_1[i]->SetMarkerSize(0.5);
		EEC_hist_truth_ep_n_1[i]->SetMarkerStyle(21);
		EEC_hist_truth_ep_n_1[i]->SetMarkerColor(kGreen);
		EEC_hist_truth_ep_n_1[i]->Draw("pe SAME"); // Draw the histogram
		// EEC_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		EEC_hist_truth_ep_n_1[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_ep_n_1[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		// EEC_hist_truth_eAu[i]->Rebin(2);
		// EEC_hist_truth_eAu[i]->Scale(0.5);
        EEC_hist_truth_eAu_n_1[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_eAu_n_1[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_eAu_n_1[i]->SetMinimum(0.03); // Set y-axis maximum
        EEC_hist_truth_eAu_n_1[i]->SetLineColor(kMagenta); 
		EEC_hist_truth_eAu_n_1[i]->SetMarkerSize(0.5);
		EEC_hist_truth_eAu_n_1[i]->SetMarkerStyle(22);
		EEC_hist_truth_eAu_n_1[i]->SetMarkerColor(kMagenta);
		EEC_hist_truth_eAu_n_1[i]->Draw("pe SAME"); // Draw the histogram
		EEC_hist_truth_eAu_n_1[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_eAu_n_1[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");

        EEC_hist_truth_ep_n_2[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_ep_n_2[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_ep_n_2[i]->SetMinimum(0.03); // Set y-axis maximum
		EEC_hist_truth_ep_n_2[i]->SetLineColor(kGreen); 
		EEC_hist_truth_ep_n_2[i]->SetMarkerSize(0.5);
		EEC_hist_truth_ep_n_2[i]->SetMarkerStyle(21);
		EEC_hist_truth_ep_n_2[i]->SetMarkerColor(kGreen);
		EEC_hist_truth_ep_n_2[i]->Draw("pe SAME"); // Draw the histogram
		// EEC_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		EEC_hist_truth_ep_n_2[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_ep_n_2[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		// EEC_hist_truth_eAu[i]->Rebin(2);
		// EEC_hist_truth_eAu[i]->Scale(0.5);
        EEC_hist_truth_eAu_n_2[i]->GetXaxis()->SetLimits(0.05, 1);
		EEC_hist_truth_eAu_n_2[i]->SetMaximum(1000.); // Set y-axis maximum
    	EEC_hist_truth_eAu_n_2[i]->SetMinimum(0.03); // Set y-axis maximum
        EEC_hist_truth_eAu_n_2[i]->SetLineColor(kMagenta); 
		EEC_hist_truth_eAu_n_2[i]->SetMarkerSize(0.5);
		EEC_hist_truth_eAu_n_2[i]->SetMarkerStyle(22);
		EEC_hist_truth_eAu_n_2[i]->SetMarkerColor(kMagenta);
		EEC_hist_truth_eAu_n_2[i]->Draw("pe SAME"); // Draw the histogram
		EEC_hist_truth_eAu_n_2[i]->GetXaxis()->SetTitle("#theta");
		EEC_hist_truth_eAu_n_2[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		

		
		TLatex ptLabel;
		ptLabel.SetNDC();
		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		ptLabel.DrawLatex(0.2, 0.70, "GenShd=1");
		ptLabel.DrawLatex(0.2, 0.65, "5M events");
		
		TLegend *legendE2C = new TLegend(0.55, 0.2, 0.92, 0.3);  // Position the legend
		legendE2C->SetBorderSize(0); 
		legendE2C->SetFillStyle(0);
		legendE2C->SetTextSize(0.05);
        legendE2C->AddEntry(EEC_hist_truth_ep_n_05[i], "truth_ep n=0.5", "p");
        legendE2C->AddEntry(EEC_hist_truth_eAu_n_05[i], "truth_eAu n=0.5", "p");
        legendE2C->AddEntry(EEC_hist_truth_ep_n_1[i], "truth_ep n=1", "p");
        legendE2C->AddEntry(EEC_hist_truth_eAu_n_1[i], "truth_eAu n=1", "p");
		legendE2C->AddEntry(EEC_hist_truth_ep_n_2[i], "truth_ep n=2", "p");
        legendE2C->AddEntry(EEC_hist_truth_eAu_n_2[i], "truth_eAu n=2", "p");
		TLatex etaLabel;
		etaLabel.SetNDC();
		etaLabel.SetTextSize(0.05);
		etaLabel.DrawLatex(0.55, 0.32, "1 < #eta < 3.5");

		legendE2C->Draw();
		
		gPad->SetLogx();
		gPad->SetLogy();

		


		// E3C plots
		cE3C->cd();
		gPad->SetLeftMargin(0.15);

        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}


		E3C_hist_truth_ep_n_05[i]->Scale(1. / (jetcounts_truth_ep_n_05->GetBinContent(i+1)*binwidth));
        E3C_hist_truth_eAu_n_05[i]->Scale(1. / (jetcounts_truth_eAu_n_05->GetBinContent(i+1)*binwidth));
        E3C_hist_truth_ep_n_1[i]->Scale(1. / (jetcounts_truth_ep_n_1->GetBinContent(i+1)*binwidth));
        E3C_hist_truth_eAu_n_1[i]->Scale(1. / (jetcounts_truth_eAu_n_1->GetBinContent(i+1)*binwidth));
        E3C_hist_truth_ep_n_2[i]->Scale(1. / (jetcounts_truth_ep_n_2->GetBinContent(i+1)*binwidth));
        E3C_hist_truth_eAu_n_2[i]->Scale(1. / (jetcounts_truth_eAu_n_2->GetBinContent(i+1)*binwidth));
		std::cout << jetcounts_truth_ep_n_05->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_05->GetBinContent(4) << std::endl;
        std::cout << jetcounts_truth_ep_n_1->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_1->GetBinContent(4) << std::endl;
        std::cout << jetcounts_truth_ep_n_2->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_n_2->GetBinContent(4) << std::endl;
        
		
        E3C_hist_truth_ep_n_05[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_ep_n_05[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_ep_n_05[i]->SetMinimum(0.03); // Set y-axis maximum
		E3C_hist_truth_ep_n_05[i]->SetLineColor(kGreen); 
		E3C_hist_truth_ep_n_05[i]->SetMarkerSize(0.5);
		E3C_hist_truth_ep_n_05[i]->SetMarkerStyle(21);
		E3C_hist_truth_ep_n_05[i]->SetMarkerColor(kGreen);
		E3C_hist_truth_ep_n_05[i]->Draw("pe"); // Draw the histogram
		// E3C_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		E3C_hist_truth_ep_n_05[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_ep_n_05[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");
		// E3C_hist_truth_eAu[i]->Rebin(2);
		// E3C_hist_truth_eAu[i]->Scale(0.5);
        E3C_hist_truth_eAu_n_05[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_eAu_n_05[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_eAu_n_05[i]->SetMinimum(0.03); // Set y-axis maximum
        E3C_hist_truth_eAu_n_05[i]->SetLineColor(kMagenta); 
		E3C_hist_truth_eAu_n_05[i]->SetMarkerSize(0.5);
		E3C_hist_truth_eAu_n_05[i]->SetMarkerStyle(22);
		E3C_hist_truth_eAu_n_05[i]->SetMarkerColor(kMagenta);
		E3C_hist_truth_eAu_n_05[i]->Draw("pe SAME"); // Draw the histogram
		E3C_hist_truth_eAu_n_05[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_eAu_n_05[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");

        E3C_hist_truth_ep_n_1[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_ep_n_1[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_ep_n_1[i]->SetMinimum(0.03); // Set y-axis maximum
		E3C_hist_truth_ep_n_1[i]->SetLineColor(kGreen); 
		E3C_hist_truth_ep_n_1[i]->SetMarkerSize(0.5);
		E3C_hist_truth_ep_n_1[i]->SetMarkerStyle(21);
		E3C_hist_truth_ep_n_1[i]->SetMarkerColor(kGreen);
		E3C_hist_truth_ep_n_1[i]->Draw("pe SAME"); // Draw the histogram
		// E3C_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		E3C_hist_truth_ep_n_1[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_ep_n_1[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");
		// E3C_hist_truth_eAu[i]->Rebin(2);
		// E3C_hist_truth_eAu[i]->Scale(0.5);
        E3C_hist_truth_eAu_n_1[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_eAu_n_1[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_eAu_n_1[i]->SetMinimum(0.03); // Set y-axis maximum
        E3C_hist_truth_eAu_n_1[i]->SetLineColor(kMagenta); 
		E3C_hist_truth_eAu_n_1[i]->SetMarkerSize(0.5);
		E3C_hist_truth_eAu_n_1[i]->SetMarkerStyle(22);
		E3C_hist_truth_eAu_n_1[i]->SetMarkerColor(kMagenta);
		E3C_hist_truth_eAu_n_1[i]->Draw("pe SAME"); // Draw the histogram
		E3C_hist_truth_eAu_n_1[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_eAu_n_1[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");

        E3C_hist_truth_ep_n_2[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_ep_n_2[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_ep_n_2[i]->SetMinimum(0.03); // Set y-axis maximum
		E3C_hist_truth_ep_n_2[i]->SetLineColor(kGreen); 
		E3C_hist_truth_ep_n_2[i]->SetMarkerSize(0.5);
		E3C_hist_truth_ep_n_2[i]->SetMarkerStyle(21);
		E3C_hist_truth_ep_n_2[i]->SetMarkerColor(kGreen);
		E3C_hist_truth_ep_n_2[i]->Draw("pe SAME"); // Draw the histogram
		// E3C_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		E3C_hist_truth_ep_n_2[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_ep_n_2[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");
		// E3C_hist_truth_eAu[i]->Rebin(2);
		// E3C_hist_truth_eAu[i]->Scale(0.5);
        E3C_hist_truth_eAu_n_2[i]->GetXaxis()->SetLimits(0.05, 1);
		E3C_hist_truth_eAu_n_2[i]->SetMaximum(1000.); // Set y-axis maximum
    	E3C_hist_truth_eAu_n_2[i]->SetMinimum(0.03); // Set y-axis maximum
        E3C_hist_truth_eAu_n_2[i]->SetLineColor(kMagenta); 
		E3C_hist_truth_eAu_n_2[i]->SetMarkerSize(0.5);
		E3C_hist_truth_eAu_n_2[i]->SetMarkerStyle(22);
		E3C_hist_truth_eAu_n_2[i]->SetMarkerColor(kMagenta);
		E3C_hist_truth_eAu_n_2[i]->Draw("pe SAME"); // Draw the histogram
		E3C_hist_truth_eAu_n_2[i]->GetXaxis()->SetTitle("#theta");
		E3C_hist_truth_eAu_n_2[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{dx_{L}}");
		
		

		ptLabel.SetNDC();
		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		ptLabel.DrawLatex(0.2, 0.70, "GenShd=1");
		ptLabel.DrawLatex(0.2, 0.65, "5M events");
		
		TLegend *legendE3C = new TLegend(0.55, 0.2, 0.92, 0.3);  // Position the legend
		legendE3C->SetBorderSize(0); 
		legendE3C->SetFillStyle(0);
		legendE3C->SetTextSize(0.05);
	
		legendE3C->AddEntry(E3C_hist_truth_ep_n_05[i], "truth_ep", "p");
        legendE3C->AddEntry(E3C_hist_truth_eAu_n_05[i], "truth_eAu", "p");
        legendE3C->AddEntry(E3C_hist_truth_ep_n_1[i], "truth_ep", "p");
        legendE3C->AddEntry(E3C_hist_truth_eAu_n_1[i], "truth_eAu", "p");
        legendE3C->AddEntry(E3C_hist_truth_ep_n_2[i], "truth_ep", "p");
        legendE3C->AddEntry(E3C_hist_truth_eAu_n_2[i], "truth_eAu", "p");

		etaLabel.SetNDC();
		etaLabel.SetTextSize(0.05);
		etaLabel.DrawLatex(0.55, 0.32, "1 < #eta < 3.5");
		

		legendE3C->Draw();
		
		gPad->SetLogx();
		gPad->SetLogy();


		cRatio->cd();

		
		TH1D* ratio_truth_ep_n_05 = (TH1D*)E3C_hist_truth_eAu_n_05[i]->Clone("ratio_truth_ep");
		ratio_truth_ep_n_05->Add(E3C_hist_truth_ep_n_05[i], -1);  // eAu - ep
		ratio_truth_ep_n_05->Divide(E3C_hist_truth_ep_n_05[i]);   // (eAu - ep)/ep
		ratio_truth_ep_n_05->SetMarkerStyle(20);
		ratio_truth_ep_n_05->SetMarkerSize(1);
		ratio_truth_ep_n_05->SetMarkerColor(kRed);
		ratio_truth_ep_n_05->Draw("pe"); // Draw truth ratio with points and error bars
		TLine *line = new TLine(0.05, 0, 1, 0);
		line->SetLineStyle(2);
		line->Draw("same");
		ratio_truth_ep_n_05->GetXaxis()->SetRangeUser(0.05, 1);
		// ratio_truth_ep->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_truth_ep_n_05->GetYaxis()->SetRangeUser(-0.05,0.15);
		ratio_truth_ep_n_05->GetXaxis()->SetTitle("#theta");
		ratio_truth_ep_n_05->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		ratio_truth_ep_n_05->SetTitle(Form("ratio"));
		ratio_truth_ep_n_05->SetLineColor(kRed); 
        

        TH1D* ratio_truth_ep_n_1 = (TH1D*)E3C_hist_truth_eAu_n_1[i]->Clone("ratio_truth_ep");
		ratio_truth_ep_n_1->Add(E3C_hist_truth_ep_n_1[i], -1);  // eAu - ep
		ratio_truth_ep_n_1->Divide(E3C_hist_truth_ep_n_1[i]);   // (eAu - ep)/ep
		ratio_truth_ep_n_1->SetMarkerStyle(21);
		ratio_truth_ep_n_1->SetMarkerSize(1);
		ratio_truth_ep_n_1->SetMarkerColor(kBlue);
		ratio_truth_ep_n_1->Draw("pe SAME"); // Draw truth ratio with points and error bars
		
		line->SetLineStyle(2);
		line->Draw("same");
		ratio_truth_ep_n_1->GetXaxis()->SetRangeUser(0.05, 1);
		// ratio_truth_ep->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_truth_ep_n_1->GetYaxis()->SetRangeUser(-0.05,0.15);
		ratio_truth_ep_n_1->GetXaxis()->SetTitle("#theta");
		ratio_truth_ep_n_1->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		ratio_truth_ep_n_1->SetTitle(Form("ratio"));
		ratio_truth_ep_n_1->SetLineColor(kBlue); 


        TH1D* ratio_truth_ep_n_2 = (TH1D*)E3C_hist_truth_eAu_n_2[i]->Clone("ratio_truth_ep");
		ratio_truth_ep_n_2->Add(E3C_hist_truth_ep_n_2[i], -1);  // eAu - ep
		ratio_truth_ep_n_2->Divide(E3C_hist_truth_ep_n_2[i]);   // (eAu - ep)/ep
		ratio_truth_ep_n_2->SetMarkerStyle(22);
		ratio_truth_ep_n_2->SetMarkerSize(1);
		ratio_truth_ep_n_2->SetMarkerColor(kGreen);
		ratio_truth_ep_n_2->Draw("pe SAME"); // Draw truth ratio with points and error bars
		
		line->SetLineStyle(2);
		line->Draw("same");
		ratio_truth_ep_n_2->GetXaxis()->SetRangeUser(0.05, 1);
		// ratio_truth_ep->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_truth_ep_n_2->GetYaxis()->SetRangeUser(-0.05,0.15);
		ratio_truth_ep_n_2->GetXaxis()->SetTitle("#theta");
		ratio_truth_ep_n_2->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		ratio_truth_ep_n_2->SetTitle(Form("ratio"));
		ratio_truth_ep_n_2->SetLineColor(kGreen); 

		TLegend *legendRatio = new TLegend(0.55, 0.2, 0.92, 0.3);
		legendRatio->SetBorderSize(0);
		legendRatio->SetFillStyle(0);
		legendRatio->SetTextSize(0.05);
		legendRatio->AddEntry(ratio_truth_ep_n_05, "truth (eAu-ep)/ep n=0.5", "p");
        legendRatio->AddEntry(ratio_truth_ep_n_1, "truth (eAu-ep)/ep n=1", "p");
        legendRatio->AddEntry(ratio_truth_ep_n_2, "truth (eAu-ep)/ep n=2", "p");
		legendRatio->Draw();

		ptLabel.DrawLatex(0.2, 0.85, "5M events");
		ptLabel.DrawLatex(0.2, 0.8, "GenShd=1");
		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		ptLabel.DrawLatex(0.2, 0.7, "1 < #eta < 3.5");
        // ptLabel.DrawLatex(0.2, 0.65, "n = 0.5");
		

		
        // legendratio->Draw();
		gPad->SetLogx();

	}

	

	cE2C->SaveAs("../../simulations/plots/E2C_eA.pdf"); // Save the histogram as a PDF file

	cE3C->SaveAs("../../simulations/plots/E3C_eA.pdf"); // Save the histogram as a PDF file

	cRatio->SaveAs("../../simulations/plots/E3C_ratio_eA.pdf");
}