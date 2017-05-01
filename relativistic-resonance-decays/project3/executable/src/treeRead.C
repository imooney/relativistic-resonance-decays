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
#include "TMath.h"


using namespace std;

Double_t myFitFunction(Double_t *x, Double_t *par)
{
	Double_t peak = par[0]*TMath::Gaus(x[0],par[1],par[2]);
	Double_t background = par[3]+par[4]*x[0]+par[5]*x[0]*x[0];

	return (peak+background);
}

void treeRead() 
{
    //TH1::SetDefaultSumw2();
    //TH2::SetDefaultSumw2();
    //gSystem->Load("$EVENT/libEvent.so");
    
    TFile *f2 = new TFile("tests.root","recreate");
    TTree *t2 = new TTree("t2","Test 2");
    
    double eSum;
    double pxSum;
    double pySum;
    double pzSum;
    double imt;
    
    t2->Branch("e",&eSum); t2->Branch("px",&pxSum); t2->Branch("py",&pySum);
    t2->Branch("pz",&pzSum); t2->Branch("imt",&imt);
    
    // create 5 histograms to plot
    TH1D *e = new TH1D("e", "Energy", 1000, 0, 200);
    TH1D *px = new TH1D("px", "Momentum in x", 1000, -200, 200);//100, -1, 2);
    TH1D *py = new TH1D("py", "Momentum in y", 1000, -200, 200);//100, -0.5, 0.5);
    TH1D *pz = new TH1D("pz", "Momentum in z", 1000, -200, 200);//100, -0.5, 0.5);
    TH1D *pt = new TH1D("pt", "Transverse momentum", 1000, -200, 200);
    TH1D *im = new TH1D("im", "Invariant Mass", 1000, -1,300);//1000, 0, 10);
    TH2D *pypz = new TH2D("pypz", "pypz", 1000,-200, 200, 1000,-200,200);//100,-0.5,0.5,100,-0.5,0.5);
    
    TFile *f = new TFile("Resonance.root"); //Resonance.root is name of TFile
    TTree *t1 = (TTree*)f->Get("t1");
    
    // set branch to our event branch
    Event *currEvent = 0;
    t1->SetBranchAddress("Event", &currEvent);
    
    int nEvents = t1->GetEntries();
    
    // create track that sums all track pairs together in a single event
    Track *eventSum = new Track(0,0,0,0,0);

	// create histograms to plot
	TH1D *e = new TH1D("e", "Energy", 1000, 0, 200);
	TH1D *px = new TH1D("px", "Momentum in x", 1000, -200, 200);
	TH1D *py = new TH1D("py", "Momentum in y", 1000, -200, 200);
	TH1D *pz = new TH1D("pz", "Momentum in z", 1000, -200, 200);
	TH1D *pt = new TH1D("pt", "Transverse momentum", 1000, -200, 200);
	TH1D *im1 = new TH1D("im", "Invariant Mass", 1000, 0,200);
	TH1D *im2 = new TH1D("im", "Invariant Mass", 1000, 0,10);
	TH1D *im3 = new TH1D("im", "Invariant Mass", 1000, 1.1,1.2);
	TH2D *pypz = new TH2D("pypz", "pypz", 1000,-200, 200, 1000,-200,200);

	// create new file and ttree to save our file into
	TFile *f = new TFile("Resonance.root"); //Resonance.root is name of TFile
	TTree *t1 = (TTree*)f->Get("t1");

	// set branch to our event branch
	Event *currEvent = 0;
	t1->SetBranchAddress("Event", &currEvent);
	 
	int nEvents = t1->GetEntries();

	// create track that sums all track pairs together in a single event
	Track *eventSum = new Track(0,0,0,0,0);
	double eSum;
	double pxSum;
	double pySum;
	double pzSum;
	 
	// runs through all events in branch
	for (unsigned i=0; i<nEvents; i++) 
	{
		// finds number of tracks in event
		t1->GetEntry(i);
     	int nTracks = currEvent->GetNumberOfTracks();

		// double for loop sums all pairs of tracks together
     	for (unsigned j=0; j<nTracks; j++) 
		{

			e->Fill(currEvent->GetTrack(j)->E());
			px->Fill(currEvent->GetTrack(j)->Px());
			py->Fill(currEvent->GetTrack(j)->Py());
			pz->Fill(currEvent->GetTrack(j)->Pz());
			pt->Fill(currEvent->GetTrack(j)->Pt());
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

	c1->cd(1);	e->Draw();
	c1->cd(2);	px->Draw();
	c1->cd(3);	py->Draw();
	c1->cd(4);	pz->Draw();
	c1->cd(5);	pt->Draw();
	c1->cd(6);	pypz->Draw("SURF2");

	//Saving
	TImage *img = TImage::Create();
	img->FromPad(c1);
	img->WriteImage("output.png");


	//Fitting invariant mass
	TF1 *fSpectrum = new TF1("fSpectrum",myFitFunction,0,100,6);
	TF1 *fBackground = new TF1("fBackground","pol2",0,100);

	fSpectrum->SetParameters(20,10,2,50,3,2);
	im1->Fit("fSpectrum");
	im2->Fit("fSpectrum");
	im3->Fit("fSpectrum");
	
	Double_t fitParameters[6];

	fSpectrum->GetParameters(fitParameters);
	fBackground->SetParameters(&fitParameters[3]);

	TCanvas* c2 = new TCanvas("c2","c2",0,0,1500,400);
	c2->Divide(3,1);
	c2->cd(1); gPad->SetLogy();	im1->Draw(); 
		fBackground->SetLineColor(kRed); fBackground->Draw("same"); 
		//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");
	c2->cd(2); gPad->SetLogy();	im2->Draw(); 
		fBackground->SetLineColor(kRed); fBackground->Draw("same"); 
		//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");
	c2->cd(3); gPad->SetLogy();	im3->Draw(); 
		fBackground->SetLineColor(kRed); fBackground->Draw("same"); 
		//fSpectrum->SetLineColor(kBlack); fSpectrum->Draw("same");
	
	img->FromPad(c2);
	img->WriteImage("invM.png");

	c2->Close();
	c1->Close();

	f->Close(); 

	delete e; delete px; delete py; delete pz; delete pt;
	delete pypz; delete im1; delete im2; delete im3;
	delete f; delete c1; delete c2;

	
}

// loop on a tree
// read a sub-branch only (the branch containing the number of tracks)
// read all branches, when selection is positive
// histogram the results of a member function of the event object

