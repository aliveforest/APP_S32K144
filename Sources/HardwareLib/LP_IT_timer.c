#include "S32K144.h"
#include "LP_IT_timer.h"
#include "LPUART.h"

void LPIT0_init(void) {
    /* LPIT Clocking: ============================== */
    PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* 时钟源 = 6 (SPLL2_DIV2_CLK 40MHz)*/
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* 使能时钟至 LPIT0 寄存器         */
    /* LPIT Initialization: */
    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK; /* DBG_EN-0: Timer chans stop in Debug mode */
                                       /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                                       /* SW_RST=0: SW reset does not reset timer chans, regs */
                                       /* M_CEN=1: 启用模块 clk（允许写入其他 LPIT0 寄存器） */
    LPIT0->MIER = LPIT_MIER_TIE0(1); /* TIE0=1：为 Chan 0 启用定时器中断*/
    LPIT0->TMR[0].TVAL = (uint32_t)(2000);     /* Chan 0 Timeout period: 40M clocks 为1s*/
    LPIT0->TMR[0].TCTRL = LPIT_TMR_TCTRL_T_EN(1);       // 开启定时器
                                        /* T_EN=1: 启用定时器通道 */
                                        /* CHAIN=0: 禁用通道链 */
                                        /* MODE=0: 32 周期计数器模式 */
                                        /* TSOT=0: 定时器在重启后立即递减 */
                                        /* TSOI=0：超时后定时器不停止 */
                                        /* TROT=0 定时器不会在触发时重新加载 */
                                        /* TRG_SRC=0: 外部触发源 */
                                        /* TRG_SEL=0: 定时/计数器第 0 个触发源被选中*/
    LPIT_NVIC_init_IRQs(LPIT0_Ch0_IRQn, 0x06); /* 启用所需的中断和优先级 优先级越高，数值越小 */
}

/* 中断配置 */
void LPIT_NVIC_init_IRQs (uint32_t vector_number, uint32_t priority) {
	uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);
	/* 清除任何挂起的 IRQ */
	S32_NVIC->ICPR[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 使能 IRQ */
	S32_NVIC->ISER[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 优先级设置 */
	S32_NVIC->IP[(uint32_t)vector_number] = (uint8_t)(((((uint32_t)priority) << shift)) & 0xFFUL);
}


/* 用于统计运行时间 */
extern volatile uint32_t CPU_RunTime = 0UL;

void LPIT0_Ch0_IRQHandler(void) {
    LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /*  清除 LPIT0 定时器标志 0 */
    ++CPU_RunTime;
//    PTD-> PTOR |= 1<<15; //red
//	PTD-> PTOR |= 1<<1;
}



