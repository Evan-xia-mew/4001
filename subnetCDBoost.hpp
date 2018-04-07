
#ifndef BOOST_SIMULATION_PDEVS_SUBNET_HPP
#define BOOST_SIMULATION_PDEVS_SUBNET_HPP

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
#include <random>
#include <boost/simulation/pdevs/atomic.hpp>

#include "../data_structures/message.hpp"

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;


/**
 * @class Subnet
*/

 class Distribution ;

template<class TIME, class MSG>
class Subnet : public pdevs::atomic<TIME, MSG>{
private:

  //PARAMETRES
  string id;  
  
  //STATE VARIABLES
  bool transmiting;
  int packet;
  int index;

public:

  /**
   * @constructor 
   */

  explicit Subnet() noexcept {   
    id              = string("Subnet");
    transmiting     = false;
    index           = 0;
  }

  /**
   * @Internal
   */

  void internal() noexcept {  
    transmiting = false;    
  }

  /**
   * @advance
   */

  TIME advance() const noexcept { 
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(3.0, 1.0); 
    TIME next_internal;
    if (transmiting) {
      next_internal = TIME(static_cast < int > (round(distribution(generator))));
    }else {
      next_internal = pdevs::atomic<TIME, MSG>::infinity;
    }    
    return next_internal;
  }

  /**
   *@output
   */

  vector<MSG> out() const noexcept {    
    MSG out;
    vector<MSG> out_put;
    if ((double)rand() / (double) RAND_MAX  < 0.95){
      out.port = id;
      out.value = packet;
      out_put.push_back(out);
    }
    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
    index ++;
    if(mb.size()>1) assert(false && "One message at a time");
    packet = static_cast < int > (mb[0].value);
    transmiting = true;
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    external (mb, 0);
  }
};

#endif // BOOST_SIMULATION_PDEVS_SUBNET_HPP
