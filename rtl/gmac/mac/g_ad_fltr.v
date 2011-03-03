//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Tubo 8051 cores MAC Interface Module                        ////
////                                                              ////
////  This file is part of the Turbo 8051 cores project           ////
////  http://www.opencores.org/cores/turbo8051/                   ////
////                                                              ////
////  Description                                                 ////
////  Turbo 8051 definitions.                                     ////
////                                                              ////
////  To Do:                                                      ////
////    nothing                                                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Dinesh Annayya, dinesha@opencores.org                 ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//`timescale 1ns/100ps
 
 
 module g_ad_fltr(
   	phy_rx_clk,
	rx_reset_n,
	app_clk,
                           scan_mode,
 //MII Interface
 	mi2af_rcv_vld,
        mi2af_strt_rcv,
        mi2af_end_rcv,
        mi2af_rx_data,
 //Configuration Interface
 //RX_FSM Interface
         af2rf_pause_frame
	 );

parameter PAUSE_CONTROL_FRAME = 48'h01_00_00_c2_80_01;

 
 input  	phy_rx_clk;
 input          rx_reset_n;
 input		app_clk;
   input        scan_mode;
 //MII Interface
 input 		mi2af_rcv_vld;
 input          mi2af_strt_rcv;
 input          mi2af_end_rcv;
 input  [7:0]   mi2af_rx_data;
 //Configuration Interface
 //RX_FSM Interface
 output         af2rf_pause_frame;
 
 //****** appliction initializing the address filter registers*******
 
 reg  pause1_valid;
  

  //***********************************************************************************
  
  reg [2:0] addr_length_ptr;  
 
  wire      rf2af_addr_chk =  (mi2af_rcv_vld && addr_length_ptr < 5);

  always @(posedge phy_rx_clk 
	   or negedge rx_reset_n)
    begin
      if(!rx_reset_n)
	addr_length_ptr <= 3'b0;
      else if(mi2af_strt_rcv)
	addr_length_ptr <= 3'b0;
      else if(mi2af_rcv_vld & rf2af_addr_chk)
	addr_length_ptr <= addr_length_ptr + 1;
    end
  
    reg [7:0] pause_filt_reg;
    wire [47:0] pause_reg;

    
    assign pause_reg = PAUSE_CONTROL_FRAME;
  
      always @(addr_length_ptr or pause_reg )
	begin
	  case(addr_length_ptr) 
	    4'h0:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[7:0];
	      end
	    4'h1:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[15:8];
	      end
	    4'h2:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[23:16];
	      end
	    4'h3:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[31:24];
	      end
	    4'h4:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[39:32];
	      end
	    4'h5:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[47:40];
	      end
	    default:	
	      begin
	        pause_filt_reg[7:0] = pause_reg[7:0];
	      end
	  endcase
	end

  reg  first_byte;
  always @(posedge phy_rx_clk or negedge rx_reset_n)
    begin
      if(!rx_reset_n)
	first_byte <= 0;
      else if(mi2af_strt_rcv)
	first_byte <= 1;
      else if(rf2af_addr_chk && mi2af_rcv_vld)
	first_byte <= 0;
    end
    
     reg pause0bit_valid,pause1bit_valid,pause2bit_valid,pause3bit_valid,
	 pause4bit_valid,pause5bit_valid,pause6bit_valid,pause7bit_valid;
	 
     
     
     always @(mi2af_rx_data or pause_filt_reg)
    begin
	pause0bit_valid = !(pause_filt_reg[0] ^ mi2af_rx_data[0]); 
	pause1bit_valid = !(pause_filt_reg[1] ^ mi2af_rx_data[1]); 
	pause2bit_valid = !(pause_filt_reg[2] ^ mi2af_rx_data[2]); 
	pause3bit_valid = !(pause_filt_reg[3] ^ mi2af_rx_data[3]); 
	pause4bit_valid = !(pause_filt_reg[4] ^ mi2af_rx_data[4]); 
	pause5bit_valid = !(pause_filt_reg[5] ^ mi2af_rx_data[5]); 
	pause6bit_valid = !(pause_filt_reg[6] ^ mi2af_rx_data[6]); 
	pause7bit_valid = !(pause_filt_reg[7] ^ mi2af_rx_data[7]); 
    end

    wire pausetmp_valid;
    
    assign pausetmp_valid =  pause0bit_valid && pause1bit_valid 
                          && pause2bit_valid && pause3bit_valid 
			  && pause4bit_valid && pause5bit_valid
			  && pause6bit_valid && pause7bit_valid
			  && rf2af_addr_chk;

  always @(posedge phy_rx_clk 
	   or negedge rx_reset_n)
    begin
      if(!rx_reset_n)
	pause1_valid <= 0;
      else if(mi2af_end_rcv)
	pause1_valid <= 0;
      else if(first_byte && pausetmp_valid)
	pause1_valid <= 1;
      else if(pause1_valid && pausetmp_valid)
	pause1_valid <= 1;
      else if(!rf2af_addr_chk)
	pause1_valid <= pause1_valid;
      else if(!pausetmp_valid && mi2af_rcv_vld)
	pause1_valid <= 1'b0;
    end
 //*****************************OUTPUTS**********************************************************
 
  wire af2rf_pause_frame;  
  
 
  assign af2rf_pause_frame = pause1_valid; 

 endmodule

