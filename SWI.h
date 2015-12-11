/**
 * @brief Mechanism to invoke a software interrupt. This allows execution
 * to jump into privileged(interrupt handling) context.
 * Depending on priority assigned you could allow selective interruption.
 */
/** @example
			// Function we want to execute in SVC 
			void testSWI(void *args)
			{
			uint32_t pI = *(uint32_t *)args;
			}
			
			uint32_t z = 3.1416;
			
	@usage SWI_Req(1, testSWI, (void*)(&z));
		This ensures testSWI will execute as a software interrupt at
		1 - interrupt priority level. Any interrupt with priority more than 1
		cannot preempt SVC. Any interrupt more or equal to @param pri
		cannot prempt execution of this function
 */
	 

#ifndef __SWI_H__
#define __SWI_H__

/* using only one SVC number but can be used to provide different 
	 privileged(OS) services depending on SVC numbers */

#define SVC_NUM							0x01

typedef void (*SWI_func)(void *);	/* Functions executed in SVC will have
														to be of this type */

/* SVC call which triggers exception */
/* This function call will trigger (SVC 0x01). Each svc call with a different 
	 arg can be aliased to a different function call */
void __svc(SVC_NUM)	SWI_Req(uint32_t pri, SWI_func func, void *fargs);
 
 
/**
 * @brief	SVC handler which invokes RL_SWI_handler
 * Overrides weak default definition
 */
void SVC_Handler(void);
 

#endif /* __RL_SWI_H__ */
