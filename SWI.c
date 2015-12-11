#include "SWI.h"
#include "cmsis.h"

/* reference - 
	The Definitive Guide to the ARM®Cortex-M3
	See section(using SVC with C function call) in chapter 'Exception prog'
*/

/* When svc call was triggered, cortex m3 pushes registers, PC etc.
	 to stack as part of 'saving context'.
	 First arg to SVC call will be in 	r0 	- svc_arg[0]
	 2nd		"		"				ri 	- svc_arg[1]
	 PC is the 7th register 				- svc_arg[6]
*/

void SVC_Handler_C(uint32_t *svc_args)
{
									/* upper byte of PC gives svc num */
	uint32_t 			svc_num  	= ((uint8_t *)svc_args[6])[-2];
	uint32_t			priority 	= 	svc_args[0];
	SWI_func   func 		 	 	= (SWI_func)svc_args[1];
	void 					*args   = (void *)svc_args[2];
	
	/* Currently only svc 1 is being used */
	if (svc_num == SVC_NUM) {
		NVIC_SetPriority(SVCall_IRQn, priority);
		func(args);
	}
	/* Will return where SVC was triggered */
}

/* If you knew which stack pointer was being used when SVC was triggered
	 the following code to determine stack ptr wouldn't be needed. */

/* This piece of code will determine which stack was being used before
	 SVC call. Pri, function pointer and arguments have been pushed 
	 to that stack. Extract the SP, pass it as argument(r0) to SVC_Handler_C 
	 Thus r0 will contain 'stack frame location'.*/
__asm void SVC_Handler(void)
{
	IMPORT SVC_Handler_C
	TST LR, #4 
	ITE EQ
	MRSEQ R0, MSP
	MRSNE R0, PSP
	B SVC_Handler_C
}
