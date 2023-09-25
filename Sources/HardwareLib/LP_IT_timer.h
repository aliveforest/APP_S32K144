#ifndef LP_IT_TIMER_H_
#define LP_IT_TIMER_H_


void LPIT0_init (void);
void LPIT_NVIC_init_IRQs (uint32_t vector_number, uint32_t priority);
void LPIT0_Ch0_IRQHandler(void);

#endif
