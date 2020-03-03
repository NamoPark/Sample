#pragma once
// Start ---------- Prefix' ----------
// ROIC Version
#define ROIC130	0x03000000
#define ROIC132 0x03200000
// Send to firmware
#define P_CMD	0x00010000
#define P_AUX	0x00020000

// Recv from firmware
#define P_ACK	0x10000000
#define P_MSG	0x20000000
// ---------- Prefix' ---------- End

// Start ---------- Commands : Core ----------
#define CMD_INIT			P_CMD + 0x0001
#define CMD_ABORT			P_CMD + 0x0002
#define CMD_START			P_CMD + 0x0003
#define CMD_READYIN			P_CMD + 0x0013
#define CMD_SET_FPGA_PARAM	P_CMD + 0x0020
#define CMD_SET_ROIC_PARAM	P_CMD + 0x0021
#define CMD_MULTI_FRAME		P_CMD + 0x0022

#define CMD_132_INIT			P_CMD + ROIC132 + 0x0001
#define CMD_132_ABORT			P_CMD + ROIC132 + 0x0002
#define CMD_132_START			P_CMD + ROIC132 + 0x0003
#define CMD_132_READYIN			P_CMD + ROIC132 + 0x0013
#define CMD_132_SET_FPGA_PARAM	P_CMD + ROIC132 + 0x0020
#define CMD_132_SET_ROIC_PARAM	P_CMD + ROIC132 + 0x0021
#define CMD_132_MULTI_FRAME		P_CMD + ROIC132 + 0x0022

#define CMD_130_INIT			P_CMD + ROIC130 + 0x0001
#define CMD_130_ABORT			P_CMD + ROIC130 + 0x0002
#define CMD_130_START			P_CMD + ROIC130 + 0x0003
#define CMD_130_READYIN			P_CMD + ROIC130 + 0x0013
#define CMD_130_SET_FPGA_PARAM	P_CMD + ROIC130 + 0x0020
#define CMD_130_SET_ROIC_PARAM	P_CMD + ROIC130 + 0x0021
#define CMD_130_MULTI_FRAME		P_CMD + ROIC130 + 0x0022
// ---------- Commands : Core ---------- End

// Start ---------- Commands : Auxiliary ----------
#define CMD_AUX_VER_FIRM		P_AUX + 0x0010  // Firmware version
#define CMD_AUX_VER_FPGA		P_AUX + 0x0011  // FPGA version
#define CMD_AUX_VER_MAIN		P_AUX + 0x0012  // Main B/D version
#define CMD_AUX_VER_TFTP		P_AUX + 0x0013  // TFT Panel Info.
#define CMD_AUX_VER_CSIS		P_AUX + 0x0014  // CSI Serial Num
#define CMD_AUX_VER_LICENSE		P_AUX + 0x0015  // LIENSE
#define CMD_AUX_SLEEP			P_AUX + 0x0016  // LIENSE
#define CMD_AUX_FPGA_PARAM		P_AUX + 0x0020
#define CMD_AUX_ROIC_PARAM		P_AUX + 0x0021
#define CMD_AUX_TEST_PTRN		P_AUX + 0x00E0  // Get test-pattern
#define CMD_AUX_RST_HARD		P_AUX + 0x00F0  // Hard Reset

#define CMD_130_AUX_VER_FIRM		P_AUX + ROIC130 + 0x0010  // Firmware version
#define CMD_130_AUX_VER_FPGA		P_AUX + ROIC130 + 0x0011  // FPGA version
#define CMD_130_AUX_VER_MAIN		P_AUX + ROIC130 + 0x0012  // Main B/D version
#define CMD_130_AUX_VER_TFTP		P_AUX + ROIC130 + 0x0013  // TFT Panel Info.
#define CMD_130_AUX_VER_CSIS		P_AUX + ROIC130 + 0x0014  // CSI Serial Num
#define CMD_130_AUX_VER_LICENSE		P_AUX + ROIC130 + 0x0015  // LIENSE
#define CMD_130_AUX_SLEEP			P_AUX + ROIC130 + 0x0016  // LIENSE
#define CMD_130_AUX_FPGA_PARAM		P_AUX + ROIC130 + 0x0020
#define CMD_130_AUX_ROIC_PARAM		P_AUX + ROIC130 + 0x0021
#define CMD_130_AUX_TEST_PTRN		P_AUX + ROIC130 + 0x00E0  // Get test-pattern
#define CMD_130_AUX_RST_HARD		P_AUX + ROIC130 + 0x00F0  // Hard Reset

#define CMD_132_AUX_VER_FIRM		P_AUX + ROIC132 + 0x0010  // Firmware version
#define CMD_132_AUX_VER_FPGA		P_AUX + ROIC132 + 0x0011  // FPGA version
#define CMD_132_AUX_VER_MAIN		P_AUX + ROIC132 + 0x0012  // Main B/D version
#define CMD_132_AUX_VER_TFTP		P_AUX + ROIC132 + 0x0013  // TFT Panel Info.
#define CMD_132_AUX_VER_CSIS		P_AUX + ROIC132 + 0x0014  // CSI Serial Num
#define CMD_132_AUX_VER_LICENSE		P_AUX + ROIC132 + 0x0015  // LIENSE
#define CMD_132_AUX_SLEEP			P_AUX + ROIC132 + 0x0016  // LIENSE
#define CMD_132_AUX_FPGA_PARAM		P_AUX + ROIC132 + 0x0020
#define CMD_132_AUX_ROIC_PARAM		P_AUX + ROIC132 + 0x0021
#define CMD_132_AUX_TEST_PTRN		P_AUX + ROIC132 + 0x00E0  // Get test-pattern
#define CMD_132_AUX_RST_HARD		P_AUX + ROIC132 + 0x00F0  // Hard Reset

// ---------- Commands : Auxiliary ---------- End

// Start ---------- Acknowledgement ----------
#define ACK_INIT_DONE			P_ACK + CMD_INIT
#define ACK_READYDONE			P_ACK + CMD_READYIN
#define ACK_ABORT				P_ACK + CMD_ABORT
#define ACK_START				P_ACK + CMD_START
#define ACK_SET_FPGA_PARAM		P_ACK + CMD_SET_FPGA_PARAM
#define ACK_SET_ROIC_PARAM		P_ACK + CMD_SET_ROIC_PARAM

#define ACK_AUX_RST_HARD		P_ACK + CMD_AUX_RST_HARD
#define ACK_AUX_VER_FIRM		P_ACK + CMD_AUX_VER_FIRM  // string
#define ACK_AUX_VER_FPGA		P_ACK + CMD_AUX_VER_FPGA  // string
#define ACK_AUX_VER_MAIN		P_ACK + CMD_AUX_VER_MAIN  // string
#define ACK_AUX_VER_TFTP		P_ACK + CMD_AUX_VER_TFTP  // string
#define ACK_AUX_VER_CSIS		P_ACK + CMD_AUX_VER_CSIS  // string
#define ACK_AUX_VER_LICENSE		P_ACK + CMD_AUX_VER_LICENSE  // string
#define ACK_AUX_FPGA_PARAM		P_ACK + CMD_AUX_FPGA_PARAM
#define ACK_AUX_ROIC_PARAM		P_ACK + CMD_AUX_ROIC_PARAM

//#define ACK_132_INIT_DONE			P_ACK + ROIC132 + CMD_INIT
//#define ACK_132_READYDONE			P_ACK + ROIC132 + CMD_READYIN
//#define ACK_132_ABORT				P_ACK + ROIC132 + CMD_ABORT
//#define ACK_132_START				P_ACK + ROIC132 + CMD_START
//#define ACK_132_SET_FPGA_PARAM		P_ACK + ROIC132 + CMD_SET_FPGA_PARAM
//#define ACK_132_SET_ROIC_PARAM		P_ACK + ROIC132 + CMD_SET_ROIC_PARAM
//
//#define ACK_132_AUX_RST_HARD		P_ACK + ROIC132 + CMD_AUX_RST_HARD
//#define ACK_132_AUX_VER_FIRM		P_ACK + ROIC132 + CMD_AUX_VER_FIRM  // string
//#define ACK_132_AUX_VER_FPGA		P_ACK + ROIC132 + CMD_AUX_VER_FPGA  // string
//#define ACK_132_AUX_VER_MAIN		P_ACK + ROIC132 + CMD_AUX_VER_MAIN  // string
//#define ACK_132_AUX_VER_TFTP		P_ACK + ROIC132 + CMD_AUX_VER_TFTP  // string
//#define ACK_132_AUX_VER_CSIS		P_ACK + ROIC132 + CMD_AUX_VER_CSIS  // string
//#define ACK_132_AUX_VER_LICENSE		P_ACK + ROIC132 + CMD_AUX_VER_LICENSE  // string
//#define ACK_132_AUX_FPGA_PARAM		P_ACK + ROIC132 + CMD_AUX_FPGA_PARAM
//#define ACK_132_AUX_ROIC_PARAM		P_ACK + ROIC132 + CMD_AUX_ROIC_PARAM
//
//#define ACK_130_INIT_DONE			P_ACK + ROIC130 + CMD_INIT
//#define ACK_130_READYDONE			P_ACK + ROIC130 + CMD_READYIN
//#define ACK_130_ABORT				P_ACK + ROIC130 + CMD_ABORT
//#define ACK_130_START				P_ACK + ROIC130 + CMD_START
//#define ACK_130_SET_FPGA_PARAM		P_ACK + ROIC130 + CMD_SET_FPGA_PARAM
//#define ACK_130_SET_ROIC_PARAM		P_ACK + ROIC130 + CMD_SET_ROIC_PARAM
//
//#define ACK_130_AUX_RST_HARD		P_ACK + ROIC130 + CMD_AUX_RST_HARD
//#define ACK_130_AUX_VER_FIRM		P_ACK + ROIC130 + CMD_AUX_VER_FIRM  // string
//#define ACK_130_AUX_VER_FPGA		P_ACK + ROIC130 + CMD_AUX_VER_FPGA  // string
//#define ACK_130_AUX_VER_MAIN		P_ACK + ROIC130 + CMD_AUX_VER_MAIN  // string
//#define ACK_130_AUX_VER_TFTP		P_ACK + ROIC130 + CMD_AUX_VER_TFTP  // string
//#define ACK_130_AUX_VER_CSIS		P_ACK + ROIC130 + CMD_AUX_VER_CSIS  // string
//#define ACK_130_AUX_VER_LICENSE		P_ACK + ROIC130 + CMD_AUX_VER_LICENSE  // string
//#define ACK_130_AUX_FPGA_PARAM		P_ACK + ROIC130 + CMD_AUX_FPGA_PARAM
//#define ACK_130_AUX_ROIC_PARAM		P_ACK + ROIC130 + CMD_AUX_ROIC_PARAM

// ---------- Acknowledgement ---------- End

// Start ---------- Messages ----------
#define MSG_STATUS			P_MSG + 0x01
#define MSG_CONNECTION		P_MSG + 0x03
#define MSG_REQCONN			P_MSG + 0x04
#define MSG_BAR				P_MSG + 0x05
#define MSG_WLAN_INFO		P_MSG + 0x06
#define MSG_XRAYSTART		P_MSG + 0x21
#define MSG_XRAYSTOP		P_MSG + 0x22
#define MSG_READYON			P_MSG + 0x23
#define MSG_READYOFF		P_MSG + 0x24
#define MSG_THERMAL			P_MSG + 0x31
#define MSG_INITIALIZING	P_MSG + 0x41
#define MSG_AT_INFO			P_MSG + 0x51
#define MSG_AT_READY		P_MSG + 0x52
#define MSG_AT_MODE			P_MSG + 0x53
#define MSG_AT_WAIT			P_MSG + 0x57
#define MSG_AXIS			P_MSG + 0x59
#define MSG_AXIS_GYRO		P_MSG + 0x60
#define MSG_AXIS_ACC		P_MSG + 0x61
// ---------- Messages ---------- End
