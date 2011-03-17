task gmac_test1;


   events_log = $fopen("../test_log_files/test1_events.log");
   tb_top.u_tb_eth.event_file = events_log;

   tb_top.u_tb_eth.init_port(3'b1, 3'b1, 1'b1, 0);

   tb_top.cpu_write('h1,8'h0,8'h01);  // tx-control
   tb_top.cpu_write('h1,8'h4,8'h65);  // Rx control
   tb_top.cpu_write('h1,8'h8,{16'h0,8'd22,8'd22}); // Tx/Rx IFG

   tb_top.u_tb_eth.set_flow_type(0);//L2 unicast 
   tb_top.u_tb_eth.set_L2_frame_size(1, 64, 84, 1); //, 1, 17, 33, 49, 64
   tb_top.u_tb_eth.set_payload_type(2, 5000,0); //make sure frame size is honored
   tb_top.u_tb_eth.set_L2_protocol(0); // Untagged frame
   tb_top.u_tb_eth.set_L2_source_address(0, 48'h12_34_56_78_9a_bc, 0,0);
   tb_top.u_tb_eth.set_L2_destination_address(0, 48'h16_22_33_44_55_66, 0,0);
   tb_top.u_tb_eth.set_L3_protocol(4); // IPV4
   tb_top.u_tb_eth.set_crc_option(0,0);
   
   fork
     tb_top.u_tb_eth.transmit_packet_sequence(10, 96, 1, 500000);
     begin
         tb_top.u_tb_eth.wait_for_event(3, 0);
         tb_top.u_tb_eth.wait_for_event(3, 0);
     end
   join

  #100000;

  `TB_AGENTS_GMAC.full_mii.status; // test status

  // Check the Transmitted & Received Frame cnt
  if(`TB_AGENTS_GMAC.full_mii.transmitted_packet_count != `TB_AGENTS_GMAC.full_mii.receive_packet_count)
       `TB_GLBL.test_err;

  // Check the Transmitted & Received Byte cnt
  if(`TB_AGENTS_GMAC.full_mii.transmitted_packet_byte_count != `TB_AGENTS_GMAC.full_mii.receive_packet_byte_count)
       `TB_GLBL.test_err;

  if(`TB_AGENTS_GMAC.full_mii.receive_crc_err_count)
       `TB_GLBL.test_err;


endtask

