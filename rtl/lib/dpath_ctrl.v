//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Tubo 8051 cores Data Path controller                        ////
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

module dpath_ctrl (
              rst_n               , 
              clk                 ,

    // gmac core to memory write interface
              g_rx_mem_rd         ,
              g_rx_mem_eop        ,
              g_rx_mem_addr       ,

    // Memory to gmac core interface
              g_tx_mem_wr         ,
              g_tx_mem_eop        ,
              g_tx_mem_addr       ,
              g_tx_mem_req        ,
              g_tx_mem_req_length ,
              g_tx_mem_ack        

      );


input         rst_n                 ; 
input         clk                   ;

// gmac core to memory write interface
input         g_rx_mem_rd           ;
input         g_rx_mem_eop          ;
output [15:0] g_rx_mem_addr         ;

// Memory to gmac core interface
input         g_tx_mem_wr           ;
output        g_tx_mem_eop          ;
output [15:0] g_tx_mem_addr         ;

output        g_tx_mem_req          ;
output [15:0] g_tx_mem_req_length   ;
input         g_tx_mem_ack          ;


reg    [15:0] g_rx_mem_addr         ;
reg    [15:0] g_tx_mem_addr         ;
reg    [15:0] g_tx_mem_req_length   ;
reg    [15:0] rx_plen               ;
reg    [15:0] tx_plen               ;
reg           g_tx_mem_req          ;

wire     g_tx_mem_eop =  ((tx_plen +1) == g_tx_mem_req_length) ? 1'b1 : 1'b0;
 

always @(negedge rst_n or posedge clk) begin
   if(rst_n == 0) begin
      g_rx_mem_addr <= 0;
      g_tx_mem_addr <= 0;
      rx_plen       <= 0;
      tx_plen       <= 0;
   end
   else begin
      //-----------------------------
      // Finding the Frame Size
      //----------------------------
      if(g_rx_mem_rd) begin
         g_rx_mem_addr <= g_rx_mem_addr+1;
         if(g_rx_mem_eop) rx_plen <= 0;
         else rx_plen <= rx_plen +1;
      end
      //------------------------
      // Generate Tx Request at last transfer of RX Req
      //------------------------
      //
      if(g_rx_mem_eop && g_rx_mem_rd)  begin
          g_tx_mem_req_length <= rx_plen+1;
          g_tx_mem_req <= 1;
      end else if (g_tx_mem_ack) begin
          g_tx_mem_req <= 0;
      end

      //------------------------
      // Generate of EOP for TX Interface at last transfer
      //-------------------------
      if(g_tx_mem_wr) begin
         g_tx_mem_addr <= g_tx_mem_addr+1;
         if(g_tx_mem_req_length == (tx_plen +1)) begin
           tx_plen      <= 0;
         end else begin
           tx_plen       <= tx_plen +1;
         end
      end
   end
end


endmodule
