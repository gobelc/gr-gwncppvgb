/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "gwnmsg_sink_impl.h"

/* GWN inclusions */
#include <iostream>  // for string support
#include <string>    // for string concatenation
#include <sstream>   // for to_string
#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages



namespace gr {
  namespace gwncppvgb {

    /*
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */


    /* ********************************************** 
     * Block specific code, REWRITE for a new block.
     ************************************************/


    /* Additional initialization, REWRITE as desired. */
    void
    gwnmsg_sink_impl::added_init() 
    {
    
      d_debug = false;

    }



    /* Timer and input messages processing, REWRITE as desired. */
    void
    gwnmsg_sink_impl::process_data(
      std::string port, pmt::pmt_t pmt_msg)
    {
      std::string d_port = port;

      // verify if message is dictionary (GWN) or other (GR)
      if ( pmt::is_dict(pmt_msg) )
      {
        // GWN message, unpack type, subtype, seq_nr
        std::string type = pmt::symbol_to_string (pmt::dict_ref(
          pmt_msg, pmt::intern("type"), pmt::PMT_NIL));
        std::string subtype = pmt::symbol_to_string (pmt::dict_ref(
          pmt_msg, pmt::intern("subtype"), pmt::PMT_NIL));
        int seq_nr = pmt::to_long (pmt::dict_ref(
          pmt_msg, pmt::intern("seq_nr"), pmt::PMT_NIL)); 
        
        std::cout << "    process_data, TIMER msg from " <<
          d_port << std::endl << "   ";
        std::cout << "    type: " << type << ", subtype: " <<
          subtype << ", seq_nr: " << seq_nr << std::endl;
        pmt::print(pmt_msg);

        if ( type == "Timer" )  
        {           // GWN timer message
          // actions on GWN timer message
          

          
          if (d_debug) {

            // debug stuff
            
          }
        } else {    // GWN non-timer message
          // actions on GWN non-timer message
        }
      } else {  // non-GWN message
        // actions on non GWN message
        if (d_debug) {
          std::cout << "    process_data, STROBE msg from " <<
            d_port << std::endl << "   ";
          pmt::print(pmt_msg);
        }

      }  // end message is GWN or GR

    }




    /* ************************* 
     *  End block specific code.
     ***************************/



    /* GWNPort */
    gwnmsg_sink_impl::GWNPort::GWNPort() {
      d_port = "";      // null port name
      d_port_nr = -1;   // first working port will be 0
      d_debug = false;
      if (d_debug) {
        std::cout << "GWNPort, default constructor"
        << std::endl; }
    }  // end GWNPort

    std::string gwnmsg_sink_impl::GWNPort::__str__() {
      std::string ss = "GWNPort name: " + d_port + 
        ", number: " + std::to_string(d_port_nr) + 
        ", in block: " + d_block->d_name + "\n";
      return ss;
    } 


    /* GWNOutPort */
    gwnmsg_sink_impl::GWNOutPort::
        GWNOutPort(gwnmsg_sink_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort()
   {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNOutPort, constructor" << std::endl;
      }
    }  // end GWNOutPort


    /* GWNInPort */
    gwnmsg_sink_impl::GWNInPort::
        GWNInPort(gwnmsg_sink_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort()
    {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNInPort, constructor" << std::endl;
      }
    }  // end GWNInPort



    /* GWNTimer constructor */
    gwnmsg_sink_impl::GWNTimer::GWNTimer(
      gwnmsg_sink_impl * block, std::string id_timer, 
      pmt::pmt_t pmt_msg, int count, float period_ms) 
      : d_block(block), d_id_timer(id_timer),
        d_pmt_msg(pmt_msg), d_count(count),
        d_period_ms(period_ms)
    {
      d_counter = 0;
      d_suspend = false;  // always emits first message
      d_debug = false;
    }  // end GWNTimer::GWNTimer



    /* GWNTimer, creates thread, timer starts immediately */
    void
    gwnmsg_sink_impl::GWNTimer::start_timer()
    {
    
      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind (
            &gwnmsg_sink_impl::GWNTimer::run_timer, this))
        );
      if (d_debug) {
        std::cout << "    === TIMER STARTED: " << d_id_timer <<
          ", count: " << d_count << std::endl;
      }
    }  // end GWNTimer::start_timer



    /* GWNTimer, runs timer, sleeps and sends message */
    void
    gwnmsg_sink_impl::GWNTimer::run_timer()
    {
      boost::mutex d_mutex;

      while( d_counter < d_count) {
        d_counter = d_counter + 1;
        // first sleep for d_period_ms milliseconds
        boost::this_thread::sleep( 
          boost::posix_time::milliseconds(d_period_ms));
        if ( d_suspend == false ) // timer is not suspended
        {
          // make timer message
          pmt::pmt_t pmt_timer_dict = pmt::make_dict();
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("type"), pmt::intern("Timer")); 
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("subtype"), pmt::intern(d_id_timer));
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("seq_nr"), pmt::from_long(d_counter));
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("message"), d_pmt_msg);
          // mutex lock, post message, unlock
          d_mutex.lock();
          d_block->gr::basic_block::_post(
            d_pmt_timer_port, pmt_timer_dict);
          d_mutex.unlock();
        }  // end if
      } // end while

      if (d_debug) {
        std::cout << "    === TIMER FINISHED: " << d_id_timer <<
          ", counter: " << d_counter << 
          ", thread id: " << d_thread->get_id() << std::endl;
      }
      d_thread->interrupt();  // end thread
      return;
 
    } // end run_timer()



    /* Handles message sent by timer threads */
    void
    gwnmsg_sink_impl::handle_timer_msg(pmt::pmt_t pmt_msg) 
    {
      std::string timer_id = pmt::symbol_to_string( pmt::dict_ref (
        pmt_msg, pmt::intern("subtype"), pmt::PMT_NIL));
      // mutex lock, invoke process_data, unlock
      boost::mutex d_mutex;
      d_mutex.lock();
      process_data(timer_id, pmt_msg);
      d_mutex.unlock();
    }  // end handle_timer_msg




    /* *** GWN gwnmsg_sink *** */

    /* GWN gwnmsg_sink attributes and functions */

    /*std::string gwnmsg_sink_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->d_name; 
      return ss;
    }*/


    /* Handles messages received on message input ports. */
    void gwnmsg_sink_impl::handle_msg (pmt::pmt_t pmt_msg)
    {
      if (d_debug) { 
        std::cout << "...handle input msg: \n";
        pmt::print(pmt_msg);
      } 
      // mutex lock, invoke process_data, unlock
      boost::mutex d_mutex;
      d_mutex.lock();
        //pmt::pmt_t pmt_port_msg = 
        process_data("handle_msg", pmt_msg); // 0);
      d_mutex.unlock();
    }  // end handle_msg



    /* post_message in PMT formatted port */
    void gwnmsg_sink_impl::post_message(pmt::pmt_t pmt_port,
        pmt::pmt_t pmt_msg_send)
    {
      if (d_debug) {
        std::cout << "...post_message on port: " << 
          pmt::symbol_to_string(pmt_port) << std::endl;
        pmt::print(pmt_msg_send);  
      }
      boost::mutex d_mutex;
      d_mutex.lock();
      message_port_pub(pmt_port, pmt_msg_send);
      d_mutex.unlock();
    }  // end post_message



    /* post_message in string formatted port */
    void gwnmsg_sink_impl::post_message(std::string port,
        pmt::pmt_t pmt_msg)
    {
      pmt::pmt_t pmt_port = pmt::intern(port); 
      post_message(pmt_port, pmt_msg);
    }



    /* GNU Radio defaults for block construction */
    gwnmsg_sink::sptr
    //gwnmsg_sink::make(std::string message, int counter)
    gwnmsg_sink::make(  )
         //std::string msg_1, float period_1, int count_1,
        //std::string msg_2, float period_2, int count_2 ) 
    {
      return gnuradio::get_initial_sptr
        (new gwnmsg_sink_impl () ); 
          //msg_1, period_1, count_1, msg_2, period_2, count_2) );  
    }  // end make



    /* gwnmsg_sink: the private constructor */
    //gwnmsg_sink_impl::gwnmsg_sink_impl(std::string message, int counter)
    gwnmsg_sink_impl::gwnmsg_sink_impl 
      (  ) 
      : gr::block("gwnmsg_sink",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) ) //,
      // GWN TAG user arguments constructor init
      

    {
      // GWN block name, ports and timers as block attributes
      d_name = "gwnmsg_sink";
      d_number_in = 1;
      d_number_out = 0;
      d_number_timers = 0;

      d_debug = false; 

      if (d_debug) {
        std::cout << "gwnmsg_sink, constructor, name " << 
          d_name << ", number_in " << d_number_in << 
          ", number_out " << d_number_out << std::endl;
      }

      // gwnmsg_sink, create out ports
      int i;
      d_ports_out.resize(d_number_out);
      std::string out_port;
      pmt::pmt_t pmt_out_port;

      GWNOutPort * out_port_new;
      for ( i=0; i < d_number_out; i++) {
        out_port = "out_port_" + std::to_string(i);
        out_port_new = new GWNOutPort(this, out_port, i);
        d_ports_out[i] = out_port_new;
        pmt_out_port = pmt::string_to_symbol(out_port);
        message_port_register_out(pmt_out_port); 
      }  // end for
      if (d_debug) {    // print items in vector of out ports
        std::cout << "=== gwnmsg_sink, out ports:" << std::endl;
        for ( i=0; i < d_number_out; i++) {
          std::cout << "  out port " << i << 
            ": " << d_ports_out[i]->__str__(); // << std::endl; 
        }
      }

      // gwnmsg_sink, create in ports
      //int i;                  // already declared
      d_ports_in.resize(d_number_in);
      std::string in_port;  
      pmt::pmt_t pmt_in_port;

      GWNInPort * in_port_new;
      for ( i=0; i < d_number_in; i++) {
        in_port = "in_port_" + std::to_string(i);
        in_port_new = new GWNInPort(this, in_port, i);
        d_ports_in[i] = in_port_new;
        pmt_in_port = pmt::string_to_symbol(in_port);

        message_port_register_in(pmt_in_port);
        set_msg_handler(pmt_in_port,
          boost::bind(&gwnmsg_sink_impl::handle_msg, this, _1));
      }  // end for
      if (d_debug) {      // print items in vector of in ports
        std::cout << "=== gwnmsg_sink, in ports:" << std::endl;
        for ( i=0; i < d_number_in; i++) {
          std::cout << "  in port " << i << 
            ": " << d_ports_in[i]->__str__(); // << std::endl; 
        }
      }    // end if


      // register the timer port
      message_port_register_in(pmt::mp("timer_port"));
      set_msg_handler(pmt::mp("timer_port"),
        boost::bind(&gwnmsg_sink_impl::handle_timer_msg, 
        this, _1));
      // create timers
      d_timers.resize(d_number_timers);
      GWNTimer * timer_new;
      for ( i=0; i< d_number_timers; i++) {
        timer_new = new GWNTimer(this, 
          "timer_" + std::to_string(i), 
          pmt::mp("--- An internal TIMER message"), 0, 10000);
          //pmt::mp("--- An internal TIMER message"), 5, 1000);
        d_timers[i] = timer_new;
        //timer_new->start_timer();  // moved to added_init
      }  // end for


      // additional initialization
      //gwnmsg_sink_impl::added_init();
      added_init();

    }  // end constructor


    /* gwnmsg_sink: our virtual destructor */
    gwnmsg_sink_impl::~gwnmsg_sink_impl()
    { }


  } /* namespace gwncppvgb */
} /* namespace gr */

