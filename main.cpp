//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: 
// Create Date:    2016/12/12
// Design Name:    
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <assert.h> 
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <boost/simulation/pdevs/atomic.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/simulation.hpp>
#include "../vendor/britime.hpp"
#include "../data_structures/message.hpp"
#include "../vendor/input_event_stream.hpp"
#include "../atomics/subnetCDBoost.hpp"
#include "../atomics/senderCDBoost.hpp"
#include "../atomics/receiverCDBoost.hpp"
#include "../atomics/filterPort.hpp"
#include "../atomics/portConversor.hpp"

using namespace std;
using namespace boost::simulation;
using namespace boost::simulation::pdevs;
using namespace boost::simulation::pdevs::basic_models;


using hclock = chrono::high_resolution_clock;
using Time =BRITime;
using Message = Message_t;


int main(int argc, char ** argv) {

  if (argc < 2) {
   cout << "you are using this program with wrong parameters. Te program should be invoked as follow:";
   cout << argv[0] << " path to the input file " << endl;
   return 1; 
  }
  
  string test_file = argv[1];
  ifstream file(test_file);
  string str;
  string file_contents;
  while (getline(file, str)){
    file_contents += str;
    file_contents.push_back('\n');
  }  
    
  string m_input;
  m_input = file_contents;

  cout << "model input:" << endl;
  cout << m_input << endl;

  shared_ptr<istringstream> pointer_iss{ new istringstream{} };
  pointer_iss->str(m_input);

  auto input_test_generator = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss, Time(0),
                [](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
            
        string aux;
        m_next.clear();          
        istringstream ss;        
        ss.str(s);
        ss >> t_next;
        ss >> m_next.port;
        ss >> m_next.value;
        
        string thrash;
        ss >> thrash;
        if ( 0 != thrash.size()) throw exception();         
  });

  auto Subnet1 = make_atomic_ptr<Subnet<Time,Message>>();
  auto Subnet2 = make_atomic_ptr<Subnet<Time,Message>>();
  auto Receiver1 = make_atomic_ptr<Receiver<Time,Message>>();
  auto Sender1 = make_atomic_ptr<Sender<Time,Message>>();
  auto FilterDataOut = make_atomic_ptr<filter<Time,Message>, string>(string("dataOut"));
  auto FilterPacketSentOut = make_atomic_ptr<filter<Time,Message>, string>(string("packetSentOut"));
  auto FilterAckReceivedOut = make_atomic_ptr<filter<Time,Message>, string>(string("ackReceivedOut"));
  auto PortConversorAckIn = make_atomic_ptr<portConversor<Time,Message>, string>(string("ackIn"));

  shared_ptr<flattened_coupled<Time, Message>> TOP(new flattened_coupled<Time, Message>(
  {input_test_generator, Subnet1, Subnet2, Receiver1, Sender1, FilterDataOut, FilterAckReceivedOut, FilterPacketSentOut, PortConversorAckIn},
  {}, 
  {{input_test_generator, Sender1}, {Sender1,FilterDataOut},{Sender1,FilterPacketSentOut}, {Sender1, FilterAckReceivedOut}, {FilterDataOut, Subnet1}, {Subnet1, Receiver1}, {Receiver1, Subnet2}, {Subnet2, PortConversorAckIn}, {PortConversorAckIn, Sender1}}, 
  {FilterAckReceivedOut, FilterPacketSentOut,}
  ));  
  
 
  cout << "Preparing runner" << endl;
  Time initial_time = Time(0); 
  ofstream out_data("abp_output.txt");   
  runner<Time, Message> r(TOP, initial_time, out_data, [](ostream& os, Message m){ os << m;});
  Time end_time = Time(1000000);

  cout << "Starting simulation until time: " << end_time << "seconds" << endl;

  auto start = hclock::now(); 

  end_time = r.runUntil(end_time); 
  auto elapsed = chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>> (hclock::now() - start).count(); 
    
  cout << "Finished simulation with time: " << end_time << "sec" << endl;
  cout << "Simulation took: " << elapsed << "sec" << endl;
  return 0;
}



    

   
