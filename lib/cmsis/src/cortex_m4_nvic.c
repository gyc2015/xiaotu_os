#include <cortex_m4_nvic.h>
#include <cortex_m4_scb.h>

void NVIC_SetPriorityGroup(uint32 group) {
    SCB->AIRCR = SCB_AIRCR_KEY_VALUE | group;
}

//
//void NVIC_EnableIRQ(IRQn_Type IRQn) {}
//
//void NVIC_DisableIRQ(IRQn_Type IRQn) {}
//
//void NVIC_SetPendingIRQ(IRQn_Type IRQn) {}
//
//void NVIC_ClearPendingIRQ(IRQn_Type IRQn) {}
//
//uint32 NVIC_GetPendingIRQ(IRQn_Type IRQn) {
//    return 0;
//}
//
//void NVIC_SetPriority(IRQn_Type IRQn, uint32 priority) {}
//
//uint32 NVIC_GetPriority(IRQn_Type IRQn) {
//    return 0;
//}
