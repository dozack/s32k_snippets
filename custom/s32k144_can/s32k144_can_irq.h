/*
 * s32k144_can_irq.h
 *
 *  Created on: Nov 17, 2020
 *      Author: zoltan
 */

#ifndef S32K144_CAN_S32K144_CAN_IRQ_H_
#define S32K144_CAN_S32K144_CAN_IRQ_H_

extern void CAN0_ORed_IRQHandler(void);
extern void CAN0_Error_IRQHandler(void);
extern void CAN0_Wake_Up_IRQHandler(void);
extern void CAN0_ORed_0_15_MB_IRQHandler(void);
extern void CAN0_ORed_16_31_MB_IRQHandler(void);

extern void CAN1_ORed_IRQHandler(void);
extern void CAN1_Error_IRQHandler(void);
extern void CAN1_ORed_0_15_MB_IRQHandler(void);
extern void CAN1_ORed_16_31_MB_IRQHandler(void);

extern void CAN2_ORed_IRQHandler(void);
extern void CAN2_Error_IRQHandler(void);
extern void CAN2_ORed_0_15_MB_IRQHandler(void);
extern void CAN2_ORed_16_31_MB_IRQHandler(void);

#endif /* S32K144_CAN_S32K144_CAN_IRQ_H_ */
