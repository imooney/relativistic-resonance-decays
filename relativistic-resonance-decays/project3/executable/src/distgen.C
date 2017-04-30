//  distgen.C
//  Created by Isaac Mooney on 4/27/17.

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"

using namespace std;

//Globals
const Double_t Ebeam        = 200.0;        //GeV
const Double_t Gamma        = 3.913623e-15; //GeV
const Double_t mLambda      = 1.115683;     //GeV
const Double_t pT           = 1.0;          //GeV

const Double_t PI = 4.0*atan(1);

int main() {
    //TH1::SetDefaultSumw2();
    //TH2::SetDefaultSumw2();
    
    //create file and tree for the distributions
    TFile *sampledists = new TFile("sampledists.root", "RECREATE");
    TTree *treerand = new TTree("treerand", "sample tree");
    
    //seeding pseudorandom number generator
    auto begin = chrono::high_resolution_clock::now();
    
    Double_t phi, theta, mass, pt;
    TRandom3* phi_rand      = new TRandom3();
    TRandom3* theta_rand    = new TRandom3();
    TRandom3* mass_rand     = new TRandom3();
    TRandom3* pt_rand       = new TRandom3();
    
    auto end = chrono::high_resolution_clock::now();
    
    phi_rand->SetSeed(chrono::duration_cast<chrono::nanoseconds>(end-begin).count());
    theta_rand->SetSeed(0);
    mass_rand->SetSeed(42);
    pt_rand->SetSeed(10);
    
    treerand->Branch("phi",     &phi);
    treerand->Branch("theta",   &theta);
    treerand->Branch("mass",    &mass);
    treerand->Branch("pt",      &pt);
    
    //fill distributions, write to tree
    for(int i = 0; i < 1000000; ++ i) {
        phi = phi_rand->Uniform(0, 2*PI);
        theta = theta_rand->Uniform(0, PI);
        mass = mass_rand->BreitWigner(mLambda, Gamma);
        pt = pt_rand->Exp(1);
        treerand->Fill();
    }
    
    //delete the TRandom3s
    delete phi_rand; delete theta_rand; delete mass_rand; delete pt_rand;
    phi_rand = NULL; theta_rand = NULL; mass_rand = NULL; pt_rand = NULL;
    
    //write to file, close the file
    sampledists->Write();
    sampledists->Close();
    
    return 0;
}
