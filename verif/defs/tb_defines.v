//
// Functional Description:
//
// This has all defines used in TB
//
// *************************************************************************

// For Top level simulations
`define TB_TOP_LEVEL_SIM  1
`define TB_RAND_SEED      0

// RTL - Instance 
`define TB_TOP          tb_top
`define CHIP_TOP        `TB_TOP.chip_top
`define CORE            `TB_TOP.u_core

// TB - Global
`define TB_GLBL         `TB_TOP.tb_glbl

`define TB_AGENTS_GMAC  `TB_TOP.u_tb_eth
`define TB_AGENTS_UART  `TB_TOP.tb_uart

