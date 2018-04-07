

#ifndef BOOST_SIMULATION_PDEVS_SENDER_HPP
#define BOOST_SIMULATION_PDEVS_SENDER_HPP

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
 * @class Sender
*/

template<class TIME, class MSG>
class Sender : public pdevs::atomic<TIME, MSG>{
private:

  //PARAMETRES
  string id;
  string controlIn;
  string ackIn;
  string packetSentOut;
  string ackReceivedOut;
  string dataOut;  
  TIME   preparationTime;
  TIME   timeout;

  //STATE VARIABLES
 
  bool ack;
  int packetNum;
  int totalPacketNum;
  int alt_bit;
  bool sending;
  bool model_active;
  TIME next_internal;

public:

  /**
   * @constructor 
   */

  explicit Sender() noexcept {   
    id               = string("Sender");
    controlIn        = string("controlIn");
    ackIn            = string("ackIn");
    packetSentOut    = string("packetSentOut");
    ackReceivedOut   = string("ackReceivedOut");
    dataOut          = string("dataOut"); 
    preparationTime  = 10;
    timeout          = 20;
    alt_bit          = 0;
    next_internal    = pdevs::atomic<TIME, MSG>::infinity;

    model_active     = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {      
    if (ack){
      if (packetNum < totalPacketNum){
        packetNum ++;
        ack = false;
        alt_bit = (alt_bit + 1) % 2;
        sending = true;
        model_active = true; 
        next_internal = preparationTime;   
      } else {
        model_active = false;
        next_internal = pdevs::atomic<TIME, MSG>::infinity;
      }
    } else{
      if (sending){
        sending = false;
        model_active = true;
        next_internal = timeout;
      } else {
        sending = true;
        model_active = true;
        next_internal = preparationTime;    
      } 
    }   
  }
  /**
   * @advance
   */

  TIME advance() const noexcept {  
    return next_internal;
  }

  /**
   *@output
   */

  vector<MSG> out() const noexcept {    
    MSG aux;
    vector<MSG> out_put;
    if (sending){
      aux.port = dataOut;
      aux.value = packetNum * 10 + alt_bit;
      out_put.push_back(aux);
      aux.port = packetSentOut;
      aux.value = packetNum;
      out_put.push_back(aux);
    }else{
      if (ack){
        aux.port = ackReceivedOut;
        aux.value = alt_bit;
        out_put.push_back(aux);
      }
    }   
    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
    
    if(mb.size() > 1){
      assert(false && "one message per time uniti");
    }
    for(int i = 0; i < mb.size(); i++){
      if( mb[i].port == controlIn  && model_active == false){
        totalPacketNum = static_cast < int > (mb[i].value);
        if (totalPacketNum > 0){
          packetNum = 1;
          ack = false;
          sending = true;
          alt_bit = packetNum % 2;  //set initial alt_bit
          model_active = true;
          next_internal = preparationTime;
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
      } else if( mb[i].port == ackIn  && model_active == true) { 
        if (alt_bit == static_cast < int > (mb[i].value)) {
          ack = true;
          sending = false;
          next_internal = TIME(0);
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
      }
    }   
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    external (mb, t);

  }

};

#endif // BOOST_SIMULATION_PDEVS_SENDER_HPP
