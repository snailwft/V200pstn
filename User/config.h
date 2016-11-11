/************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : config.h
* Version        : V1.0
* Programmer(s)  :
* Parameters     : ARM LPC1114  12MHz
* DebugTools     : JLINK V8.0 And Realview MDK V4.53
* Description    : 
*
*
**************************************************************************************/

#ifndef __CONFIG_H 
#define __CONFIG_H

/**************************************************************************************
*                                MacroDeFinition                                                
**************************************************************************************/
#ifndef TRUE
#define TRUE            (1)					   //
#endif

#ifndef FALSE
#define FALSE           (!TRUE)				   //
#endif

/**************************************************************************************
*                             DataType                               
**************************************************************************************/
typedef unsigned  char   uint8;                // defined for unsigned 8-bits integer variable 	   
typedef signed    char   int8;                 // defined for signed 8-bits integer variable		 
typedef unsigned  short  uint16;               // defined for unsigned 16-bits integer variable 	
typedef signed    short  int16;                // defined for signed 16-bits integer variable 	      
typedef unsigned  int    uint32;               // defined for unsigned 32-bits integer variable 	  
typedef signed    int    int32;                // defined for signed 32-bits integer variable 	    
typedef float            fp32;                 // single precision floating point variable (32bits) 
typedef double           fp64;                 // double precision floating point variable (64bits)

/*************************************************************************************/
//#define	SETBIT(x,y)     (x |= (1 << y))        // set bit y in byte x
//#define	CLRBIT(x,y)     (x &= (~(1 << y)))     // clear bit y in byte x
//#define	CHKBIT(x,y)     (x & (1 << y))         // check bit y in byte x

//为了方便使用，采用了位的宏定义操作
#define CPL_BIT(x,y,z) (x->y^=(1<<z)) 
#define SET_BIT(x,y,z) (x->y|=(1<<z))
#define CLR_BIT(x,y,z) (x->y&=~(1<<z))
#define GET_BIT(x,y,z) (x->y&(1<<z))
#define GET_BITS(x,y) (x->y&0xf00)

/**************************************************************************************
*                             SystemHeaderFiles                    
**************************************************************************************/
#include    <string.h>                         // strlen strcat

/**************************************************************************************
*                             UserHeaderFiles            
**************************************************************************************/
#include    "../system/LPC11xx.h"			   // IC头文件
#include    "../system/system_lpc11xx.h"       // system

/*************************************************************************************/
#endif
									 
/**************************************************************************************
*                              End Of File         
**************************************************************************************/
