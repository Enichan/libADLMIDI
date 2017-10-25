/*
 * libADLMIDI is a free MIDI to WAV conversion library with OPL3 emulation
 *
 * Original ADLMIDI code: Copyright (c) 2010-2014 Joel Yliluoma <bisqwit@iki.fi>
 * ADLMIDI Library API:   Copyright (c) 2017 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * Library is based on the ADLMIDI, a MIDI player for Linux and Windows with OPL3 emulation:
 * http://iki.fi/bisqwit/source/adlmidi.html
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "adlmidi_private.hpp"

static const unsigned MaxCards = 100;

/*---------------------------EXPORTS---------------------------*/

ADLMIDI_EXPORT struct ADL_MIDIPlayer *adl_init(long sample_rate)
{
    ADL_MIDIPlayer *midi_device;
    midi_device = (ADL_MIDIPlayer *)malloc(sizeof(ADL_MIDIPlayer));
    MIDIplay *player = new MIDIplay;
    midi_device->adl_midiPlayer = player;
    player->m_setup.PCM_RATE = static_cast<unsigned long>(sample_rate);
    player->m_setup.mindelay = 1.0 / (double)player->m_setup.PCM_RATE;
    player->m_setup.maxdelay = 512.0 / (double)player->m_setup.PCM_RATE;
    player->ChooseDevice("none");
    adlRefreshNumCards(midi_device);
    return midi_device;
}

ADLMIDI_EXPORT int adl_setNumCards(ADL_MIDIPlayer *device, int numCards)
{
    if(device == NULL)
        return -2;

    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.NumCards = static_cast<unsigned int>(numCards);
    if(play->m_setup.NumCards < 1 || play->m_setup.NumCards > MaxCards)
    {
        std::stringstream s;
        s << "number of cards may only be 1.." << MaxCards << ".\n";
        ADLMIDI_ErrorString = s.str();
        return -1;
    }

    play->opl.NumCards = play->m_setup.NumCards;

    return adlRefreshNumCards(device);
}

ADLMIDI_EXPORT int adl_setBank(ADL_MIDIPlayer *device, int bank)
{
    #ifdef DISABLE_EMBEDDED_BANKS
    ADL_UNUSED(device);
    ADL_UNUSED(bank);
    ADLMIDI_ErrorString = "This build of libADLMIDI has no embedded banks. Please load bank by using of adl_openBankFile() or adl_openBankData() functions instead of adl_setBank()";
    return -1;
    #else
    const uint32_t NumBanks = static_cast<uint32_t>(maxAdlBanks());
    int32_t bankno = bank;

    if(bankno < 0)
        bankno = 0;

    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.AdlBank = static_cast<uint32_t>(bankno);
    play->opl.AdlBank = play->m_setup.AdlBank;

    if(play->m_setup.AdlBank >= NumBanks)
    {
        std::stringstream s;
        s << "bank number may only be 0.." << (NumBanks - 1) << ".\n";
        ADLMIDI_ErrorString = s.str();
        return -1;
    }

    return adlRefreshNumCards(device);
    #endif
}

ADLMIDI_EXPORT int adl_getBanksCount()
{
    return maxAdlBanks();
}

ADLMIDI_EXPORT const char *const *adl_getBankNames()
{
    return banknames;
}

ADLMIDI_EXPORT int adl_setNumFourOpsChn(ADL_MIDIPlayer *device, int ops4)
{
    if(!device)
        return -1;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.NumFourOps = static_cast<unsigned int>(ops4);
    play->opl.NumFourOps = play->m_setup.NumFourOps;

    if(play->m_setup.NumFourOps > 6 * play->m_setup.NumCards)
    {
        std::stringstream s;
        s << "number of four-op channels may only be 0.." << (6 * (play->m_setup.NumCards)) << " when " << play->m_setup.NumCards << " OPL3 cards are used.\n";
        ADLMIDI_ErrorString = s.str();
        return -1;
    }

    return adlRefreshNumCards(device);
}


ADLMIDI_EXPORT void adl_setPercMode(ADL_MIDIPlayer *device, int percmod)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.AdlPercussionMode = (percmod != 0);
    play->opl.AdlPercussionMode = play->m_setup.AdlPercussionMode;
}

ADLMIDI_EXPORT void adl_setHVibrato(ADL_MIDIPlayer *device, int hvibro)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.HighVibratoMode = (hvibro != 0);
    play->opl.HighVibratoMode = play->m_setup.HighVibratoMode;
}

ADLMIDI_EXPORT void adl_setHTremolo(ADL_MIDIPlayer *device, int htremo)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.HighTremoloMode = (htremo != 0);
    play->opl.HighTremoloMode = play->m_setup.HighTremoloMode;
}

ADLMIDI_EXPORT void adl_setScaleModulators(ADL_MIDIPlayer *device, int smod)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.ScaleModulators = (smod != 0);
    play->opl.ScaleModulators = play->m_setup.ScaleModulators;
}

ADLMIDI_EXPORT void adl_setLoopEnabled(ADL_MIDIPlayer *device, int loopEn)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.loopingIsEnabled = (loopEn != 0);
}

ADLMIDI_EXPORT void adl_setLogarithmicVolumes(struct ADL_MIDIPlayer *device, int logvol)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.LogarithmicVolumes = (logvol != 0);
    play->opl.LogarithmicVolumes = play->m_setup.LogarithmicVolumes;
}

ADLMIDI_EXPORT void adl_setVolumeRangeModel(struct ADL_MIDIPlayer *device, int volumeModel)
{
    if(!device) return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.VolumeModel = volumeModel;
    play->opl.ChangeVolumeRangesModel(static_cast<ADLMIDI_VolumeModels>(volumeModel));
}

ADLMIDI_EXPORT int adl_openBankFile(struct ADL_MIDIPlayer *device, char *filePath)
{
    ADLMIDI_ErrorString.clear();

    if(device && device->adl_midiPlayer)
    {
        MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
        play->m_setup.stored_samples = 0;
        play->m_setup.backup_samples_size = 0;
        if(!play->LoadBank(filePath))
        {
            if(ADLMIDI_ErrorString.empty())
                ADLMIDI_ErrorString = "ADL MIDI: Can't load file";
            return -1;
        }
        else return 0;
    }

    ADLMIDI_ErrorString = "Can't load file: ADLMIDI is not initialized";
    return -1;
}

ADLMIDI_EXPORT int adl_openBankData(struct ADL_MIDIPlayer *device, void *mem, long size)
{
    ADLMIDI_ErrorString.clear();

    if(device && device->adl_midiPlayer)
    {
        MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
        play->m_setup.stored_samples = 0;
        play->m_setup.backup_samples_size = 0;
        if(!play->LoadBank(mem, static_cast<size_t>(size)))
        {
            if(ADLMIDI_ErrorString.empty())
                ADLMIDI_ErrorString = "ADL MIDI: Can't load data from memory";
            return -1;
        }
        else return 0;
    }

    ADLMIDI_ErrorString = "Can't load file: ADL MIDI is not initialized";
    return -1;
}

ADLMIDI_EXPORT int adl_openFile(ADL_MIDIPlayer *device, char *filePath)
{
    ADLMIDI_ErrorString.clear();

    if(device && device->adl_midiPlayer)
    {
        MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
        play->m_setup.stored_samples = 0;
        play->m_setup.backup_samples_size = 0;
        if(!play->LoadMIDI(filePath))
        {
            if(ADLMIDI_ErrorString.empty())
                ADLMIDI_ErrorString = "ADL MIDI: Can't load file";
            return -1;
        }
        else return 0;
    }

    ADLMIDI_ErrorString = "Can't load file: ADL MIDI is not initialized";
    return -1;
}

ADLMIDI_EXPORT int adl_openData(ADL_MIDIPlayer *device, void *mem, long size)
{
    ADLMIDI_ErrorString.clear();

    if(device && device->adl_midiPlayer)
    {
        MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
        play->m_setup.stored_samples = 0;
        play->m_setup.backup_samples_size = 0;
        if(!play->LoadMIDI(mem, static_cast<size_t>(size)))
        {
            if(ADLMIDI_ErrorString.empty())
                ADLMIDI_ErrorString = "ADL MIDI: Can't load data from memory";
            return -1;
        }
        else return 0;
    }

    ADLMIDI_ErrorString = "Can't load file: ADL MIDI is not initialized";
    return -1;
}


ADLMIDI_EXPORT const char *adl_errorString()
{
    return ADLMIDI_ErrorString.c_str();
}

ADLMIDI_EXPORT const char *adl_getMusicTitle(ADL_MIDIPlayer *device)
{
    if(!device)
        return "";
    return reinterpret_cast<MIDIplay *>(device->adl_midiPlayer)->musTitle.c_str();
}

ADLMIDI_EXPORT void adl_close(ADL_MIDIPlayer *device)
{
    if(device->adl_midiPlayer)
        delete reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    device->adl_midiPlayer = NULL;
    free(device);
    device = NULL;
}

ADLMIDI_EXPORT void adl_reset(ADL_MIDIPlayer *device)
{
    if(!device)
        return;
    MIDIplay *play = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    play->m_setup.stored_samples = 0;
    play->m_setup.backup_samples_size = 0;
    play->opl.Reset(play->m_setup.PCM_RATE);
}

ADLMIDI_EXPORT double adl_totalTimeLength(ADL_MIDIPlayer *device)
{
    if(!device)
        return -1.0;
    return reinterpret_cast<MIDIplay *>(device->adl_midiPlayer)->timeLength();
}

ADLMIDI_EXPORT double adl_positionTell(struct ADL_MIDIPlayer *device)
{
    if(!device)
        return -1.0;
    return reinterpret_cast<MIDIplay *>(device->adl_midiPlayer)->tell();
}

ADLMIDI_EXPORT void adl_positionSeek(struct ADL_MIDIPlayer *device, double seconds)
{
    if(!device)
        return;
    reinterpret_cast<MIDIplay *>(device->adl_midiPlayer)->seek(seconds);
}

ADLMIDI_EXPORT void adl_positionRewind(struct ADL_MIDIPlayer *device)
{
    if(!device)
        return;
    reinterpret_cast<MIDIplay *>(device->adl_midiPlayer)->rewind();
}



#ifdef ADLMIDI_USE_DOSBOX_OPL

#define ADLMIDI_CLAMP(V, MIN, MAX) std::max(std::min(V, (MAX)), (MIN))

inline static void SendStereoAudio(MIDIplay::Setup &device,
                                   int      &samples_requested,
                                   ssize_t  &in_size,
                                   int      *_in,
                                   ssize_t  out_pos,
                                   short    *_out)
{
    if(!in_size) return;

    device.stored_samples = 0;
    ssize_t out;
    ssize_t offset;
    ssize_t pos = static_cast<ssize_t>(out_pos);

    for(ssize_t p = 0; p < in_size; ++p)
    {
        for(ssize_t w = 0; w < 2; ++w)
        {
            out    = _in[p * 2 + w];
            offset = pos + p * 2 + w;

            if(offset < samples_requested)
                _out[offset] = static_cast<short>(ADLMIDI_CLAMP(out, static_cast<ssize_t>(INT16_MIN), static_cast<ssize_t>(INT16_MAX)));
            else
            {
                device.backup_samples[device.backup_samples_size] = static_cast<short>(out);
                device.backup_samples_size++;
                device.stored_samples++;
            }
        }
    }
}
#else
inline static void SendStereoAudio(MIDIplay::Setup &device,
                                   int      &samples_requested,
                                   ssize_t  &in_size,
                                   short    *_in,
                                   ssize_t   out_pos,
                                   short    *_out)
{
    if(!in_size)
        return;

    device.stored_samples = 0;
    size_t offset       = static_cast<size_t>(out_pos);
    size_t inSamples    = static_cast<size_t>(in_size * 2);
    size_t maxSamples   = static_cast<size_t>(samples_requested) - offset;
    size_t toCopy       = std::min(maxSamples, inSamples);
    std::memcpy(_out + out_pos, _in, toCopy * sizeof(short));

    if(maxSamples < inSamples)
    {
        size_t appendSize = inSamples - maxSamples;
        std::memcpy(device.backup_samples + device.backup_samples_size,
                    maxSamples + _in, appendSize * sizeof(short));
        device.backup_samples_size += (ssize_t)appendSize;
        device.stored_samples += (ssize_t)appendSize;
    }
}
#endif


ADLMIDI_EXPORT int adl_play(ADL_MIDIPlayer *device, int sampleCount, short *out)
{
    if(!device)
        return 0;

    MIDIplay *player = reinterpret_cast<MIDIplay *>(device->adl_midiPlayer);
    MIDIplay::Setup &setup = player->m_setup;

    sampleCount -= sampleCount % 2; //Avoid even sample requests

    if(sampleCount < 0)
        return 0;

    ssize_t gotten_len = 0;
    ssize_t n_periodCountStereo = 512;
    //ssize_t n_periodCountPhys = n_periodCountStereo * 2;
    int left = sampleCount;

    while(left > 0)
    {
        if(setup.backup_samples_size > 0)
        {
            //Send reserved samples if exist
            ssize_t ate = 0;

            while((ate < setup.backup_samples_size) && (ate < left))
            {
                out[ate] = setup.backup_samples[ate];
                ate++;
            }

            left -= (int)ate;
            gotten_len += ate;

            if(ate < setup.backup_samples_size)
            {
                for(ssize_t j = 0; j < ate; j++)
                    setup.backup_samples[(ate - 1) - j] = setup.backup_samples[(setup.backup_samples_size - 1) - j];
            }

            setup.backup_samples_size -= ate;
        }
        else
        {
            const double eat_delay = setup.delay < setup.maxdelay ? setup.delay : setup.maxdelay;
            setup.delay -= eat_delay;
            setup.carry += setup.PCM_RATE * eat_delay;
            n_periodCountStereo = static_cast<ssize_t>(setup.carry);
            //n_periodCountPhys = n_periodCountStereo * 2;
            setup.carry -= n_periodCountStereo;

            if(setup.SkipForward > 0)
                setup.SkipForward -= 1;
            else
            {
                #ifdef ADLMIDI_USE_DOSBOX_OPL
                std::vector<int>     out_buf;
                #else
                std::vector<int16_t> out_buf;
                #endif
                out_buf.resize(1024, 0);

                if((player->atEnd) && (setup.delay <= 0.0))
                    break;//Stop to fetch samples at reaching the song end with disabled loop

                ssize_t in_generatedStereo = (n_periodCountStereo > 512) ? 512 : n_periodCountStereo;
                ssize_t in_generatedPhys = in_generatedStereo * 2;

                //fill buffer with zeros
                size_t in_countStereoU = static_cast<size_t>(in_generatedStereo * 2);

                if(player->m_setup.NumCards == 1)
                {
                    #ifdef ADLMIDI_USE_DOSBOX_OPL
                    player->opl.cards[0].GenerateArr(out_buf.data(), &in_generatedStereo);
                    in_generatedPhys = in_generatedStereo * 2;
                    #else
                    OPL3_GenerateStream(&player->opl.cards[0], out_buf.data(), static_cast<Bit32u>(in_generatedStereo));
                    #endif
                    /* Process it */
                    SendStereoAudio(setup, sampleCount, in_generatedStereo, out_buf.data(), gotten_len, out);
                }
                else if(n_periodCountStereo > 0)
                {
                    #ifdef ADLMIDI_USE_DOSBOX_OPL
                    std::vector<int32_t> in_mixBuffer;
                    in_mixBuffer.resize(1024); //n_samples * 2
                    ssize_t in_generatedStereo = n_periodCountStereo;
                    #endif
                    std::memset(out_buf.data(), 0, in_countStereoU * sizeof(short));

                    /* Generate data from every chip and mix result */
                    for(unsigned card = 0; card < player->m_setup.NumCards; ++card)
                    {
                        #ifdef ADLMIDI_USE_DOSBOX_OPL
                        player->opl.cards[card].GenerateArr(in_mixBuffer.data(), &in_generatedStereo);
                        in_generatedPhys = in_generatedStereo * 2;
                        size_t in_samplesCount = static_cast<size_t>(in_generatedPhys);
                        for(size_t a = 0; a < in_samplesCount; ++a)
                            out_buf[a] += in_mixBuffer[a];
                        #else
                        OPL3_GenerateStreamMix(&player->opl.cards[card], out_buf.data(), static_cast<Bit32u>(in_generatedStereo));
                        #endif
                    }

                    /* Process it */
                    SendStereoAudio(setup, sampleCount, in_generatedStereo, out_buf.data(), gotten_len, out);
                }

                left -= (int)in_generatedPhys;
                gotten_len += (in_generatedPhys) - setup.stored_samples;
                out_buf.clear();
            }

            setup.delay = player->Tick(eat_delay, setup.mindelay);
        }
    }

    return static_cast<int>(gotten_len);
}
