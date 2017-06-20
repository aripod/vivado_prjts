#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "TP_TD4_I2S_Behavioral.h"
#include "TP_TD4_I2S_Structural.h"
#include <math.h>
#include <stdint.h>
#include "acdc2.h"

#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INT_LRCLK			61	//Como hay una sola interrupcion, es asignado el valor minimo (Ver configuracion PS en Vivado)
#define I2S_TX_Behavioral XPAR_TP_TD4_I2S_BEHAVIORAL_0_S00_AXI_BASEADDR
#define I2S_TX_Structural XPAR_TP_TD4_I2S_STRUCTURAL_0_S00_AXI_BASEADDR

static int IntcInitFunction(u16 DeviceId);
int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
void LRCLK_Intr_Handler(void *data);

int status;
static XScuGic INTCInst;
static float y = 0;
uint16_t t=0, val;
uint32_t sval;
uint32_t ind=0;
static float f = 1000.0f, fs=48000.0f;
static float amplitud = 0.01f;


int main()
{

	status = IntcInitFunction(INTC_DEVICE_ID);

	init_platform();

    while(1)
    {
    	//xil_printf("Hello World\n\r");


    }

    cleanup_platform();

	return 0;
}

static int IntcInitFunction(u16 DeviceId)
{
	XScuGic_Config *IntcConfig;

	// Interrupt controller initialization
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	XScuGic_SetPriorityTriggerType(&INTCInst, INT_LRCLK, 0, 3);	//Max priority, rising edge.

	status = XScuGic_Connect(&INTCInst,
								INT_LRCLK,
								 (Xil_ExceptionHandler)LRCLK_Intr_Handler,
								 (void *) 0);
	if(status != XST_SUCCESS) return XST_FAILURE;

	XScuGic_Enable(&INTCInst, INT_LRCLK);

	return XST_SUCCESS;
}

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	/*
	 * Initialize the interrupt controller driver so that it is ready to use.
	 * */
	Xil_ExceptionInit();

	// Enable interrupt


	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}

void LRCLK_Intr_Handler(void *data)
{
	/*y = amplitud*sin((2*M_PI*f*t)/fs);
	val = (int) (y*65535);
	sval = (val<<16) | val;
	//sval = 0xFFFFAAAA;
	TP_TD4_I2S_BEHAVIORAL_mWriteReg(I2S_TX_Behavioral, 0, sval);
	TP_TD4_I2S_STRUCTURAL_mWriteReg(I2S_TX_Structural, 0, sval);
	t++;
	if(t>fs)
		t=0;*/

	val = (int) (acdc[ind]*65535*0.05);
	sval = (val<<16) | val;
	TP_TD4_I2S_BEHAVIORAL_mWriteReg(I2S_TX_Behavioral, 0, sval);
	TP_TD4_I2S_STRUCTURAL_mWriteReg(I2S_TX_Structural, 0, sval);
	ind++;
	if(ind>190118)
		ind=0;
}
