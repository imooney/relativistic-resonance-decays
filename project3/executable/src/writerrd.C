//  rrd.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
#include "TTree.h"					//Added by Sam, write file
#include "TFile.h"					//Added by Sam, write file	
#include "TStyle.h"
#include <iostream>
#include "TF1.h"
#include "TRandom.h"

using namespace std;

Event* creation();
void writeFile(Event &eventtest);

const double Ebeam = 200; //GeV
const double mLambda = 1.115; //GeV
const double pT = 1; //GeV
const double mProton = 0.939; //GeV
const double mPion = 0.138; //GeV
const double PI = 4*atan(1);

int main() 
{	
	Event *eventtest = creation();


//////////////////////////////////////////////////////////

	/*TF1* func = new TF1("Isaac's funk", "TRandom::Uniform()",-5,6);


	for(int i=0;i<1000;i++)
	{
		cout<<func->GetRandom()<<"\n";
	}*/
	
	/*Double_t face;
	
	for(int i=0;i<1000;i++)
	{
		face = TRandom::Uniform(0,10);
		cout<<face<<"\n";
	}*/
    
	

//////////////////////////////////////////////////////////


	writeFile(*eventtest);

   delete eventtest; eventtest = NULL;

   return 0;
}

Event* creation()
{
	//Decay
	
   Track * tracktest = new Track();
   Track * trackbetter = new Track(1.1,2.3,3.2,4.5,1);
   tracktest->Print();
   trackbetter->Print();
   Event * eventtest = new Event();
   eventtest->Print();
   eventtest->AddTrack(tracktest);
   eventtest->AddTrack(trackbetter);
   eventtest->Print();

	return eventtest;
}

void writeFile(Event &eventtest)
{
	TFile *f = new TFile("Resonance.root","recreate");
   TTree *t1 = new TTree("t1","Test 1");

	t1->Branch("Event",&eventtest);
	t1->Fill();

   t1->Write();
   f->Close();
	
	cout<<"Tree written to and file closed"<<endl;
}

