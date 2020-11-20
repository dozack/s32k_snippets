/*
 * s32k144_can.c
 *
 *  Created on: Nov 15, 2020
 *      Author: zoltan
 */

#include "s32k144_can.h"

#include "S32K144.h"

#include "armv7m.h"

/* PRIVATE DEFINES AND VARIABLES */
/* ------------------------------------------------------------------------------------------*/

enum {
    CAN_MB_RX_INACTIVE = 0x00,
    CAN_MB_RX_FULL = 0x02,
    CAN_MB_RX_EMPTY = 0x04,
    CAN_MB_RX_OVERRUN = 0x06,
    CAN_MB_RX_BUSY = 0x08,
    CAN_MB_RX_RANSWER = 0x0a,
    CAN_MB_RX_UNUSED = 0x0f,

    CAN_MB_TX_INACTIVE = 0x08,
    CAN_MB_TX_ABORT = 0x09,
    CAN_MB_TX_DATA = 0x0c,
    CAN_MB_TX_REMOTE = 0x1c,
    CAN_MB_TX_TANSWER = 0x0a,
    CAN_MB_TX_UNUSED = 0x0f
};

#define CAN_ID_EXT_SHIFT                        (0)
#define CAN_ID_EXT_MASK                         (uint32_t)(0x3ffff << CAN_ID_EXT_SHIFT)
#define CAN_ID_STD_SHIFT                        (18)
#define CAN_ID_STD_MASK                         (uint32_t)(0x1ffc << CAN_ID_STD_SHIFT)
#define CAN_ID_PRIO_SHIFT                       (29)
#define CAN_ID_PRIO_MASK                        (uint32_t)(0x7 << CAN_ID_PRIO_SHIFT)
/* CS Bit Fields */
#define CAN_CS_TIME_STAMP_SHIFT                 (0)
#define CAN_CS_TIME_STAMP_MASK                  (uint32_t)(0xffff << CAN_CS_TIME_STAMP_SHIFT)
#define CAN_CS_DLC_SHIFT                        (16)
#define CAN_CS_DLC_MASK                         (uint32_t)(0xf << CAN_CS_DLC_SHIFT)
#define CAN_CS_RTR_SHIFT                        (20)
#define CAN_CS_RTR_MASK                         (uint32_t)(1 << CAN_CS_RTR_SHIFT)
#define CAN_CS_IDE_SHIFT                        (21)
#define CAN_CS_IDE_MASK                         (uint32_t)(1 << CAN_CS_IDE_SHIFT)
#define CAN_CS_SRR_SHIFT                        (22)
#define CAN_CS_SRR_MASK                         (uint32_t)(1 << CAN_CS_SRR_SHIFT)
#define CAN_CS_CODE_SHIFT                       (24)
#define CAN_CS_CODE_MASK                        (uint32_t)(0xf << CAN_CS_CODE_SHIFT)
#define CAN_MB_EDL_SHIFT                        (31)
#define CAN_MB_EDL_MASK                         (uint32_t)(1 << CAN_MB_EDL_SHIFT)
#define CAN_MB_BRS_SHIFT                        (30)
#define CAN_MB_BRS_MASK                         (uint32_t)(1 << CAN_MB_BRS_SHIFT)

typedef struct {
    s32k144_can_t *instances[CAN_INSTANCE_NUM];
} s32k144_can_driver_t;

static s32k144_can_driver_t s32k144_can_driver;

static CAN_Type * const _s32k144_can_base_pool[CAN_INSTANCE_NUM] = {
        CAN0,
        CAN1,
        CAN2 };

static const uint32_t _s32k144_can_pcc_pool[CAN_INSTANCE_NUM] = {
        PCC_FlexCAN0_INDEX,
        PCC_FlexCAN1_INDEX,
        PCC_FlexCAN2_INDEX };

static const uint32_t _s32k144_can_mb_count[CAN_INSTANCE_NUM] = {
        32,
        16,
        16 };

/* PRIVATE FUNCTIONS */
/* ------------------------------------------------------------------------------------------*/

static void _s32k144_can_clear_ram(uint32_t instance) {
    if (instance > CAN_INSTANCE_NUM)
    {
        return;
    }
    CAN_Type *base = _s32k144_can_base_pool[instance];
    uint32_t ram_size = _s32k144_can_mb_count[instance] * 4;
    uint32_t rximr_size = _s32k144_can_mb_count[instance];
    volatile uint32_t * ram = base->RAMn;

    uint32_t data_idx;
    for (data_idx = 0; data_idx < ram_size; data_idx++)
    {
        ram[data_idx] = 0x00;
    }
    ram = base->RXIMR;
    for (data_idx = 0; data_idx < rximr_size; data_idx++)
    {
        ram[data_idx] = 0x00;
    }
}

volatile uint32_t * _s32k144_can_get_mailbox(uint32_t instance, uint32_t index) {
    if (index > _s32k144_can_mb_count[instance])
    {
        return NULL;
    }
    CAN_Type *base = _s32k144_can_base_pool[instance];
    uint32_t mb_size = 16;
    uint32_t max_mb_num = 32;
    uint32_t ram_block_offset = 128 * (index / max_mb_num);
    uint32_t mb_index = ram_block_offset + ((index % max_mb_num) * (mb_size >> 2));
    return &(base->RAMn[mb_index]);
}

void _s32k144_can_set_rx_mailbox(uint32_t instance, uint32_t index, const s32k144_can_mb_code_t *cfg, uint32_t id,
                                 uint32_t code) {
    if ((cfg == NULL) && (index > _s32k144_can_mb_count[instance]))
    {
        return;
    }
    volatile uint32_t *mb = _s32k144_can_get_mailbox(instance, index);
    volatile uint32_t *mb_id = &mb[1];
    *mb = 0;
    *mb_id = 0;
    if (cfg->id_type == CAN_ID_STD)
    {
        *mb &= ~(CAN_CS_IDE_MASK | CAN_CS_SRR_MASK );
        *mb_id = ~CAN_ID_STD_MASK;
        *mb_id |= (id << CAN_ID_STD_SHIFT) & CAN_ID_STD_MASK;
    }
    if (cfg->id_type == CAN_ID_EXT)
    {
        *mb |= CAN_CS_IDE_MASK;
        *mb &= ~CAN_CS_SRR_MASK;
        *mb_id &= ~(CAN_ID_STD_MASK | CAN_ID_EXT_MASK );
        *mb_id |= (id & (CAN_ID_STD_MASK | CAN_ID_EXT_MASK ));
    }
    if (code != CAN_MB_RX_UNUSED)
    {
        *mb &= ~CAN_CS_CODE_MASK;
        *mb |= (code << CAN_CS_CODE_SHIFT) & CAN_CS_CODE_MASK;
    }
    return;
}

void _s32k144_can_reset_rx_mailbox(uint32_t instance, uint32_t index) {
    uint32_t mb_config = 0;
    uint32_t code = CAN_MB_RX_INACTIVE;
    volatile uint32_t *mb = _s32k144_can_get_mailbox(instance, index);
    mb_config = *mb;

    /* reset mailbox */
    mb_config &= ~CAN_CS_CODE_MASK;
    mb_config |= (code << CAN_CS_CODE_SHIFT) & CAN_CS_CODE_MASK;
    *mb = mb_config;
    /* init mailbox again */
    code = CAN_MB_RX_EMPTY;
    mb_config &= ~CAN_CS_CODE_MASK;
    mb_config |= (code << CAN_CS_CODE_SHIFT) & CAN_CS_CODE_MASK;
    *mb = mb_config;
    return;
}

void _s32k144_can_abort_tx_mailbox(uint32_t instance, uint32_t index) {
    uint32_t mb_config = 0;
    uint32_t code = CAN_MB_TX_ABORT;
    volatile uint32_t *mb = _s32k144_can_get_mailbox(instance, index);
    mb_config = *mb;
    mb_config &= ~CAN_CS_CODE_MASK;
    mb_config |= (code << CAN_CS_CODE_SHIFT) & CAN_CS_CODE_MASK;
    *mb = mb_config;
}

/* PUBLIC FUNCTIONS */
/* ------------------------------------------------------------------------------------------*/

void s32k144_can_create(s32k144_can_t *can, uint32_t instance, s32k144_can_cfg_t *config) {
    if (instance >= CAN_INSTANCE_COUNT)
    {
        return;
    }
    /* reset all structure values */
    *can = (s32k144_can_t ) {
                    0 };
    CAN_Type *base = _s32k144_can_base_pool[instance];
    can->base = base;
    can->instance = instance;
    can->clock = config->clock_source;
    can->mode = config->mode;
    can->timing = config->timing;
    s32k144_can_driver.instances[instance] = can;
    can->state = CAN_STATE_INIT;
    return;
}

void s32k144_can_destroy(s32k144_can_t *can) {
    if (can->state != CAN_STATE_INIT)
    {
        return;
    }
    can->callback_ev = NULL;
    can->context_ev = NULL;
    can->callback_err = NULL;
    can->context_err = NULL;
    s32k144_can_driver.instances[can->instance] = NULL;
    can->state = CAN_STATE_NONE;
    return;
}

void s32k144_can_enable(s32k144_can_t *can) {
    if (can->state != CAN_STATE_INIT)
    {
        return;
    }
    PCC->PCCn[_s32k144_can_pcc_pool[can->instance]] |= PCC_PCCn_CGC_MASK;
    CAN_Type *base = can->base;
    if (!(base->MCR & CAN_MCR_MDIS_MASK))
    {
        /* can is enabled, enter freeze mode and disable */
        base->MCR |= CAN_MCR_FRZ_MASK;
        base->MCR |= CAN_MCR_HALT_MASK;
        while (base->MCR & CAN_MCR_FRZACK_MASK)
            ;
        base->MCR |= CAN_MCR_MDIS_MASK;
    }
    /* set clock source */
    base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_CLKSRC_MASK) | (uint32_t) (can->clock << CAN_CTRL1_CLKSRC_SHIFT);
    /* enable and enter freeeze mode immediately */
    base->MCR &= ~CAN_MCR_MDIS_MASK;
    while (base->MCR & CAN_MCR_LPMACK_MASK)
        ;
    base->MCR |= CAN_MCR_FRZ_MASK;
    base->MCR |= CAN_MCR_HALT_MASK;
    while (!(base->MCR & CAN_MCR_FRZACK_MASK))
        ;
    base->MCR = (base->MCR & ~CAN_MCR_SOFTRST_MASK) | CAN_MCR_SOFTRST_MASK;
    while (base->MCR & CAN_MCR_SOFTRST_MASK)
        ;
    base->MCR |= (CAN_MCR_AEN_MASK | CAN_MCR_IRMQ_MASK);
    _s32k144_can_clear_ram(can->instance);
#if 0
    /* legacy functionality - not needed when MCR[IRMQ] is set */
    base->RXMGMASK = CAN_RXMGMASK_MG_MASK;
    base->RX14MASK = CAN_RX14MASK_RX14M_MASK;
    base->RX15MASK = CAN_RX15MASK_RX15M_MASK;
#endif
// TODO: clear mailbox memory
    /* reset interrupts */
    base->IMASK1 = 0x0;
    base->IFLAG1 = CAN_IMASK1_BUF31TO0M_MASK;
    /* wkup, error, busoff interrupt masks */
    base->ESR1 = 0x3B0006U;
    /* set state to freeze allowing configuration calls */
    can->state = CAN_STATE_FREEZE;
    s32k144_can_set_timing(can, can->timing);
    s32k144_can_set_mode(can, can->mode);
    if (can->mode != CAN_MODE_FREEZE)
    {
        /* leave freeeze mode */
        base->MCR &= ~CAN_MCR_HALT_MASK;
        base->MCR &= ~CAN_MCR_FRZ_MASK;
        while (base->MCR & CAN_MCR_FRZACK_MASK)
            ;
    }
    /* enable interrupts */
    switch (can->instance)
    {
        case CAN_INSTANCE_CAN0:
            armv7m_nvic_enable_irq(CAN0_ORed_IRQn);
            armv7m_nvic_enable_irq(CAN0_ORed_0_15_MB_IRQn);
            armv7m_nvic_enable_irq(CAN0_Error_IRQn);
            /* flexcan 0 specific interrupts */
            armv7m_nvic_enable_irq(CAN0_ORed_16_31_MB_IRQn);
            armv7m_nvic_enable_irq(CAN0_Wake_Up_IRQn);
            break;
        case CAN_INSTANCE_CAN1:
            armv7m_nvic_enable_irq(CAN1_ORed_IRQn);
            armv7m_nvic_enable_irq(CAN1_ORed_0_15_MB_IRQn);
            armv7m_nvic_enable_irq(CAN1_Error_IRQn);
            break;
        case CAN_INSTANCE_CAN2:
            armv7m_nvic_enable_irq(CAN2_ORed_IRQn);
            armv7m_nvic_enable_irq(CAN2_ORed_0_15_MB_IRQn);
            armv7m_nvic_enable_irq(CAN2_Error_IRQn);
            break;
        default:
            break;
    }
    can->state = CAN_STATE_READY;
    return;
}

void s32k144_can_disable(s32k144_can_t *can) {
    if (can->state != CAN_STATE_READY)
    {
        return;
    }
    CAN_Type *base = can->base;
    base->MCR |= CAN_MCR_FRZ_MASK;
    base->MCR |= CAN_MCR_HALT_MASK;
    while (base->MCR & CAN_MCR_FRZACK_MASK)
        ;
    if (!(base->MCR & CAN_MCR_MDIS_MASK))
    {
        base->MCR = ((base->MCR & ~CAN_MCR_MDIS_MASK) | CAN_MCR_MDIS_MASK);
        while (!(base->MCR & CAN_MCR_LPMACK_MASK))
            ;
    }
    switch (can->instance)
    {
        case CAN_INSTANCE_CAN0:
            armv7m_nvic_disable_irq(CAN0_ORed_IRQn);
            armv7m_nvic_disable_irq(CAN0_ORed_0_15_MB_IRQn);
            armv7m_nvic_disable_irq(CAN0_Error_IRQn);
            /* flexcan 0 specific interrupts */
            armv7m_nvic_disable_irq(CAN0_ORed_16_31_MB_IRQn);
            armv7m_nvic_disable_irq(CAN0_Wake_Up_IRQn);
            break;
        case CAN_INSTANCE_CAN1:
            armv7m_nvic_disable_irq(CAN1_ORed_IRQn);
            armv7m_nvic_disable_irq(CAN1_ORed_0_15_MB_IRQn);
            armv7m_nvic_disable_irq(CAN1_Error_IRQn);
            break;
        case CAN_INSTANCE_CAN2:
            armv7m_nvic_disable_irq(CAN2_ORed_IRQn);
            armv7m_nvic_disable_irq(CAN2_ORed_0_15_MB_IRQn);
            armv7m_nvic_disable_irq(CAN2_Error_IRQn);
            break;
        default:
            break;
    }
    can->state = CAN_STATE_INIT;
    return;
}

void s32k144_can_freeze(s32k144_can_t *can) {
    if (can->state != CAN_STATE_READY)
    {
        return;
    }
    CAN_Type *base = can->base;
    base->MCR = ((base->MCR & ~CAN_MCR_FRZ_MASK) | CAN_MCR_FRZ_MASK);
    base->MCR = ((base->MCR & ~CAN_MCR_HALT_MASK) | CAN_MCR_HALT_MASK);
    /* save disabled mode state */
    uint32_t en = base->MCR & CAN_MCR_MDIS_MASK;
    base->MCR &= ~CAN_MCR_MDIS_MASK;
    while (!(base->MCR & CAN_MCR_FRZACK_MASK))
        ;
    /* restore disabled mode state */
    base->MCR |= en;
    can->state = CAN_STATE_FREEZE;
}

void s32k144_can_unfreeze(s32k144_can_t *can) {
    if (can->state != CAN_STATE_FREEZE)
    {
        return;
    }
    CAN_Type *base = can->base;
    base->MCR &= ~CAN_MCR_HALT_MASK;
    base->MCR &= ~CAN_MCR_FRZ_MASK;
    while (base->MCR & CAN_MCR_FRZACK_MASK)
        ;
    can->state = CAN_STATE_READY;
}

void s32k144_can_set_timing(s32k144_can_t *can, s32k144_can_timing_t timing) {
    if (can->state != CAN_STATE_FREEZE)
    {
        return;
    }
    CAN_Type *base = can->base;
    (base->CTRL1) = ((base->CTRL1)
        & ~((CAN_CTRL1_PROPSEG_MASK | CAN_CTRL1_PSEG2_MASK | CAN_CTRL1_PSEG1_MASK | CAN_CTRL1_PRESDIV_MASK)
            | CAN_CTRL1_RJW_MASK));

    (base->CTRL1) = ((base->CTRL1)
        | (CAN_CTRL1_PROPSEG(timing.prop_seg) | CAN_CTRL1_PSEG2(timing.phase_seg2) | CAN_CTRL1_PSEG1(timing.phase_seg1)
            | CAN_CTRL1_PRESDIV(timing.pre_divider) | CAN_CTRL1_RJW(timing.rjump_width)));
    can->timing = timing;
}

s32k144_can_timing_t s32k144_can_get_timing(s32k144_can_t *can) {
    return can->timing;
}

void s32k144_can_set_mode(s32k144_can_t *can, s32k144_can_mode_t mode) {
    if (can->state != CAN_STATE_FREEZE)
    {
        return;
    }
    CAN_Type *base = can->base;
    /* disable self reception */
    base->MCR |= CAN_MCR_SRXDIS_MASK;
    switch (mode)
    {
        case CAN_MODE_DISABLE:
            base->MCR = CAN_MCR_MDIS_MASK;
            break;
        case CAN_MODE_FREEZE:
            /* can must be in freeze mode in order to set operation mode */
            break;
        case CAN_MODE_LISTEN_ONLY:
            base->CTRL1 = ((base->CTRL1 & ~CAN_CTRL1_LOM_MASK) | CAN_CTRL1_LOM_MASK);
        case CAN_MODE_LOOPBACK:
            /* enable self reception */
            base->MCR &= ~CAN_MCR_SRXDIS_MASK;
            base->CTRL1 = ((base->CTRL1 & ~CAN_CTRL1_LPB_MASK) | CAN_CTRL1_LPB_MASK);
            base->CTRL1 = ((base->CTRL1 & ~CAN_CTRL1_LOM_MASK) | CAN_CTRL1_LOM_MASK);
            break;
        case CAN_MODE_NORMAL:
            base->MCR &= ~CAN_MCR_SUPV_MASK;
            base->CTRL1 &= ~CAN_CTRL1_LOM_MASK;
            base->CTRL1 &= ~CAN_CTRL1_LPB_MASK;
            break;
        default:
            break;
    }
    can->mode = mode;
}

s32k144_can_mode_t s32k144_can_get_mode(s32k144_can_t *can) {
    return can->mode;
}

void s32k144_can_notify_on_event(s32k144_can_t *can, s32k144_can_callback_t cb, void *context) {
    if ((can->state != CAN_STATE_READY) && (can->state != CAN_STATE_INIT))
    {
        return;
    }
    can->callback_ev = cb;
    can->context_ev = context;

}

void s32k144_can_notify_on_error(s32k144_can_t *can, s32k144_can_callback_t cb, void *context) {
    if ((can->state != CAN_STATE_READY) && (can->state != CAN_STATE_INIT))
    {
        return;
    }
    can->callback_err = cb;
    can->context_err = context;

}

