/***********************************************************************************************************************
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#include "rs_gpio_debounce_cfg_fpb_ra4e2.h"
#include "common_data.h"

gpio_debounce_ctrl_t g_gpio_debounce_ctrl = { 0 };
gpio_debounce_cfg_t g_gpio_debounce_cfg =
{
    .active_level      = BSP_IO_LEVEL_LOW,
    .count_to_active   = 5,
    .count_to_inactive = 5,
    .event_enable_mask = GPIO_DEBOUNCE_EVENT_PRESSED,
    .p_ioport          = &g_ioport,
    .portpin           = BSP_IO_PORT_03_PIN_04,
    .p_callback        = &button_cb,
    .p_context         = NULL
};
