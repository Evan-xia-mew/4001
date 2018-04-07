

#ifndef BOOST_SIMULATION_PDEVS_RECEIVER_HPP
#define BOOST_SIMULATION_PDEVS_RECEIVER_HPP

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
#include <boost/simulation/pdevs/atomic.hpp>

#include "../data_structures/message.hpp"

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;

/**
 * @class Receiver
*/

template<class TIME, class MSG>
class Receiver : public pdevs::atomic<TIME, MSG>{
private:

  //PARAMETRES
  string id;  
  TIME  preparationTime;

  //STATE VARIABLES
  int ackNum;
  bool sending;

public:

  /**
   * @constructor 
   */

  explicit Receiver() noexcept {   
    id              = string("Receiver");
    preparationTime = 10;
    ackNum          = 0;
    sending         = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {
    sending = false;    
  }

  /**
   * @advance
   */

  TIME advance() const noexcept {  
    TIME next_internal;
    if (sending) {
      next_internal = preparationTime;
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
    out.port = id;
    out.value = ackNum % 10;
    out_put.push_back(out);
    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
   if(mb.size()>1){
    assert(false && "Only one message at a time");
   }else{
    ackNum = static_cast < int > (mb[0].value);
    sending = true;
   }
   
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    external (mb, 0);
  }
};

#endif // BOOST_SIMULATION_PDEVS_RECEIVER_HPP
