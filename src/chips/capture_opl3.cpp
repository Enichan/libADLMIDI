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

#include "capture_opl3.hpp"

CaptureOPL3::CaptureOPL3() :
    OPLChipBaseBufferedT()
{
    reset();
}

CaptureOPL3::~CaptureOPL3()
{
}

void CaptureOPL3::setRate(uint32_t rate)
{
    OPLChipBaseBufferedT::setRate(rate);
    m_rate = rate;
}

void CaptureOPL3::reset()
{
    OPLChipBaseBufferedT::reset();

    m_cmdbuff.clear();
}

void CaptureOPL3::writeReg(uint16_t addr, uint8_t data)
{
    m_cmdbuff.push_back({ addr, data });
}

void CaptureOPL3::writePan(uint16_t addr, uint8_t data)
{
}

void CaptureOPL3::nativeGenerateN(int16_t *output, size_t frames)
{
    if (captureCallback && m_cmdbuff.size() > 0) {
        captureCallback(&m_cmdbuff[0], m_cmdbuff.size());
    }

    m_cmdbuff.clear();
}

const char * CaptureOPL3::emulatorName()
{
    return "OPL3 Register Capture Device";
}

OPLChipBase::ChipType CaptureOPL3::chipType()
{
    return CHIPTYPE_OPL3;
}
