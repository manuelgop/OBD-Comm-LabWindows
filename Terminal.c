//==============================================================================
//
// Title:       Terminal
// Purpose:     Using the DLPDesign IO20 to read temperatures from Port 2
//
// Created on:  13/04/2010 at 04:13:08 p.m. by Rick Swenson.
// Copyright:   ITESM. All Rights Reserved.
//
// Instructions:
//    1) Obtain a LM35 temperature sensor
//    2) Connect the LM35 voltage input  to the +5V terminal of the IO-20
//    3) Connect the LM35 ground  input  to the Gnd terminal of the IO-20
//    4) Connect the LM35 central output to the 2   terminal of the IO-20
//==============================================================================

//==============================================================================
// Include files

#include <rs232.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <formatio.h>
#include "Terminal.h"
#include "toolbox.h"

//==============================================================================
// Constants
#define  comport          36  		//adjust com port!!!
#define  baudrate         38400
#define  databits         8
#define  stopbits         1
#define  inputbuffersize  16       // Number of characters that will be received
#define  outputbuffersize 6       // Number of characters that will be transmitted


//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int start = 0;
int stop = 0;
int running = 0;
int cerrar = 0;

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    OpenComConfig(comport, "", baudrate, 0, databits, stopbits, inputbuffersize, outputbuffersize);
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "Terminal.uir", PANEL));
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK START_READING (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			cerrar = 1;
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}

int CVICALLBACK COMMAND_START (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{	
	stop = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			while((stop==0)&&(cerrar==0)){
				//Declare a local variable
				int A,B,C;
				float load, thr, cool, flow, sens, error;
				char receive[inputbuffersize+1];
				char send[outputbuffersize+1] = {'0', '1', '0', 'C', '1', 0x0D};
				const char data[3];
				int read;

				
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				Fmt(data,"%c%c",receive[14],receive[15]);
				B = (int) strtol(data,NULL,16);
				C = ((A*256)+B)/4;
				SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(C>500){
					SetCtrlVal(panelHandle,PANEL_NUMERICGAUGE_RPM,C);
				}
				
				
				send[3] = 'D';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=13){
					SetCtrlVal(panelHandle,PANEL_NUMERICGAUGE_SPEED,A);
				}
				//Load function
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '4';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				load = (float)(A*100)/255;
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_LOAD,load);
				}
				
				///Throttle
				
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '1';
				send[2] = '1';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				thr = (float)(A*100)/255;
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_THROTTLE,thr);
				}
				
				
				///Coolant sensor
				
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '5';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				cool = (float)(A-40);
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_NUMERICMETER_ENGINEC,cool);
				}
				
				
				////In take air 
					
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '5';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				cool = (float)(A-40);
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_NUMERICTHERM_INTAKE,cool);
				}
				
				
				
				////Mass air flow rate 
					
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '0';
				send[2] = '1';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				Fmt(data,"%c%c",receive[14],receive[15]);
				B = (int) strtol(data,NULL,16);
				flow = (float)((A*256)+B)/100;
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_NUMERICDIAL_FLOW,flow);
				}
				
					////02 SENSOR
					
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[3] = '5';
				send[2] = '1';
				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				Fmt(data,"%c%c",receive[14],receive[15]);
				B = (int) strtol(data,NULL,16);
				sens = (float)(A/200);
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				if(A!=0){
					SetCtrlVal(panelHandle,PANEL_COLORNUM_MINIA,sens);
				}
				
				
					
					////ERROR
					
				Delay(.10);
				FlushOutQ(comport);
				FlushInQ(comport);
				send[2] = '2';
				send[3] = '1';

				FlushOutQ(comport);
				FlushInQ(comport);
				send[outputbuffersize] = '\0';
				ComWrt(comport,send,outputbuffersize);
				//SetCtrlVal(panelHandle,PANEL_SEND,send);
				//Read response from vehicle
				memset(receive,'\0',inputbuffersize);
				read = ComRd(comport,receive,inputbuffersize-1);
				//Add null char to finish String
				receive[inputbuffersize]= '\0';
				//obtain value from received string
				Fmt(data,"%c%c",receive[12],receive[13]);
				A = (int) strtol(data,NULL,16);
				Fmt(data,"%c%c",receive[14],receive[15]);
				B = (int) strtol(data,NULL,16);
				error = (float)(A*256)+B+55;
				//SetCtrlVal(panelHandle,PANEL_NUMERIC_TEXT,receive);
				 if(A==0){
					SetCtrlVal(panelHandle,PANEL_NUMERICTANK_ERROR,error);
				 }
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
			}
			if(cerrar==1)
			{
				QuitUserInterface (0);
			}
			break;
		case EVENT_RIGHT_CLICK:
			break;
	}
	return 0;
}

int CVICALLBACK STOP_READING (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			stop=1;
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
