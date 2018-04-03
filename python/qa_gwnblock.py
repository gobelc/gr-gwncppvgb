#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks

# corrected, gr_modtool creation said gwncppvgb_swig, not found!
import gwncppvgb.gwncppvgb_swig as gwncppvgb

import time

class qa_gwnblock (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        print "\n========= TEST 1 =========\n"
        myblock = gwncppvgb.gwnblock('block_1', 0, 3, 0, 0)
        #print myblock
        print "myblock.__str__()", myblock.__str__()


        # set up fg
        self.tb.run ()
        # check data

    def test_out_port (self):
        print "\n========= TEST 2 =========\n"
        src = gwncppvgb.gwnblock('block_2', 0, 1, 0, 0)
        dbg = blocks.message_debug()
        #self.tb.msg_connect(src, src.ports_out[0].port,
        #                    dbg, "print")   # ports_out not published!
        self.tb.msg_connect( (src, "port_0"), (dbg, "print") )
  

        self.tb.start ()
        time.sleep(3)
        self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_gwnblock, "qa_gwnblock.xml")
