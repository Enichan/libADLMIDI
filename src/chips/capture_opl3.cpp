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
    //, m_chip(new ADL_JavaOPL3::OPL3(true))
{
    reset();
}

CaptureOPL3::~CaptureOPL3()
{
    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);
    //delete chip_r;
}

void CaptureOPL3::setRate(uint32_t rate)
{
    OPLChipBaseBufferedT::setRate(rate);
    m_rate = rate;
    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);
    //chip_r->Reset();

    //float pan = sinf((float)M_SQRT1_2);
    //for (unsigned channel = 0; channel < 18; ++channel)
    //    chip_r->SetPanning(channel, pan, pan);
}

void CaptureOPL3::reset()
{
    OPLChipBaseBufferedT::reset();

    m_cmdbuff.clear();

    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);
    //chip_r->Reset();
}

void CaptureOPL3::writeReg(uint16_t addr, uint8_t data)
{
    m_cmdbuff.push_back({ addr, data });

    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);
    //chip_r->WriteReg(addr, data);
}

void CaptureOPL3::writePan(uint16_t addr, uint8_t data)
{
    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);

    //unsigned high = (addr >> 8) & 0x01;
    //unsigned regm = addr & 0xff;
    //unsigned channel = 9 * high + (regm & 0x0f);

    //float phase = (data == 63 || data == 64) ? 63.5f : (float)data;
    //phase *= (float)(M_PI / 2 / 127);
    //chip_r->SetPanning(channel, cosf(phase), sinf(phase));
}

void CaptureOPL3::nativeGenerateN(int16_t *output, size_t frames)
{
    if (captureCallback && m_cmdbuff.size() > 0) {
        captureCallback(&m_cmdbuff[0], m_cmdbuff.size());
    }

    m_cmdbuff.clear();

    //ADL_JavaOPL3::OPL3 *chip_r = reinterpret_cast<ADL_JavaOPL3::OPL3 *>(m_chip);

    //enum { maxframes = 256 };

    //float buf[2 * maxframes];
    //while(frames > 0)
    //{
    //    memset(buf, 0, sizeof(buf));

    //    size_t curframes = (frames < (size_t)maxframes) ? frames : (size_t)maxframes;
    //    chip_r->Update(buf, (int)curframes);

    //    size_t cursamples = 2 * curframes;
    //    for(size_t i = 0; i < cursamples; ++i)
    //    {
    //        int32_t sample = (int32_t)lround(4096 * buf[i]);
    //        sample = (sample > -32768) ? sample : -32768;
    //        sample = (sample < +32767) ? sample : +32767;
    //        output[i] = (int16_t)sample;
    //    }

    //    output += cursamples;
    //    frames -= curframes;
    //}
}

const char * CaptureOPL3::emulatorName()
{
    return "OPL3 Register Capture Device";
}

OPLChipBase::ChipType CaptureOPL3::chipType()
{
    return CHIPTYPE_OPL3;
}
