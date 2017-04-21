//  rrd.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
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
    
    delete eventtest; eventtest = NULL;
    
    return 0;
}

