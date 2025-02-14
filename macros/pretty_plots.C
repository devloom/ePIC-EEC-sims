#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void pretty_plots()
{
    TFile *file_truth = TFile::Open("test_outfile_truth.root", "READ");
    TFile *file_smeared = TFile::Open("test_outfile_smeared.root", "READ");
   

	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
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

	
    if (!jetCountTree_smeared) {																					//get jet counts for scaling
        std::cerr << "Error: `jetCountTree_smeared` not found in `test_outfile_smeared.root`" << std::endl;
        return;
    }
	if (!jetCountTree_truth) {
        std::cerr << "Error: `jetCountTree_truth` not found in `test_outfile_truth.root`" << std::endl;
        return;
    }
	double jetCounts_truth[7] = {0};
	double jetCounts_smeared[7] = {0};
	jetCountTree_truth->SetBranchAddress("jetCounts_truth", jetCounts_truth);
    jetCountTree_smeared->SetBranchAddress("jetCounts_smeared", jetCounts_smeared);
    jetCountTree_truth->GetEntry(0);
    jetCountTree_smeared->GetEntry(0);

  
	
    std::vector<TH1F*> EEC_hist_smeared(6), EEC_hist_truth(6);

    for (int i = 0; i < 6; ++i) {                                      //get the histogram from root outfile
        std::string hist_name_smeared = Form("eec_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);
        std::string hist_name_truth = Form("eec_pt_bin_%d_%d", (int)pTarray[i], (int)pTarray[i+1]);

        EEC_hist_smeared[i] = (TH1F*)file_smeared->Get(hist_name_smeared.c_str());
        EEC_hist_truth[i] = (TH1F*)file_truth->Get(hist_name_truth.c_str());

    
    }


	for (int i=0; i<6; ++i){			//drawing histograms
		c1->cd(i+1);
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}
        EEC_hist_smeared[i]->Scale(1. / (jetCounts_smeared[i]*binwidth));
		EEC_hist_truth[i]->Scale(1. / (jetCounts_truth[i]*binwidth));
		std::cout<<jetCounts_truth[i]<<std::endl;



		EEC_hist_smeared[i]->GetXaxis()->SetRangeUser(0.0005, 10);
		EEC_hist_truth[i]->GetXaxis()->SetRangeUser(0.0005, 10);

		EEC_hist_smeared[i]->SetMaximum(10.); // Set y-axis maximum
    	EEC_hist_smeared[i]->SetMinimum(0.001); // Set y-axis maximum
		EEC_hist_truth[i]->SetMaximum(10.); // Set y-axis maximum
    	EEC_hist_truth[i]->SetMinimum(0.001); // Set y-axis maximum

		EEC_hist_smeared[i]->SetLineColor(kRed); 
        EEC_hist_truth[i]->SetLineColor(kGreen); 

		EEC_hist_smeared[i]->Draw("Pe"); // Draw the histogram
		EEC_hist_truth[i]->Draw("Pe SAME"); // Draw the histogram
		
		EEC_hist_smeared[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_smeared[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		EEC_hist_truth[i]->GetXaxis()->SetTitle("x_{L}");
		EEC_hist_truth[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		
		std::cout << "Histogram: " << EEC_hist_smeared[i]->GetName() 
          << ", Xmin: " << EEC_hist_smeared[i]->GetXaxis()->GetXmin() 
          << ", Xmax: " << EEC_hist_smeared[i]->GetXaxis()->GetXmax() 
          << std::endl;


		TRatioPlot* rp = new TRatioPlot(EEC_hist_truth[i], EEC_hist_smeared[i]);
        rp->SetH1DrawOpt("PE");  // so the top pad is drawn with points
        rp->SetH2DrawOpt("PE");  // so the ratio is drawn with points
        rp->Draw("SAME"); 

		rp->GetUpperPad()->SetLogx(true);
		rp->GetUpperPad()->SetLogy(true);
		rp->GetLowerPad()->SetLogx(true);
		rp->GetLowerPad()->SetLogy(false);
        rp->GetLowerRefGraph()->GetYaxis()->SetRangeUser(0, 6);
		rp->GetLowerRefGraph()->GetYaxis()->SetTitle("ratio");

		// EEC_hist_smeared[i]->SetTitle("2-point EEC"); // Title


		// gPad->Update();
		gPad->SetLogx();
		gPad->SetLogy();

		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);

        TLegend *legend = new TLegend(0.25, 0.25, 0.47, 0.35);  // Position the legend
		legend->SetBorderSize(0); 
		legend->SetFillStyle(0);
		legend->SetTextSize(0.03);
        legend->AddEntry(EEC_hist_smeared[i], "smeared", "l");
        legend->AddEntry(EEC_hist_truth[i], "truth", "l");
		gPad->Update();
		}

	
	c1->cd(7);
	TLatex latex;
	latex.SetNDC();
    latex.SetTextSize(0.03);
	latex.DrawLatex(0.5, 0.85, "1000000 events");
    latex.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
	latex.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
	latex.DrawLatex(0.5, 0.70, "jet radius= 1.0");


    c1->Update();
	c1->SaveAs("../../simulations/plots/E2C_pretty_plot.pdf"); // Save the histogram as a PDF file

}