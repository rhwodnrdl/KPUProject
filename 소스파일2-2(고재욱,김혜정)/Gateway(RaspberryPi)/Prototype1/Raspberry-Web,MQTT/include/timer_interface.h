/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

/**
 * @file timer_interface.h
 * @brief Timer interface definition for MQTT client.
 *
 * Defines an interface to timers that can be used by other system
 * components.  MQTT client requires timers to handle timeouts and
 * MQTT keep alive.
 * Starting point for porting the SDK to the timer hardware layer of a new platform.
 */
#pragma once
#ifndef __TIMER_INTERFACE_H_
#define __TIMER_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The platform specific timer header that defines the Timer struct
 */
#include "timer_platform.h"

#include <stdint.h>
#include <stdbool.h>


typedef struct Timer Timer;

bool has_timer_expired(Timer *);

void countdown_ms(Timer *, __time_t);

void countdown_sec(Timer *, __time_t);

uint32_t left_ms(Timer *);

void init_timer(Timer *);

#ifdef __cplusplus
}
#endif

#endif //__TIMER_INTERFACE_H_
