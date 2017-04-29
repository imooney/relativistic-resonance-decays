//  writerrd.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
#include "TTree.h"					//Added by Sam, write file
#include "TFile.h"					//Added by Sam, write file	
#include "TStyle.h"
#include <iostream>

using namespace std;

int main() {
    
    cout << "test" << endl;
    
    Track * tracktest = new Track();
    Track * trackbetter = new Track(1.1,2.3,3.2,4.5,1);
    tracktest->Print();
    trackbetter->Print();
    Event * eventtest = new Event();
    eventtest->Print();
    eventtest->AddTrack(tracktest);
    eventtest->AddTrack(trackbetter);
    eventtest->Print();
    

//////////////////////////////////////////////////////////
/////////////////  WRITE FILE  ///////////////////////////
//////////////////////////////////////////////////////////

   TFile *f = new TFile("Resonance.root","recreate");
   TTree *t1 = new TTree("t1","Test 1");

	//Event *eventtest2 = new Event();

	t1->Branch("Event",&eventtest);
	t1->Fill();

	//forsave->Print();

   t1->Write();
   //f->Write();
   f->Close();
	
	cout<<"Tree written to and file closed"<<endl;

//////////////////////////////////////////////////////////

    delete eventtest; eventtest = NULL;// delete f; delete t1;

    return 0;
}

