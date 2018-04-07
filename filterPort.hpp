/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
* This model filters the message based on the port field of the incomming message
*/

#ifndef BOOST_SIMULATION_PDEVS_BM_FILTER_H 
#define BOOST_SIMULATION_PDEVS_BM_FILTER_H

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
class filter : public pdevs::atomic<TIME, MSG> {

private:
  string            id;
  vector <MSG>      msg_pass_filter;
            
public:
   
  explicit filter(string Id) noexcept {
    id              = Id;
    msg_pass_filter.clear();               
  }

  void internal() noexcept {
    msg_pass_filter.clear();    
  }
   
  TIME advance() const noexcept {        
    TIME next_internal;
    if (msg_pass_filter.empty()){
      next_internal = pdevs::atomic<TIME, MSG>::infinity; 
    }else{
      next_internal = 0;
    }        
    return next_internal; 
  }
   
  vector<MSG> out() const noexcept {         
    vector<MSG> out_put;
    if(msg_pass_filter.empty()){
      out_put.clear();
    }else {
      out_put = msg_pass_filter;
    }
    return out_put;
  }
        
  void external(const vector<MSG>& mb, const TIME& t) noexcept {  
    for (int i = 0; i<mb.size(); i++){
      if (mb[i].port == id) msg_pass_filter.push_back(mb[i]);    
    }
  }
         
  void confluence(const vector<MSG>& mb, const TIME& t)  noexcept  {       
    internal();
    external(mb, 0);
  }

};

#endif // BOOST_SIMULATION_PDEVS_BM_FILTER_H
