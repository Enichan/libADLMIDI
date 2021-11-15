/*
 * libADLMIDI capture chip
 *
 * Copyright (c) 2021 Eniko Fox (Emma Maassen)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef CAPTURE_CB_H
#define CAPTURE_CB_H

#include "stdint.h"

typedef struct RegWriteCmd {
    uint16_t addr;
    uint8_t data;
} RegWriteCmd;

#ifdef __cplusplus
extern "C" {
#endif    

typedef void (*captureCallbackFunc)(RegWriteCmd* commands, size_t count);
extern captureCallbackFunc captureCallback;

void captureOPL3_setCallback(captureCallbackFunc cb);

#ifdef __cplusplus
}
#endif
#endif // CAPTURE_CB_H
