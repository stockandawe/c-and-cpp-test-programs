//-----------------------------------------------------------------------------
// FILE NAME:    vntrUtil.c
//
// DESCRIPTION:  This file contains utility routines for the transrating
//				library module      
//               
// DOCUMENT REF: VNC Software coding guidelines
//
// NOTES:        -
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files:
//-----------------------------------------------------------------------------
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <csl_timerhal.h>
#include <stdarg.h>
#include <csl_timer.h>
#include <assert.h>
#include <csl_cache.h>
#include <csl_gpiohal.h>

//-----------------------------------------------------------------------------
// Project include files:
//-----------------------------------------------------------------------------
//
#include "vntrShellDefs.h"
#include "vntrUtil.h"
#include "vntrCmdMsg.h"
#include "vntrMuxEngine.h"
#include "vntrIoBufs.h"
#include "vntrDemux.h"

//-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
//
#define USE_DATA_SECTION  0   // FOR DMA TEST

//#define VERBOSE_PROFILING
#define TIMER_BUFFER_SIZE 10000
#define DBG_TIME_LEN       (28)  // Max length of time string
#define DBG_MAX_LINE_LEN   (256)  // Max output line length
#define DBG_FORMAT_LEN     (128)  // Max length of the format string

#define MEASURE_PERIOD        (5) // Num secs to check/report min idle usage
#define MIN_CPU_IDLE_PERCENT (10) // Threshold to report error to host



typedef struct
{
	UInt32* pBuf;            // buffer start address
	UInt32  size;            // size in bytes of buffer
	UInt32  writePosition;   // offset in bytes from start
	UInt16  wrap;            // 1 if buffer has wrapped
	UInt16  reserved;
}
DebugCtrl_t, *pDebugCtrl_t;

// profiling
typedef struct
{
    UInt32  start;
    UInt32  total;

} ContextMeasure_t;

typedef struct
{
    ContextMeasure_t  context[MAX_CONTEXTS];
    UInt32            prev;
    UInt32            curr;

} ContextCtrl_t;

char* gTaskStrs[MAX_CONTEXTS] = 
{
	"IDLE ", 
	"SHELL", 
	"CORE ", 
	"MUX  "
};

//-----------------------------------------------------------------------------
// Global data:
//-----------------------------------------------------------------------------
//

extern UInt32 gInputLost;
extern void vectors();
extern UInt32 gCpuMeasure;
extern TIMER_Handle gTimer2Handle ;

UInt32 gElapsedTime = 0;
UInt32 vcc=0, acc=0;
//UInt32 total_time = 0;

TIMER_Handle gTimer2Handle = Null;
volatile UInt32 gTimerSecs = 0;
UInt32 gTimerCnt = 0;
UInt32 gIdleCnt = 0xffffffff;
unsigned int 	ptimer0_cnt = 0;

extern int gRxCnt;
extern int gTxCnt;
extern int gRxRptCnt;
extern int gRxMisCnt;
extern int gDspId;
//static int gDataSeed0 = 0x100 ;

static char     gTimeBuf[DBG_TIME_LEN+1];
static char     gFormatBuf[DBG_MAX_LINE_LEN];
static char     gLineBuf[DBG_MAX_LINE_LEN+1];

static char logStuffing[7][9] = {
{0x0A,0x00},
{0x20,0x0A,0x00},
{0x20,0x20,0x0A,0x00},
{0x20,0x20,0x20,0x0A,0x00},
{0x20,0x20,0x20,0x20,0x0A,0x00},
{0x20,0x20,0x20,0x20,0x20,0x0A,0x00},
{0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x00}
};

static int gTimer2Init = 0;

static UInt32 gUtilDbgMessageLevel = DBG_INFO;

static int gUtilInit = False;
static pDebugCtrl_t  gpDebugCtrl;
#pragma DATA_SECTION (dbg_ctl, ".shell_dbg_ctl");
static unsigned char dbg_ctl[sizeof(DebugCtrl_t)];
static pDebugCtrl_t  gpDebugCtrl ; 	

#pragma DATA_SECTION (dbgBuffer, ".shell_dbg_buffer");
static unsigned char dbgBuffer[DEBUG_BUFFER_SIZE];

#if USE_DATA_SECTION
	#pragma DATA_SECTION (dmaBuf, ".dma_buffer_ptr");
	static unsigned char dmaBuf[0x100000];
#endif
static ContextCtrl_t gContextCtrl;

static UInt32 gMeasurePeriod = MEASURE_PERIOD;
static UInt32 gMinCpuThreshold = MIN_CPU_IDLE_PERCENT;

static float  gMinIdleUsage = 0xffffffff;

//-----------------------------------------------------------------------------
// Internal Prototypes:
//-----------------------------------------------------------------------------
//

static int UtilPrn(char* line);

static int TimeStampFormat(char*  pTimeBuf, size_t bufsize);
//-----------------------------------------------------------------------------
// Exported functions:
//-----------------------------------------------------------------------------
//
void reset_timer0();
unsigned int print_timer0();
extern void vntrCleanPmcDmaInputStack();
void DataReceive(int channelId,int* pPattern);
void DataSend(int channelId,int senderId,int destId,int* pPattern);

//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilInit
//
// DESCRIPTION:  
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
void vntrUtilInit(int initdebug)
{

	gUtilInit = True;
	
	
	if (initdebug)
	{
		gpDebugCtrl = (pDebugCtrl_t)dbg_ctl;
		gpDebugCtrl->pBuf = (UInt32*)dbgBuffer;			
		memset(dbgBuffer,0,DEBUG_BUFFER_SIZE);

		gpDebugCtrl->writePosition = 0;
		gpDebugCtrl->size = DEBUG_BUFFER_SIZE;
		gpDebugCtrl->wrap = 0;
		gpDebugCtrl->reserved = 0;
	}
	
	// Reset profiling
	vntrUtilContextReset();

}

//-----------------------------------------------------------------------------
// FUNCTION:    vntrTimer1Isr
//
// DESCRIPTION: Interrupt service routine for timer1 of the DM642 
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
UInt32 timer1_heartbeat=0;

interrupt void vntrTimer1Isr()
{
    Uint32  gie;

    gie = IRQ_globalDisable();


#if 0 //TRACE_DETAIL
	if (timer1_heartbeat++%36000 == 0) // every 10mns
	    TRACE1(DBG_INFO,"t1_heartbeat %08x\n", timer1_heartbeat);
#endif	


	// Call main MUX function
#if MUX
	CONTEXT_SWITCH(CONTEXT_MUX);	
	vntrMuxFieldInterrupt();	
	CONTEXT_SWITCH(CONTEXT_PREV);
#endif
	
	IRQ_globalRestore(gie);
}
//-----------------------------------------------------------------------------
// FUNCTION:    vntrTimer2Isr
//
// DESCRIPTION: Interrupt service routine for timer2 of the DM642 
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
UInt32 timer2_heartbeat=0;


interrupt void vntrTimer2Isr()
{
    Uint32  gie;
    
    gie = IRQ_globalDisable();

#if 0 //TRACE_DETAIL
	if (timer2_heartbeat++%600 == 0) // every 10mns
	    TRACE1(DBG_INFO,"t2_heartbeat %08x\n", timer2_heartbeat);
#endif

	gTimerSecs++;	
	gElapsedTime++;

	if (gCpuMeasure)
	{
		vntrUtilContextCheck();
    	vntrUtilContextReset();    
	}

	TIMER_setCount(gTimer2Handle,0);
   	IRQ_globalRestore(gie);
    
}

//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilInitTimer0
//
// DESCRIPTION: This function initialises timer 0 on the DM642, this timer is  
//              intended to be used for profiling purpose
// RETURN:      
//                  
// NOTES:       The counter register increments every cycle*8, cycle = 1/DSP
//				clock value (600MHz, 720MHz, ..). It will return to 0 when it
//				reaches the value set in the periode register
//				At 600MHz, the counter increments every 8/600000 = 13.3333ns
//				Will return to 0 every 0xffffffff*13.33ns # 57sec.
//-----------------------------------------------------------------------------
void vntrUtilInitTimer0()
{
char* p;

	p = (char*)_TIMER_CTL0_ADDR;
		*(int*)p = 0x000002C0;

	p = (char*)_TIMER_PRD0_ADDR; // Timer period register
	*(int*)p = 0xffffffff;

	p = (char*)_TIMER_CNT0_ADDR; // Timer counter register
	*(int*)p = 0x00000000; // reset to 0
	
	ptimer0_cnt = *(int*)p;

}


//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilInitTimer1
//
// DESCRIPTION: This function initialises timer 1 on the DM642, this timer is  
//              intended to generate a field interrupt
// RETURN:      
//                  
// NOTES:       The counter register increments every cycle*8, cycle = 1/DSP
//				clock value (600MHz, 720MHz, ..).
//-----------------------------------------------------------------------------
#if 1
void vntrUtilInitTimer1()
{
	char* p;

	// *********** Init Timer1 interrupt **********
	IRQ_nmiDisable();
	IRQ_globalDisable();
	
	//Point to the IRQ vector table  
	IRQ_setVecs(vectors);
	
	IRQ_map(IRQ_EVT_TINT1,15);
	
    //Reset the TIMER1 INT event
    IRQ_reset(IRQ_EVT_TINT1);    
	IRQ_enable(IRQ_EVT_TINT1);
	
	p = (char*)_TIMER_CTL1_ADDR;
		*(int*)p = _TIMER_CTL_GO_MASK|_TIMER_CTL_HLD_MASK|_TIMER_CTL_CLKSRC_MASK; //0x000002C0;

	p = (char*)_TIMER_PRD1_ADDR; // Timer period register
#if TEST_PAL
	*(int*)p = (isPAL) ? TIMER1_RELOAD_PAL : TIMER1_RELOAD;
	TRACE2(DBG_INFO,"Timer 1 reload is %d. Cpu freq id %d\n",*(int*)p,cpuFreq);	
#else	
	*(int*)p = TIMER1_RELOAD;

	TRACE2(DBG_INFO,"Timer 1 reload is %d. Cpu freq id %d\n",TIMER1_RELOAD,cpuFreq);
#endif

	p = (char*)_TIMER_CNT1_ADDR; // Timer counter register
	*(int*)p = 0x00000000; // reset to 0

	// Enable interrupts when everything is intialized
    IRQ_globalEnable(); 
    IRQ_nmiEnable();

}
#endif

//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilInitTimer2
//
// DESCRIPTION: This function initialises timer 0 on the DM642, this timer is  
//              intended to be used for profiling purpose
// RETURN:      
//                  
// NOTES:   
//-----------------------------------------------------------------------------
#if 1
void vntrUtilInitTimer2()
{
	TIMER_Config timerConfig;

	timerConfig.ctl = 0x2c0;
	timerConfig.prd = TIMER2_RELOAD;
									
	timerConfig.cnt = 0; 

	if (gTimer2Init)
	{
		TRACE0(DBG_WARN,"Timer2 already init!\n");	
		return;
	}
	
	gTimer2Handle = TIMER_open(TIMER_DEV2,TIMER_OPEN_RESET);
	if (gTimer2Handle == INV)
	{
		TRACE0(DBG_ERR,"Timer2 open failed!\n");
		return;
	}


	TIMER_config(gTimer2Handle, &timerConfig);

	IRQ_nmiDisable();
	IRQ_globalDisable();

    IRQ_setVecs(vectors);

    //Map DSPINT event to physical interrupt number
    IRQ_map(IRQ_EVT_TINT2, 10);

    IRQ_reset(IRQ_EVT_TINT2);    
    IRQ_enable(IRQ_EVT_TINT2);

	// Enable interrupts when everything is intialized
    IRQ_globalEnable(); 
    IRQ_nmiEnable();

	TRACE1(DBG_INFO,"timer 2 init! reload = %d\n",TIMER2_RELOAD);
	gTimer2Init = 1;
}
#endif

/*
//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilContextSwitch
//
// DESCRIPTION: Call when entering into new Code area
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
*/
void vntrUtilContextSwitch(int nextContext)
{
	UInt32  end;
	UInt32  start;
	UInt32  idx;

	// Calculate timer ticks on exit of current Context
	idx = gContextCtrl.curr;
	start = gContextCtrl.context[idx].start;
	end = TIMER_getCount(gTimer2Handle);
	if (end >= start)
		gContextCtrl.context[idx].total += ((end - start));
	else
		// Handle wrap
		gContextCtrl.context[idx].total += (TIMER2_RELOAD - start + end);


	if (gContextCtrl.context[idx].total > (cpuFreq>>3))
	{
		// Not sure why but ig is 1 timer
		TRACE4(DBG_VERB,"Context total overflow [%d] - %d %d %d\n", 
			idx,gContextCtrl.context[idx].total,start,end);
	
		gContextCtrl.context[idx].total-=(cpuFreq>>3);
	}

	// Determine next context	
	if (nextContext == CONTEXT_PREV)
	{
		gContextCtrl.curr = gContextCtrl.prev;
	}
	else
	{
		gContextCtrl.prev = gContextCtrl.curr;
		gContextCtrl.curr = (UInt32)nextContext;	
	}

	// Store current timer ticks
	idx = gContextCtrl.curr;
	gContextCtrl.context[idx].start = TIMER_getCount(gTimer2Handle);

}

/*
//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilContextReset
//
// DESCRIPTION: Call to reset measurements
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
*/

void vntrUtilContextReset(void)
{
	
	memset((void*)&gContextCtrl, 0, sizeof(gContextCtrl));

	gContextCtrl.context[CONTEXT_IDLE].start = TIMER_getCount(gTimer2Handle);
}

/*
//-----------------------------------------------------------------------------
// FUNCTION:    vntrUtilContextCheck
//
// DESCRIPTION: Call to print results
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------
*/

void vntrUtilContextCheck(void)
{
    UInt32  idx;
    UInt32  total=0;
    float  cpuUsage;
    
    
    if (gTimerSecs < 15)
    	return;
    
	vntrUtilContextSwitch(CONTEXT_IDLE);

#ifdef VERBOSE_PROFILING	
	TRACE0(DBG_INFO, "Task Measurements: \n");
#endif	

	for (idx = 0; idx < MAX_CONTEXTS; idx++)
	{
		gContextCtrl.context[idx].total <<= 3;
		cpuUsage = gContextCtrl.context[idx].total/(float)(cpuFreq/100);
		
		// Watch idle/available cpu
		if (idx == CONTEXT_IDLE && cpuUsage < gMinIdleUsage)
			gMinIdleUsage = cpuUsage;
			
#ifdef VERBOSE_PROFILING			
		TRACE3(DBG_INFO,"%s : %9d (%4f %%)\n", 
			gTaskStrs[idx], 
			gContextCtrl.context[idx].total,
			cpuUsage);
#endif

#ifdef EXCEL_PROFILING			
		vntmUtilPrn(DBG_MOD_ID,DBG_INFO,"%16d",		
			gContextCtrl.context[idx].total);
#endif

		total += gContextCtrl.context[idx].total;

	}	

#ifdef EXCEL_PROFILING			
		vntmUtilPrn(DBG_MOD_ID,DBG_INFO,"\n");
#endif

#ifdef VERBOSE_PROFILING	
//	TRACE0(DBG_INFO, "-------------------\n");
//	TRACE1(DBG_INFO,"Total: %d\n\n",total);
#endif

	if (gTimerSecs > gMeasurePeriod 
		&& gTimerSecs % gMeasurePeriod == 0)
	{
		if (gMinIdleUsage < gMinCpuThreshold)
		{
			TRACE1(DBG_INFO,"Reporting CPU overload, Min Idle = %f %%\n", gMinIdleUsage);
#if MSG
//			vntrMsgSend(MSG_ERROR, MSG_ERR_CPU, 0);
#endif
		}
		gMinIdleUsage = 0xffffffff;
	}

}





//-----------------------------------------------------------------------------
// FUNCTION:    set_timer
//
// DESCRIPTION: This function sets timer1 of the DM642 to the specified value
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------

void set_timer1(int value)
{
char* p;

	p = (char*)_TIMER_CNT1_ADDR; 
	*(int*)p = value;  
}

//-----------------------------------------------------------------------------
// FUNCTION:    get_timer1
//
// DESCRIPTION: This function gets the counter value of timer1 of the DM642 
//
// RETURN:		None      
//                  
// NOTES:       
//-----------------------------------------------------------------------------

unsigned int get_timer1()
{
char* p;

	p = (char*)_TIMER_CNT1_ADDR; // Timer counter register
	return ( *(int*)p );
}




//-----------------------------------------------------------------------------
// FUNCTION:    DMA_In_Out_Test
//
// DESCRIPTION: This function passes input direct to output. 
//              
// RETURN:      void: 
//                  
// NOTES:       AKA passthru mode
//              
//----------------------------------------------------------------------------
int prdma=0;
extern UInt32 gDspNum;
UInt8 gdmabuff[50000];
#define DMA_TEST_BUFFER_SIZE 0x40000
extern int gTransfer_started;
#define INTER_DSP_DMA_TEST 1
extern unsigned char test_dma[];
volatile char txfr_done[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void DMA_In_Out_Test()
{
    UInt32 contigDataAvailable, contigSpaceAvailable, data, dataToProcess;
    UInt32 total_data_received=0, total_data_sent=0, data_to_send;
    UInt8 *pdmabuff;
    UInt8 *psource;
    volatile UInt8* dmaTestBuffer = (volatile UInt8*)test_dma;
    UInt8 dmaPattern = 0;
    int i;
    UInt32 dest;

    prdma = 0;

    pdmabuff = &gdmabuff[0];
    dataToProcess = 0;

TRACE2(DBG_INFO,"In DMA_In_Out_Test() ap state is %d buffer %08x\n",ApStateGet(), pdmabuff);


    for (i=0; i<DMA_TEST_BUFFER_SIZE; i++)
    {
		dmaTestBuffer[i]=0x55;
	}


    while(ApStateGet() == AP_STATE_RUNNING)
    {
#if 1
        data = 0;
        if ( (contigDataAvailable=GetInstreamContigDataAvail()) >= MAX_MSG)
        {
            // Put data in intermediate buffer
            if (contigDataAvailable > MAX_MSG)
                contigDataAvailable = MAX_MSG;
            dataToProcess += contigDataAvailable;

            assert(contigDataAvailable < 50000);

            if ( (prdma%100)==0  || prdma<10)
            {
                TRACE3(DBG_INFO,"memcpy %08x %08x %i\n",pdmabuff, *instream_rdpt
                , contigDataAvailable);
            }
            psource = (UInt8*)*instream_rdpt;
            memcpy(pdmabuff,psource,contigDataAvailable);
            if ( (prdma%100)==0  || prdma<10)
            {
                prdma++;
                TRACE2(DBG_INFO,"R %i %i\n",contigDataAvailable,prdma);
            }

            //testMpegPacketLoss((char*)buffer, contigDataAvailable);
            total_data_received += contigDataAvailable;
            // Update instream buffer read pointer
            InstreamUpdateRdPtr(contigDataAvailable);

            data = contigDataAvailable;

        }


        // ****************************
        // Now copy in outstream buffer
        // ****************************

        while(data && (ApStateGet()==AP_STATE_RUNNING) )
        {

            contigSpaceAvailable = GetOutstreamContigSpaceAvail();
            if(contigSpaceAvailable >= data)
            {
                data_to_send = data;
                data = 0;
            }
            else
            {
                data_to_send = contigSpaceAvailable;
                data -= contigSpaceAvailable;
            }

            total_data_sent += data_to_send;

            if ( !((*outstream_wrpt>=OUTSTREAM_BUFF_ADDR) && (*outstream_wrpt<OUTSTREAM_BUFF_END_ADDR)) )
            {
            	TRACE0(DBG_INFO,"outstream_wrpt out of range,stop\n")
                IRQ_globalDisable();
                for ( ; ; );
            }


            if ( (prdma%100)==0  || prdma<10)
            {
                TRACE3(DBG_INFO,"memcpy %08x %08x %i\n",*outstream_wrpt, pdmabuff
                , data_to_send);
            }

            memcpy((char*)*outstream_wrpt,pdmabuff,data_to_send);

            if ( (prdma%100)==0 || prdma<10)
            {
                prdma++;
                TRACE2(DBG_INFO,"S %i %i\n",data_to_send, prdma);
            }

            OutstreamUpdateWrPtr(data_to_send);
        } // While(data)

		{
    		unsigned uDsp = *((volatile unsigned char*)0xB0000000);
   		 	if ( gDspNum != uDsp )
    		{
            	TRACE0(DBG_INFO,"gDspNum != uDsp out of range,stop\n")
        		IRQ_globalDisable();
        		for ( ; ; );
    		}
		}

        // Process 20000 bytes of data
        if (dataToProcess >= 20000)
        {
            if (gDspNum%2==1)
            {
#if INTER_DSP_DMA_TEST
                // Dma test pattern to slave DSP

                dmaPattern++;

                TRACE1(DBG_INFO,"master dma : %02x\n",dmaPattern);

                if (gDspNum == 1) dest = 2;
                else if(gDspNum == 3) dest = 8;
                StartSdramTransfer(dmaTestBuffer,DMA_TEST_BUFFER_SIZE,dest);

                *(volatile char *)txfr_done = 1;
                CACHE_wbInvL2((void*)txfr_done,8,CACHE_WAIT);
                StartSdramTransfer((void*)txfr_done,8,dest);


                for ( ; ; ) // wait for DSP TO DSP int
                {
                    while ( !( *(volatile int*)_GPIO_GPVAL_ADDR & (1 << 4) ) ); // wait interrupt

                    if ( (*(volatile char*)0xB000000C) & 0x08 )
                    {
                        // read interrupt source
                        unsigned char DspMaskInterruptSource = *(volatile unsigned char *)0xB000000E;

                        // clear interrupt
                        *(volatile unsigned char *)0xB000000F = 2; // select dsp_to_dsp interrupt clear flags
                        *(volatile unsigned char *)0xB000000E = DspMaskInterruptSource;
                        *(volatile unsigned char *)0xB000000F = 0; // return to dsp_to_dsp interrupt set flags
                        break;
                    }
                }

                TRACE1(DBG_INFO,"Master got ack %02x\n",dmaPattern);
#endif
            }
            else
            {

#if INTER_DSP_DMA_TEST

                dmaPattern++;
                TRACE1(DBG_INFO,"Slave wait dma : %02x\n",dmaPattern);

                while( *(volatile char *)txfr_done != 1 )
                {
                      CACHE_invL2((void*)txfr_done, 8, CACHE_WAIT) ;
                }
                *(volatile char *)txfr_done = 0 ;
                CACHE_wbInvL2((void*)txfr_done,8,CACHE_WAIT);

                TRACE1(DBG_INFO,"Slave wait dma, done : %02x\n",dmaPattern);

                if (gDspNum == 2) dest = 1;
                else if(gDspNum == 4) dest = 4;

                StartSdramTransfer(dmaTestBuffer,DMA_TEST_BUFFER_SIZE,dest);

				if ( *(volatile char *)txfr_done != 0 )
				{
    				IRQ_globalDisable();
    				for ( ; ; );
				}
                *(volatile unsigned char *)0xB000000E = dest; // send interrupt to targeted DSPs

#endif
            }

            // Simulate frame processing, 5ms
//          reset_timer0();
//          while(getdelta_timer0() < 90000*5);

            dataToProcess = 0;
        }


#endif
    } // while(ApStateGet() == AP_STATE_RUNNING)

#if 1
    TRACE1(DBG_INFO,"DMA_In_Out_Test(), exit while loop, state %d\n",ApStateGet());

    //IRQ_globalDisable();
    //for ( ; ; );

#endif
}

void testMpegPacketLoss(char* buffer, int size)
{
    int nb = size;
    UInt32  vidpid;
    UInt8* pp = (UInt8*)buffer;

		while(nb > 0)
		{
		vidpid = ((UInt32)(*(pp+1)&0x1f)<<8) + *(pp+2);
		pp+=3;

		// Check video continuity
		if(vidpid==demux_pid && (*pp&0x10) ) // VIDEO_PID (input) 48 (output)
		  {
			if(vcc)
		    {
		    	//TRACE0(DBG_INFO,"*");
				if ( (char)(*pp&0xf) != (char)((vcc+1)&0xf) )
				{
					TRACE3(DBG_INFO,"*Video CC error E %02x R %02x (%08x)\n", (int)((vcc+1)&0xf),(int)(*pp&0xf),(int)pp );

				}
				vcc = *pp&0xf;	
		    }
		  else
		    {
		    vcc = *pp&0xF;	
		    } 	
		  }

		// Check audio continuity
//		if(*(pp-1)==shell_base.audioStream[0].pid)
		if(vidpid==shell_base.audioStream[0].pid)
		  {
		  if(acc)
		    {
				if ( (char)(*pp&0xf) != (char)((acc+1)&0xf) )
				{
					TRACE3(DBG_INFO,"Audio CC error E %02x R %02x (%08x)\n",(int)((acc+1)&0xf),(int)(*pp&0xf),(int)pp);
				}
		    acc = *pp&0xf;	
		    }
		  else
		    {
		    acc = *pp&0xF;	
		    } 	
		  }
		pp +=185;
		nb -= 188;

		}
}

//-------------------------------------------------------------
// FUNCTION:    DebugBufferUpdateWrPtr:
//
// DESCRIPTION: This function prints the debug message. Simple version 
//              for now.
//              
// RETURN:      void: 
//                  
// NOTES:       Called from dp(). TRACE or assert would cause recursion
//              
//-----------------------------------------------------------------------------

void DebugBufferUpdateWrPtr(UInt32 PutData)
{

	gpDebugCtrl->writePosition += PutData;

	if(gpDebugCtrl->writePosition == gpDebugCtrl->size) 
	{
		gpDebugCtrl->wrap = 1;
		gpDebugCtrl->writePosition = 0;
	}
	
#if CACHE_CE1
		CACHE_flush(CACHE_L2,(void*)(gpDebugCtrl),sizeof(DebugCtrl_t)/4);
#endif


}



//-----------------------------------------------------------------------------
// FUNCTION:    vntmUtilPrn:
//
// DESCRIPTION: This function prints the debug message. Simple version 
//              for now.
//              
// RETURN:      void: 
//                  
// NOTES:       Assert calls this routine. Cannot assert here!
//              
//-----------------------------------------------------------------------------

void 
vntmUtilPrn(
    UInt32 modId,   // I: Module Id
    UInt32 level,   // I: Debug message level
    char*  pFormat, // I: String 'printf' style
    ...
    )
{

    UInt32   len;
	int 	 align;
	va_list  argList;
	UInt32  gie;

    gie = IRQ_globalDisable();

	//return;

    /* Check configured level */
    if (level > gUtilDbgMessageLevel)
    {
    	IRQ_globalRestore(gie);
        return;
    }
        
	memset(gTimeBuf,0,sizeof(gTimeBuf));

    TimeStampFormat(gTimeBuf,sizeof(gTimeBuf));
     
    snprintf(gFormatBuf, DBG_MAX_LINE_LEN, "%s %s",gTimeBuf,pFormat);
  
    // Generate the full string based on the parameters  
    va_start(argList, pFormat);
    len = vsnprintf(gLineBuf, DBG_MAX_LINE_LEN, gFormatBuf, argList);
    va_end(argList);

	/* Check length*/
    if (len >= DBG_MAX_LINE_LEN)
    {
        gLineBuf[DBG_MAX_LINE_LEN] = '\0';
     	if (UtilPrn("vntmUtilPrn: WARNING format string chopped\n"))
     	   {
    		printf("HELP! UtilPrn() returned error! Cannot write to debug buffer!\n");
			IRQ_globalRestore(gie);
    		return;
    		}
    }
    
	// Align data on 8 bytes for DMA
	align = len%8;
	
	if (align)
	{
   	 	if (gLineBuf[len-1]=='\n') 
    		gLineBuf[len-1] = 0x20;
	  	align = 7-align;
	  	strcat(gLineBuf, (char*)&logStuffing[align]);
	}
	
    

#if STREAMING
    // Output to debug buffer
    if (UtilPrn(gLineBuf))
    {
    	TRACE0(DBG_ERR,"HELP! UtilPrn() returned error! Cannot write to debug buffer!\n");
		IRQ_globalRestore(gie);
    	return;
    }
#else
	// Output to console
	TRACE1(DBG_INFO,"%s", gLineBuf);
#endif


	IRQ_globalRestore(gie);
} // vntmUtilPrn()


//-----------------------------------------------------------------------------
// FUNCTION:    vntmUtilDump:
//
// DESCRIPTION: This function dumps a buffer to the debug buffer in alphanumerical
//				characters 
//              
//              
// RETURN:      void: 
//                  
// NOTES:       Assert calls this routine. Cannot assert here!
//              
//-----------------------------------------------------------------------------

void 
vntmUtilDump(
    UInt32 modId,   // I: Module Id
    UInt32 level,   // I: Debug message level
    UInt32 size, 	// I: amount of data to dump
    UInt8* buffer	// I: data buffer
    )
{

    UInt32   len= size;
	UInt8*   pData = buffer;
	char*	 pOut = gLineBuf;
	int to_send,i;
	UInt32  gie;

    gie = IRQ_globalDisable();

	//return;

    /* Check configured level */
    if (level > gUtilDbgMessageLevel)
    {
		IRQ_globalRestore(gie);
        return;
    }

	while (len)
    { 
    	pOut = gLineBuf;
    	to_send = len >= 32 ? 32 : len;
    	for (i=0;i<to_send;i++){sprintf(pOut, "%02x",pData[i]);pOut+=2;}
    	sprintf(pOut,"\n");
    	UtilPrn(gLineBuf);
    	len -= to_send;
    	pData += to_send;
  	}

	IRQ_globalRestore(gie);
} // vntmUtilPrn()

//-----------------------------------------------------------------------------
// FUNCTION:    vntmUtilByte:
//
// DESCRIPTION: This function dumps a byte to the debug buffer in alphanumerical
//				or numeric mode 
//              
//              
// RETURN:      void: 
//                  
// NOTES:       
//              
//-----------------------------------------------------------------------------

void 
vntmUtilByte(
    UInt32 modId,   // I: Module Id
    UInt32 level,   // I: Debug message level
    UInt8 byte,		// I: byte value
    int mode		// I: 0 alphanumeric, 1 numeric
    )
{
	UInt32  gie;

    gie = IRQ_globalDisable();
   /* Check configured level */
    if (level > gUtilDbgMessageLevel)
    {
		IRQ_globalRestore(gie);
        return;
    }

		if (mode)
    		sprintf(gLineBuf, "%02x",byte);
    	else
    	{
			 gLineBuf[0]=byte;
			 gLineBuf[1]=0;
		}    		
    	UtilPrn(gLineBuf);

	IRQ_globalRestore(gie);
} // vntmUtilPrn()

//-----------------------------------------------------------------------------
// FUNCTION:    vntmUtilWord:
//
// DESCRIPTION: This function dumps a word to the debug buffer in alphanumerical
//				or numeric mode 
//              
//              
// RETURN:      void: 
//                  
// NOTES:       
//              
//-----------------------------------------------------------------------------

void 
vntmUtilWord(
    UInt32 modId,   // I: Module Id
    UInt32 level,   // I: Debug message level
    UInt32 word		// I: byte value
    )
{
	UInt32  gie;

    gie = IRQ_globalDisable();
   /* Check configured level */
    if (level > gUtilDbgMessageLevel)
    {
		IRQ_globalRestore(gie);
        return;
	}
	
	sprintf(gLineBuf, "%08x",word);

   	UtilPrn(gLineBuf);


	IRQ_globalRestore(gie);
} // vntmUtilPrn()


//-----------------------------------------------------------------------------
// FUNCTION:    UtilPrn:
//
// DESCRIPTION: Print a line of data to the debug buffer.
//              
// RETURN:      1 if err 
//                  
// NOTES:       Called from vnntrUtilPrn(). TRACE or assert would cause recursion
//              
//-----------------------------------------------------------------------------

static int UtilPrn(char* line)
{
	UInt32 contigSpaceAvailable, data;
	UInt32 data_to_send;

	assert(gUtilInit);

	data = strlen(line);

	while(data)
	{
		contigSpaceAvailable =  DEBUG_BUFFER_SIZE - gpDebugCtrl->writePosition;
		// GetDebugBufferContigSpaceAvail();
		
		if(contigSpaceAvailable >= data)
		{
			data_to_send = data;
			data = 0;
		}
		else
		{
			data_to_send = contigSpaceAvailable;
			data -= contigSpaceAvailable;
		}

		assert(((UInt32)gpDebugCtrl->pBuf+gpDebugCtrl->writePosition+data_to_send) <=
								 DEBUG_BUFF_END_ADDR);
		assert(((UInt32)gpDebugCtrl->pBuf+gpDebugCtrl->writePosition+data_to_send) > 
								 DEBUG_BUFF_ADDR);
		
		memcpy((char*)(DEBUG_BUFF_ADDR+gpDebugCtrl->writePosition),line,data_to_send);
#if CACHE_CE1
		CACHE_flush(CACHE_L2,(void*)((DEBUG_BUFF_ADDR+gpDebugCtrl->writePosition)&0xfffffffc),(data_to_send>>2)+1);
#endif
		
		DebugBufferUpdateWrPtr(data_to_send);
		
	} // While(data)		
	
	return 0;

}




//-----------------------------------------------------------------------------
// FUNCTION:    TimeStampFormat:
//
// DESCRIPTION: This function gets the current time from pSOS, parses it into
//              the tm extension structure and finally formats a string.
//              
// RETURN:      length of formatted string
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------
static int
TimeStampFormat(
    char*  pTimeBuf,  // O: buffer for the formatted timestamp
    size_t bufsize    // I: size of the provided buffer
    )
{
    Int32           len;
    struct tm     timeExt;
   	UInt32   days,time,hours,min,sec,msec;

    /* elapsed time from start of app*/
    /* todo; get ntp time from host at init, add elapsed and convert */
	time = gElapsedTime;
	
	if (time != 0)
	{
		days  =  (float)time/86400;
		hours = ((float)(time%86400))/3600;
		min   =  ((float)(time%3600))/60;
		sec   =  time-((days*86400)+(hours*3600)+(min*60));		
    	msec  = ((float)TIMER_getCount(gTimer2Handle)/TIMER2_RELOAD)*1000;
    }
    else
    	days=hours=min=sec=msec=0;
    	
          
    timeExt.tm_sec  = sec;
    timeExt.tm_min  = min;
    timeExt.tm_hour = hours;
 	timeExt.tm_mday = 1;
    timeExt.tm_mon  = 0;
    timeExt.tm_year = 0;

    //strftime(pTimeBuf, bufsize, "%b-%d-%Y %H:%M:%S", &timeExt);
    // Don't clobber the days robert
    strftime(pTimeBuf+4, bufsize-4, "%H:%M:%S", &timeExt);
    len = snprintf(pTimeBuf, DBG_TIME_LEN,"%3d:%s:%3d;",days,pTimeBuf+4,msec);

    if (len < 0)
        return 0;
    else
        return (len);

}

void reset_timer0()
{
char* p;

	p = (char*)_TIMER_CNT0_ADDR; 
	*(int*)p = 0x00000000; // reset to 0
	ptimer0_cnt = *(int*)p;
}

unsigned int print_timer0()
{
char* p;
unsigned int cnt, delta;

	p = (char*)0x01940008; // Timer counter register
	cnt = *(int*)p;
	
	if (cnt >= ptimer0_cnt)
	{
		delta = cnt - ptimer0_cnt;
//		printf("Timer 0 delta time : %08x (%f msec)", delta , (float)delta*(float)8/(float)600000 );
//		printf("%08x %08x %i", *instream_wrpt, *instream_rdpt,GetInstreamContigDataAvail());
//		TRACE2(DBG_INFO,"Timer 0 delta time : %08x (%f msec)\n", delta , (float)delta*(float)8/(float)600000 );
		TRACE1(DBG_INFO,"frame : (%f msec)\n",(float)delta*(float)8/(float)720000 );
//		TRACE3(DBG_INFO,"%08x %08x %i", *instream_wrpt, *instream_rdpt,GetInstreamContigDataAvail());
//		TRACE0(DBG_INFO,"\n");
		return (delta);
	}
	else
	{
		TRACE0(DBG_INFO,"Timer wrapped around\n");
		return (0xffffffff);
	}
}


//-----------------------------------------------------------------------------
// FUNCTION:    getdelta_timer0:
//
// DESCRIPTION: 
//              
// RETURN:      length of formatted string
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------


unsigned int getdelta_timer0()
{
char* p;
unsigned int cnt, delta;

	p = (char*)0x01940008; // Timer counter register
	cnt = *(int*)p;
	
	if (cnt >= ptimer0_cnt)
	{
		delta = cnt - ptimer0_cnt;
		return (delta);
	}
	else
	{
		TRACE0(DBG_INFO,"Timer wrapped around\n");
		return (0xffffffff);
	}
}



//-----------------------------------------------------------------------------
// FUNCTION:    DMA_Isram_Sdram_Test:
//
// DESCRIPTION: This function tests intensively EDMA from internal memory
//			    to external memory. It uses QDMA on DM642 (spru234b.pdf) 
//              
// RETURN:      length of formatted string
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------

#if TEST_DMA
void setupInterrupts(void);
void stopEdma(void); 

#include <csl_edma.h>
#define TCCINTNUM   	10
#define BUFF_SZ      	352*480/4 //256
#define TRANSFER_CNT 	100
#define TESTS_CNT		5
int* ping=0;
int ping_data[BUFF_SZ];
volatile int transferCount=0;
volatile int testsCount=0;
int totaltestCount=0;
#pragma DATA_SECTION(ping_data, ".ittiam_core_dsect_isram");
#pragma DATA_ALIGN(ping,128);
#pragma DATA_ALIGN(ping_data,128);

EDMA_Config cfgEdmaPing =
{  
    //Making Options parameter register - EDMA_OPT
    EDMA_OPT_RMK
    (          
		EDMA_OPT_PRI_LOW,
   		EDMA_OPT_ESIZE_32BIT,
   		EDMA_OPT_2DS_NO,
   		EDMA_OPT_SUM_INC,
   		EDMA_OPT_2DD_NO,
   		EDMA_OPT_DUM_INC,
   		EDMA_OPT_TCINT_YES,
   		EDMA_OPT_TCC_OF(TCCINTNUM),
    	EDMA_OPT_TCCM_OF(TCCINTNUM >> 4),
    	EDMA_OPT_ATCINT_NO,
    	EDMA_OPT_ATCC_OF(0),
    	EDMA_OPT_PDTS_DISABLE,
    	EDMA_OPT_PDTD_DISABLE,
        EDMA_OPT_LINK_YES,
    	EDMA_OPT_FS_NO
    ),
    EDMA_SRC_OF(ping_data),//Source address register 
    EDMA_CNT_OF(BUFF_SZ),   //Transfer count parameter
    EDMA_IDX_OF(0x00000004),//Index parameter
    EDMA_RLD_OF(0x00000000) //Count reload/link parameter
};                         



 void DMA_Isram_Sdram_Test()
 {
 int i;
 
 	totaltestCount = 0;
	if (!ping)
	ping = malloc(4*BUFF_SZ*TRANSFER_CNT);
	if (!ping)
	{
		TRACE0(DBG_INFO,"Ping malloc failed, exit\n");
		exit(0);
	}

	TRACE2(DBG_INFO,"Source address : %08x, destination address : %08x\n",
	ping_data,ping);
  
  	memset(ping,0,BUFF_SZ*TRANSFER_CNT*4);
  
  	for (i=0;i<BUFF_SZ;i++)
	{
    	ping_data[i]=i;
  	}

    //Let's disable/clear related interrupts just in case they are pending
    //  from a previous run of the program.
    setupInterrupts();
    
    //Although not required, let's clear all of the EDMA parameter RAM.
    //This makes it easier to view the RAM and see the changes as we
    //  configure it.
    EDMA_clearPram(0x00000000);
   
  	cfgEdmaPing.dst = EDMA_DST_OF(ping);

  	//Enable the related interrupts
  	IRQ_enable(IRQ_EVT_EDMAINT);
  	EDMA_intDisable(TCCINTNUM);
  	EDMA_intClear(TCCINTNUM);  
  	EDMA_intEnable(TCCINTNUM);        
  
	EDMA_qdmaConfig(&cfgEdmaPing);
	
	//Waiting for interrupts
  	while(ApStateGet() == AP_STATE_RUNNING);
  	stopEdma();
 }

//-----------------------------------------------------------------------------
// FUNCTION:    setupInterrupts:
//
// DESCRIPTION: Function to sets up interrupts to service EDMA transfers
//              
// RETURN:      -
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------

void setupInterrupts(void)
{
	//Point to the IRQ vector table
    IRQ_setVecs(vectors);
    IRQ_nmiEnable();
    IRQ_globalEnable();
    IRQ_map(IRQ_EVT_EDMAINT, 8);
    IRQ_reset(IRQ_EVT_EDMAINT);
}

//-----------------------------------------------------------------------------
// FUNCTION:    edmaIsr:
//
// DESCRIPTION: Interrupt Service Routine c_int08 : ISR to service EDMAINT. 
//				vecs_edma1.asm must be modified to include c_int08 entry.
//              
// RETURN:      -
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------

Uint32 pattern[4] = {0x00,0x55,0xaa,0xff};
#define BUFFER_SIZE 100000
UInt8 buffer[BUFFER_SIZE];

interrupt void    
edmaIsr(void)    
{
int i;
int *p;
UInt32 contigDataAvailable, contigSpaceAvailable, data;
int total_data_received=0, total_data_sent=0, data_to_send;

  	//Clear the pending interrupt from the EDMA interrupt pending register
  	EDMA_intClear(TCCINTNUM);

	// Verify DMA result for current buffer
	p = (int *)cfgEdmaPing.dst;

	for (i=0; i< BUFF_SZ; i++)
	{
		if (p[i] != ping_data[i])
		{
			TRACE3(DBG_INFO,"DMA error at address %08x got : %i exp : %i\n",&p[i], p[i],ping_data[i]);
		}
	}

	transferCount++;

    if (transferCount >= TRANSFER_CNT)
    {
	    if (++testsCount>= TESTS_CNT)
	    {
			testsCount=1;    
   		}
    		
   		TRACE1(DBG_INFO,"Test : %i\n",totaltestCount++);
    		
	  	for (i=0;i<BUFF_SZ;i++)
		{
			ping_data[i]=pattern[testsCount-1];
		}

	    transferCount=0;
    }

  	// Transfer to next buffer
 	cfgEdmaPing.dst = (Uint32)ping + transferCount*BUFF_SZ*4;


	//Keep checking data/in out buffer for test  closer to
	// real implementation
 	if ( (contigDataAvailable=GetInstreamContigDataAvail()) != 0 /*MAX_MSG*/)
	{
		// Put data in intermediate buffer	
        if (contigDataAvailable > BUFFER_SIZE)
		    contigDataAvailable = BUFFER_SIZE;
		contigDataAvailable &= 0xFFFFFFF8;	// 8-byte align
			
		if (contigDataAvailable)
		{
											
			memcpy(buffer,(char*)*instream_rdpt,contigDataAvailable);

			if ((prdma%100)==0)
			{
				TRACE2(DBG_INFO,"R %i %i\n",contigDataAvailable,GetInstreamTotalDataAvail());
			} 

			//testMpegPacketLoss((char*)buffer, contigDataAvailable);
			total_data_received += contigDataAvailable;
			// Update instream buffer read pointer
			InstreamUpdateRdPtr(contigDataAvailable);
		}
	}
		
	// ****************************
	// Now copy in outstream buffer
	// ****************************
	data = contigDataAvailable;

	while(data)
	{
		contigSpaceAvailable = GetOutstreamContigSpaceAvail();
		if(contigSpaceAvailable >= data)
		{
			data_to_send = data;
			data = 0;
		}
		else
		{
			data_to_send = contigSpaceAvailable;
			data -= contigSpaceAvailable;
		}

		total_data_sent += data_to_send;
		memcpy((char*)*outstream_wrpt,buffer,data_to_send);

		if ((prdma++%100)==0)
		{
			TRACE2(DBG_INFO,"S %i %i\n",contigDataAvailable,prdma);
		} 

		OutstreamUpdateWrPtr(data_to_send);
	} // While(data)		
 
 
	// Start DMA
   	EDMA_qdmaConfig(&cfgEdmaPing);
    	
  	return;
}

//-----------------------------------------------------------------------------
// FUNCTION:    stopEdma:
//
// DESCRIPTION: Stops the EDMA service. 
//				
//              
// RETURN:      -
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------
void stopEdma(void)
{
    //Disable interrupts, close EDMA channel before exit of the program
    IRQ_disable(IRQ_EVT_EDMAINT);
    EDMA_RSET(CCER,0x00000000);
    EDMA_intDisable(TCCINTNUM);
    EDMA_intClear(TCCINTNUM);
 	EDMA_resetAll();
 	EDMA_RSET(CIPR,0xFFFFFFFF);
 	EDMA_RSET(ECR,0xFFFFFFFF);
}
#endif


#if 0
//-----------------------------------------------------------------------------
// FUNCTION:    PMC_DMA_Test:
//
// DESCRIPTION: This function tests inter DSP communication using Vitec's 
//              DMA cpabilities
//
// RETURN:      length of formatted string
//
// NOTES:       -
//              
//-----------------------------------------------------------------------------

#define DMA_TRANSFER_SIZE 4096 //720*480  // Must be 64 bits aligned.
#define ELEMENT_DATA_TYPE 32 //720*480  // Must be 64 bits aligned.
#define DMA_CHANNELS 2
int i4_handle[DMA_CHANNELS];
int* v_addr[DMA_CHANNELS];

int* dmabuffer,*dmabuffer1;

void PMC_DMA_Test()
{
	int i,j, testcnt=0;

#if !EMULATOR_LOAD	
	while(gCoreId == -1);
    TRACE1(DBG_INFO,"gCoreId : %i\n",gCoreId);
#else
	gCoreId = GetDspNumber();

    gCoreId = (gCoreId-1)%2 ;

	printf("Core Id = %d \n",gCoreId);
	
#endif    
    
	for (i=0;i<DMA_CHANNELS;i++)
	{
#if USE_DATA_SECTION	
		v_addr[i] = (void*)(dmaBuf + (DMA_TRANSFER_SIZE*4*i));
#else		
		v_addr[i] = (void*)memalign(8,DMA_TRANSFER_SIZE*4);		
#endif		
		memset(v_addr[i],0,DMA_TRANSFER_SIZE*4);			
	}    
	
	
	printf("DMA Channel Address 0x%x \n",v_addr[0]);
	
    vntrInterDSPCommInit_t(DMA_CHANNELS,i4_handle,v_addr);

//    TRACE2(DBG_INFO,"i4_handle[0]:%08x,i4_handle[1]:%08x\n",i4_handle[0], i4_handle[1]);

	for (j=0;j<DMA_CHANNELS;j++)
	vntrInterDSPCommConfigure_t(i4_handle[j],          
	  						    0,    // I : number of element to skip in source
								0,    // I : number of element to skip in destination
								ELEMENT_DATA_TYPE,    // I : element data type 32 bits ... 16 ... 8 
								0,    // I : number of element to skip in source/destination
								DMA_TRANSFER_SIZE,  // I : element count to transfer
								0);    // I : type of transfer 


//	TRACE1(DBG_INFO,"Start transfer test for DSP ID %i\n",gCoreId);
	
	if (gCoreId == DSP_MASTER_ID)
	{
		reset_timer0();
		
		while(getdelta_timer0() < (720000000>>3)*2)
		
		while(1)
		{
			TRACE0(DBG_INFO,"Start Master DSP transfer\n");
			DataSend(0,1,1,&gDataSeed0);
#if !EMULATOR_LOAD			
			TRACE4(DBG_INFO,"Master transfer Tx Count %i :: Rx Count %i,Overwrite %i, MisMatch %i\n", 
			                 gTxCnt,gRxCnt,gRxRptCnt,gRxMisCnt);
#else
			printf("Master transfer Tx Count %i :: Rx Count %i,Overwrite %i, MisMatch %i\n", 
			                 gTxCnt,gRxCnt,gRxRptCnt,gRxMisCnt);
#endif			                 

			reset_timer0();
		
			while(getdelta_timer0() < (720000000>>3)*5)			
			testcnt++;
		}	
		
	}
	else
	{
		reset_timer0();
		
		while(getdelta_timer0() < (720000000>>3)*2)	

		while(1)
		{		
			TRACE0(DBG_INFO,"Wait for transfer\n");	
			
			DataReceive(0,&gDataSeed0);
#if !EMULATOR_LOAD			
			TRACE4(DBG_INFO,"Slave transfer Tx Count %i:: Rx Count %i ,Overwrite %i, MisMatch %i\n", 
			                 gTxCnt,gRxCnt,gRxRptCnt,gRxMisCnt);
#else
			printf("Slave transfer Tx Count %i :: Rx Count %i ,Overwrite %i, MisMatch %i\n", 
			                 gTxCnt,gRxCnt,gRxRptCnt,gRxMisCnt);
#endif			                 
			testcnt++;
		}		

	}	
}

int Random(int data)
{
   if (data & 0x80000000)
   {
      data =  (data << 1) ^ 0xFBAB01F1  ;
   }
   else
   {
      data = data << 1 ;
   }
   
   return data;
}
// for 1 channel
void DataSend(int channelId,int senderId,int destId,int* pPattern)
{
	int i,j = channelId,dest = destId;
	
	//WaitX((72000000>>3)*5);	// Wait 0.5 sec
	
    for ( i = 0 ; i < DMA_TRANSFER_SIZE ; i++ )
    {

    	v_addr[j][i] = *pPattern;
    	*pPattern = Random(*pPattern);
    }		

	TRACE3(DBG_INFO,"DSP %i start transfer DMA %i from %08x\n",gDspId,channelId,v_addr[j]);

	// Start
	vntrInterDSPCommStart_t(i4_handle[j],
						    senderId,			// I : sender ID
						    &v_addr[j],	// I : data address to send
						    1,
						    &dest, 			// I : receiver ID, 0=master, 1=slave1,..
					    	&v_addr[j], 	// I : data address to receive
					    	1,
					    	1);				// 1 : blocking
		
}
// for 1 channel
void DataReceive(int channelId,int* pPattern)
{
	int i,j = channelId,ret;
	int dataRead ;
			
//	printf("Receiver wait for data \n");

	ret = vntrInterDSPCommWait_t(i4_handle[j]);
	
	if(ret == 0)
	{
		TRACE3(DBG_INFO,"Receiver returned %i for %08x %08x\n",ret,i4_handle[j],v_addr[j]);
	}
	else
	{
		TRACE3(DBG_INFO,"FAIL:Receiver returned %i for %08x %08x\n",ret,i4_handle[j],v_addr[j]);	
	}

 	for ( i = 0 ; i < DMA_TRANSFER_SIZE ; i++ )
 	{
    	dataRead = v_addr[j][i];

        if ( dataRead != *pPattern )
	    {
       		TRACE2(DBG_ERR," Error : expected = 0x%x , received = 0x%x \n", *pPattern, dataRead);
			exit(0);
    	}
    	*pPattern = Random (*pPattern);
 	}
 	
	
}

#endif
