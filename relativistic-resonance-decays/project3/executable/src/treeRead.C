// reads in the TTree and saves histograms

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "../Event.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TF1.h"
#include <iostream>

using namespace std;

//Globals:
Double_t pt_resolution = 0.100;      //GeV

//fitting function, probably needs some work
Double_t myFitFunction(Double_t *x, Double_t *par)
{
    Double_t peak = par[0]*TMath::Gaus(x[0],par[1],par[2]);
    Double_t background = par[3]+par[4]*x[0]+par[5]*x[0]*x[0];
    
    return (peak+background);
}

//Reads in trees and plots histograms
int main()
{
	//gSystem->Load("$EVENT/libEvent.so");

	TFile *f = new TFile("Resonance.root"); //Resonance.root is name of TFile
	TTree *t1 = (TTree*)f->Get("t1");

	// set branch to our event branch
	Event *currEvent = 0;
	t1->SetBranchAddress("Event", &currEvent);

	int nEvents = t1->GetEntries();


        
	// create histograms to plot
	TH1D *e = new TH1D("e","Energy",200,0,20);
	TH1D *px = new TH1D("px","Momentum in x",200,-10,10);
	TH1D *py = new TH1D("py","Momentum in y",200,-10,10);
	TH1D *pz = new TH1D("pz","Momentum in z",200,-10,10);
	TH1D *pt = new TH1D("pt","Transverse momentum",200,0,20);
	TH1D *im1 = new TH1D("im1","Invariant Mass",1000,0,200);
	TH1D *im2 = new TH1D("im2","Invariant Mass",1000,0,10);
	TH1D *im3 = new TH1D("im3","Invariant Mass",1000,1,2);
	TH2D *pypz = new TH2D("py, pz","Momentum in y and z",200,-10,10,200,-10,10);

	// create track that sums all track pairs together in a single event
	Track *eventSum = new Track(0,0,0,0,0);	
	double eSum;
	double pxSum;
	double pySum;
	double pzSum;

	Double_t ptres = -1;
	TRandom3* pt_smear = new TRandom3();
	pt_smear->SetSeed(4);


	// runs through all events in branch
	for (unsigned i=0; i<nEvents; i++)
	{
		// finds number of tracks in event
		t1->GetEntry(i);
		int nTracks = currEvent->GetNumberOfTracks();
		
		// double for loop sums all pairs of tracks together
		for (unsigned j=0; j<nTracks; j++)
		{
			//plotting all other histograms
			ptres = pt_smear->Gaus(currEvent->GetTrack(j)->Pt(), 0.1);
			currEvent->GetTrack(j)->SetPtEtaPhiE(ptres,
						                          currEvent->GetTrack(j)->Eta(),
						                          currEvent->GetTrack(j)->Phi(),
						                          currEvent->GetTrack(j)->E());
			e->Fill(currEvent->GetTrack(j)->E());
			px->Fill(currEvent->GetTrack(j)->Px());
			py->Fill(currEvent->GetTrack(j)->Py());
			pz->Fill(currEvent->GetTrack(j)->Pz());
			pt->Fill(ptres);
			pypz->Fill(currEvent->GetTrack(j)->Py(), currEvent->GetTrack(j)->Pz());


			for(unsigned k=nTracks-1; k>j; k--)
			{
			  // creates new track for every pair
			  eSum = currEvent->GetTrack(j)->E() + currEvent->GetTrack(k)->E();
			  pxSum = currEvent->GetTrack(j)->Px() + currEvent->GetTrack(k)->Px();
			  pySum = currEvent->GetTrack(j)->Py() + currEvent->GetTrack(k)->Py();
			  pzSum = currEvent->GetTrack(j)->Pz() + currEvent->GetTrack(k)->Pz();
			  
			  eventSum->SetE(eSum);
			  eventSum->SetPx(pxSum);
			  eventSum->SetPy(pySum);
			  eventSum->SetPz(pzSum);
			  
			  // plots every pair on histograms for invariant mass
			  im1->Fill(eventSum->M());
			  im2->Fill(eventSum->M());
			  im3->Fill(eventSum->M());
			}
		}
	}

	// normalizing
	Double_t scale = 1/e->Integral();
	e->Scale(scale);

	scale = 1/px->Integral();
	px->Scale(scale);

	scale = 1/py->Integral();
	py->Scale(scale);

	scale = 1/pz->Integral();
	pz->Scale(scale);

	scale = 1/pt->Integral();
	pt->Scale(scale);

	scale = 1/im1->Integral();
	im1->Scale(scale);

	scale = 1/im2->Integral();
	im2->Scale(scale);

	scale = 1/im3->Integral();
	im3->Scale(scale);

	scale = 1/pypz->Integral();
	pypz->Scale(scale);

	//Drawing
	TCanvas* c1 = new TCanvas("c1","c1",0,0,1500,800);
	c1->Divide(3,2);

	c1->cd(1);	gPad->SetLogy(); e->Draw();
	e->GetXaxis()->SetTitle("energy [GeV]");
	c1->cd(2);	gPad->SetLogy(); px->Draw();
	px->GetXaxis()->SetTitle("momentum [GeV/c]");
	c1->cd(3);	gPad->SetLogy(); py->Draw();
	py->GetXaxis()->SetTitle("momentum [GeV/c]");
	c1->cd(4);	gPad->SetLogy(); pz->Draw();
	pz->GetXaxis()->SetTitle("momentum [GeV/c]");
	c1->cd(5);	gPad->SetLogy(); pt->Draw();
	pt->GetXaxis()->SetTitle("momentum [GeV/c]");

	//Saving
	TImage *img = TImage::Create();
	img->FromPad(c1);
	img->WriteImage("output.png");


	//Fitting invariant mass
	TF1 *fSpectrum = new TF1("fSpectrum",myFitFunction,0,100,6);
	TF1 *fBackground = new TF1("fBackground","pol2",0,100);

	fSpectrum->SetParameters(-2.37404e+06,1.34884e+01,4.60439e-01,1.30450e-04,2.03331e-04,-1.67398e-05);//20,10,2,50,3,2);
	//im1->Fit("fSpectrum");
	im2->Fit("fSpectrum");
	//im3->Fit("fSpectrum");

	Double_t fitParameters[6];

	//fSpectrum->GetParameters(fitParameters);
	fBackground->SetParameters(&fitParameters[3]);

	//Drawing invariant mass plots
	TCanvas* c2 = new TCanvas("c2","c2",0,0,1500,400);
	c2->Divide(3,1);
	c2->cd(1); gPad->SetLogy();	im1->Draw();
	im1->GetXaxis()->SetTitle("mass [GeV/c^2]");
	//fBackground->SetLineColor(kRed); fBackground->Draw("same");
	//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");
	c2->cd(2); gPad->SetLogy();	im2->Draw();
	im2->GetXaxis()->SetTitle("mass [GeV/c^2]");
	fBackground->SetLineColor(kRed); fBackground->Draw("same");
	//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");
	c2->cd(3); gPad->SetLogy();	im3->Draw();
	im3->GetXaxis()->SetTitle("mass [GeV/c^2]");
	//fBackground->SetLineColor(kRed); fBackground->Draw("same");
	//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");

	img->FromPad(c2);
	img->WriteImage("invM.png");

	// drawing the pypz 2D histograms
	TCanvas *c3 = new TCanvas("c3", "c3",0, 0, 1500, 800);
	c3->Divide(2,1);
	c3->cd(1);  gPad->SetLogz(); pypz->Draw("SURF2");
	pypz->GetXaxis()->SetTitle("momentum [GeV/c]"); 
	pypz->GetYaxis()->SetTitle("momentum [GeV/c]");
	c3->cd(2); pypz->Draw();

	img->FromPad(c3);
	img->WriteImage("pxpy.png");

	c3->Close();
	c2->Close();
	c1->Close();
	f->Close();
	
	delete pt_smear; pt_smear = NULL;

	return 0;
}

