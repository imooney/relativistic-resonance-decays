//  eventgen.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
#include "TTree.h"					//Added by Sam, write file
#include "TFile.h"					//Added by Sam, write file
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include <vector>

using namespace std;

//Globals
const Double_t mLambda      = 1.115683;     //GeV
const Double_t pT           = 1.0;          //GeV
const Double_t mProton      = 0.9382720813; //GeV
const Double_t mPion        = 0.13957018;   //GeV

const Double_t PI           = 4.0*atan(1);

const Double_t num_events   = 100;

//Things to do:
//      1) Would be nice to figure out how to fill branches in t1
//      with the phi and theta that we actually pull from the distribution without messing
//      with much.
//      2) Generalize now to multiple tracks, general parent/general daughters
//      Would also mean we need to add a check about energies (or masses?) of
//      daughters not exceeding parent's.
//      3) Check that I'm boosting correctly.
//      4) Figure out why beam energy doesn't matter????? I think it's because it's
//      supposed to determine the shape of the Breit-Wigner distribution http://iopscience.iop.org/article/10.1088/1742-6596/597/1/012020/pdf
//      5) Background particles?

Event* creation(vector<Double_t> sampledret, Int_t evNum) {
    //STARTING WITH 2-BODY PROTON-PION DECAY - GENERALIZE LATER
    //Lab frame
    Double_t momLambda = pT; //since at mid-rapidity
    Double_t ELambda = sqrt(pow(momLambda,2) + pow(mLambda,2));
    //should we assume it's only along this direction?
    Double_t pxL = momLambda;
    Double_t pyL = 0;
    Double_t pzL = 0;
    Track * tracklambda = new Track(pxL, pyL, pzL, ELambda, 0);
    
    Double_t Ep = (pow(mLambda,2) + pow(mProton,2) - pow(mPion,2)) / (2.0*mLambda);
    Double_t Epi = (pow(mLambda,2) - pow(mProton,2) + pow(mPion,2)) / (2.0*mLambda);
    Double_t mom = sqrt(pow(Ep,2) - pow(mProton,2));
    
    Double_t phi = sampledret[0];
    Double_t theta = sampledret[1];
    
    Double_t px = mom*sin(theta)*cos(phi);
    Double_t py = mom*sin(theta)*sin(phi);
    Double_t pz = mom*cos(theta);
    
    Track * trackprot = new Track(px, py, pz, Ep, 1);
    Track * trackpion = new Track(-px, -py, -pz, Epi, -2);
    
    Event * event = new Event();
    event->SetEventNumber(evNum);
    event->AddTrack(trackprot);
    event->AddTrack(trackpion);
    /*cout << "Before boost: " << '\n';
    event->Print();
    tracklambda->Print();
    cout << "Decays into: " << '\n';
    trackprot->Print();
    trackpion->Print();*/
    
    //NOT SURE ABOUT THIS BEING THE CORRECT WAY TO BOOST
    trackprot->Boost(pxL/ELambda,pyL/ELambda,pzL/ELambda);
    trackpion->Boost(pxL/ELambda,pyL/ELambda,pzL/ELambda);

    //But, momentum & energy of the boosted tracks sum to the lambda momentum & energy
    
    /*cout << "After boost: " << '\n';
    event->Print();
    trackprot->Print();
    trackpion->Print();*/
    
    return event;
}

vector<Double_t> sample(const unsigned index, TTree* samples) {
    //initialize more variables to be sampled if more distributions are added to sampledists.root
    Double_t philoc = -1, thetaloc = -1;
    samples->SetBranchAddress("phi",&philoc);
    samples->SetBranchAddress("theta",&thetaloc);
    //index corresponds to current event number, so kinematics change for each event
    samples->GetEntry(index);
    //add these numbers to the kinematics vector
    vector<Double_t> sampled = {philoc,thetaloc};
    
    //just for testing purposes:
    cout << "PHI: " << sampled[0] << " THETA: " << sampled[1] << endl;
    return sampled;
}

int main() {
    //open sample distributions file and get tree; create file & tree of events
    TFile *f = new TFile("Resonance.root","recreate");
    TTree *t1 = new TTree("t1","Test 1");
    TFile *samp = new TFile("sampledists.root");
    TTree *samples = (TTree*)samp->Get("treerand");
    
    //create an empty event, and put it on a branch
    Event * eventtest = new Event();
    t1->Branch("Event",&eventtest);
    //initialize vector for sampled kinematics
    vector<Double_t> sampledret = {-1,-1};

    //event loop
    for (unsigned i = 0; i < num_events; ++ i) {
        //fill the vector
        sampledret = sample(i, samples);
        //create a decay, using the given sampled kinematics
        eventtest = creation(sampledret, i);
        //fill the tree with this event
        t1->Fill();
    }
    
    delete eventtest; eventtest = NULL;
    
    //close the file of distributions
    samp->Close();
    //write the tree of events to file and close it
    t1->GetCurrentFile()->Write();
    f->Close();
    
    cout << "Tree written to and file closed" << '\n';
    
    return 0;
}
