

#ifndef BOOST_SIMULATION_PDEVS_BM_PORT_CONVERSOR_H 
#define BOOST_SIMULATION_PDEVS_BM_PORT_CONVERSOR_H

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <boost/simulation/pdevs/atomic.hpp>
#include "../data_structures/message.hpp"
#include "../vendor/britime.hpp"
#include <random>

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;

template<class TIME, class MSG>
class portConversor : public pdevs::atomic<TIME, MSG> {

private:
  string            id;  
  vector <MSG>      msg_port_converted;
            
public:
   
  explicit portConversor(string Id) noexcept {
    id = Id;
    msg_port_converted.clear();               
  }

  void internal() noexcept {
    msg_port_converted.clear();         
  }
   
  TIME advance() const noexcept {
    TIME next_internal;
    if (msg_port_converted.empty()){
      next_internal = pdevs::atomic<TIME, MSG>::infinity; 
    }else{
      next_internal = 0;
    }        
    return next_internal; 
  }
   
  vector<MSG> out() const noexcept { 
    vector<MSG> out_put;
    if(msg_port_converted.empty()){
      out_put.clear();
    }else {
      out_put = msg_port_converted;
    }
    return out_put;
  }
        
  void external(const vector<MSG>& mb, const TIME& t) noexcept { 
    MSG aux;
    for (int i = 0; i<mb.size(); i++){
      aux = mb[i];
      aux.port = id;
      msg_port_converted.push_back(aux);      
    }
  }
         
  void confluence(const vector<MSG>& mb, const TIME& t)  noexcept  {       
    internal();
    external(mb, 0);
  }

};

#endif // BOOST_SIMULATION_PDEVS_BM_PORT_CONVERSOR_H
