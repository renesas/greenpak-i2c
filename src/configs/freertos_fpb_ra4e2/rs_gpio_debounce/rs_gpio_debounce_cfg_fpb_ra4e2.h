/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef RS_GPIO_DEBOUNCE_RS_GPIO_DEBOUNCE_CFG_FPB_RA4E2_H_
#define RS_GPIO_DEBOUNCE_RS_GPIO_DEBOUNCE_CFG_FPB_RA4E2_H_

#include "rs_gpio_debounce/rs_gpio_debounce.h"

extern gpio_debounce_ctrl_t g_gpio_debounce_ctrl;
extern gpio_debounce_cfg_t g_gpio_debounce_cfg;

extern void button_cb(const gpio_debounce_callback_args_t * p_args);

#endif /* RS_GPIO_DEBOUNCE_RS_GPIO_DEBOUNCE_CFG_FPB_RA4E2_H_ */
