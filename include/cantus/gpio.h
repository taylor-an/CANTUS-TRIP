#pragma once

#define R_P0DIR	((volatile unsigned int*)0x80023400)
#define R_P1DIR	((volatile unsigned int*)0x80023440)
#define R_P2DIR	((volatile unsigned int*)0x80023480)
#define R_P3DIR	((volatile unsigned int*)0x80023500)
#define R_P4DIR	((volatile unsigned int*)0x80023540)
#define R_P5DIR	((volatile unsigned int*)0x80023580)
#define R_P6DIR	((volatile unsigned int*)0x80023600)

#define	R_P0oDIR R_P0DIR
#define	R_P1oDIR R_P1DIR
#define	R_P2oDIR R_P2DIR
#define	R_P3oDIR R_P3DIR
#define	R_P4oDIR R_P4DIR
#define	R_P5oDIR R_P5DIR
#define	R_P6oDIR R_P6DIR

// Port Direction Input Mode Setting Registers
#define R_P0iDIR	((volatile unsigned int*)0x80023404)
#define R_P1iDIR	((volatile unsigned int*)0x80023444)
#define R_P2iDIR	((volatile unsigned int*)0x80023484)
#define R_P3iDIR	((volatile unsigned int*)0x80023504)
#define R_P4iDIR	((volatile unsigned int*)0x80023544)
#define R_P5iDIR	((volatile unsigned int*)0x80023584)
#define R_P6iDIR	((volatile unsigned int*)0x80023604)

//Port Output Data Level Registers
#define R_P0oLEV	((volatile unsigned int*)0x80023408)
#define R_P1oLEV	((volatile unsigned int*)0x80023448)
#define R_P2oLEV	((volatile unsigned int*)0x80023488)
#define R_P3oLEV	((volatile unsigned int*)0x80023508)
#define R_P4oLEV	((volatile unsigned int*)0x80023548)
#define R_P5oLEV	((volatile unsigned int*)0x80023588)
#define R_P6oLEV	((volatile unsigned int*)0x80023608)

//Port Output Data High Level Setting Registers
#define R_P0oHIGH R_P0oLEV	
#define R_P1oHIGH R_P1oLEV	
#define R_P2oHIGH R_P2oLEV	
#define R_P3oHIGH R_P3oLEV 
#define R_P4oHIGH R_P4oLEV	
#define R_P5oHIGH R_P5oLEV 
#define R_P6oHIGH R_P6oLEV	

//Port Output Data Low Level Setting Registers
#define R_P0oLOW ((volatile unsigned int*)0x8002340c)
#define R_P1oLOW ((volatile unsigned int*)0x8002344c)
#define R_P2oLOW ((volatile unsigned int*)0x8002348c)
#define R_P3oLOW ((volatile unsigned int*)0x8002350c)
#define R_P4oLOW ((volatile unsigned int*)0x8002354c)
#define R_P5oLOW ((volatile unsigned int*)0x8002358c)
#define R_P6oLOW ((volatile unsigned int*)0x8002360c)

//Port Input Data Level Registers
#define R_P0iLEV	((volatile unsigned int*)0x80023410)
#define R_P1iLEV	((volatile unsigned int*)0x80023450)
#define R_P2iLEV	((volatile unsigned int*)0x80023490)
#define R_P3iLEV	((volatile unsigned int*)0x80023510)
#define R_P4iLEV	((volatile unsigned int*)0x80023550)
#define R_P5iLEV	((volatile unsigned int*)0x80023590)
#define R_P6iLEV	((volatile unsigned int*)0x80023610)

//Port Pull-up Status Registers
#define R_P0PUS		((volatile unsigned int*)0x80023418)
#define R_P1PUS		((volatile unsigned int*)0x80023458)
#define R_P2PUS		((volatile unsigned int*)0x80023498)
#define R_P3PUS		((volatile unsigned int*)0x80023518)
#define R_P4PUS		((volatile unsigned int*)0x80023558)
#define R_P5PUS		((volatile unsigned int*)0x80023598)
#define R_P6PUS		((volatile unsigned int*)0x80023618)

//Port Pull-up Enable Registers
#define R_P0PUEN		((volatile unsigned int*)0x80023418)
#define R_P1PUEN		((volatile unsigned int*)0x80023458)
#define R_P2PUEN		((volatile unsigned int*)0x80023498)
#define R_P3PUEN		((volatile unsigned int*)0x80023518)
#define R_P4PUEN		((volatile unsigned int*)0x80023558)
#define R_P5PUEN		((volatile unsigned int*)0x80023598)
#define R_P6PUEN		((volatile unsigned int*)0x80023618)

//Port Pull-up Disable Registers
#define R_P0PUDIS		((volatile unsigned int*)0x8002341C)
#define R_P1PUDIS		((volatile unsigned int*)0x8002345C)
#define R_P2PUDIS		((volatile unsigned int*)0x8002349C)
#define R_P3PUDIS		((volatile unsigned int*)0x8002351C)
#define R_P4PUDIS		((volatile unsigned int*)0x8002355C)
#define R_P5PUDIS		((volatile unsigned int*)0x8002359C)
#define R_P6PUDIS		((volatile unsigned int*)0x8002361C)

//Port Rising-Edge Detect Registers
#define R_P0RED		((volatile unsigned int*)0x80023420)
#define R_P1RED		((volatile unsigned int*)0x80023460)
#define R_P2RED		((volatile unsigned int*)0x800234A0)
#define R_P3RED		((volatile unsigned int*)0x80023520)
#define R_P4RED		((volatile unsigned int*)0x80023560)
#define R_P5RED		((volatile unsigned int*)0x800235A0)
#define R_P6RED		((volatile unsigned int*)0x80023620)

//Port Falling-Edge Detect Registers
#define R_P0FED		((volatile unsigned int*)0x80023424)
#define R_P1FED		((volatile unsigned int*)0x80023464)
#define R_P2FED		((volatile unsigned int*)0x800234A4)
#define R_P3FED		((volatile unsigned int*)0x80023524)
#define R_P4FED		((volatile unsigned int*)0x80023564)
#define R_P5FED		((volatile unsigned int*)0x800235A4)
#define R_P6FED		((volatile unsigned int*)0x80023624)

//Port Edge Status Registers
#define R_P0EDS		((volatile unsigned int*)0x80023428)
#define R_P1EDS		((volatile unsigned int*)0x80023468)
#define R_P2EDS		((volatile unsigned int*)0x800234A8)
#define R_P3EDS		((volatile unsigned int*)0x80023528)
#define R_P4EDS		((volatile unsigned int*)0x80023568)
#define R_P5EDS		((volatile unsigned int*)0x800235A8)
#define R_P6EDS		((volatile unsigned int*)0x80023628)



