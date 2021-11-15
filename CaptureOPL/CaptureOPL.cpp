#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "OPBinaryLib\opblib.h"
#include "..\include\adlmidi.h"
#include "..\src\chips\capture_opl3.hpp"

static bool Silenced = false;
static ADL_MIDIPlayer* MidiPlayer = NULL;

#define CAPTUREOPL_LOG(...) \
    if (!Silenced) std::cout << __VA_ARGS__

static std::vector<OPB_Command> CommandStream;

void writeRegCB(RegWriteCmd* commands, size_t count) {
    double time = adl_positionTell(MidiPlayer);

    for (int i = 0; i < count; i++) {
        RegWriteCmd cmd = *(commands + i);
        CommandStream.push_back({ cmd.addr, cmd.data, time });
    }
}

void InitCapture() {
    captureOPL3_setCallback(&writeRegCB);
    CommandStream.clear();
}

std::string RemoveExtension(const std::string& path) {
    if (path == "." || path == "..")
        return path;

    size_t pos = path.find_last_of("\\/.");
    if (pos != std::string::npos && path[pos] == '.')
        return path.substr(0, pos);

    return path;
}

std::string GetFilename(const std::string& path) {
    if (path == "." || path == "..")
        return path;

    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
        return path.substr(pos + 1, path.size());

    return path;
}

void OPB_LogImpl(const char* s) {
    if (!Silenced) {
        printf(s);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << GetFilename(argv[0]) << " <inmidi> [outfile] [-b (bank|woplfile)] [-s]\n";
        std::cout << "  inmidi      Midi file to convert to OPB format\n";
        std::cout << "  outfile     OPB target output filename\n";
        std::cout << "\n";
        std::cout << "Options:\n";
        std::cout << "  -b (bank|woplfile)\n";
        std::cout << "    Use embedded or external soundbank:\n";
        std::cout << "    woplfile    External soundbank (WOPL format)\n";
        std::cout << "    bank        Internal soundbank index\n";
        std::cout << "  -s\n";
        std::cout << "    Silence output\n";
        std::cout << "\n";
        std::cout << "Note: To create WOPL bank files use OPL Bank Editor you can get here:\n";
        std::cout << "https://github.com/Wohlstand/OPL3BankEditor\n";
        std::cout << "\n";
        std::cout << "Available embedded banks by number:\n";
        std::cout << "   0 = AIL (Star Control 3, Albion, Empire 2, etc.)\n";
        std::cout << "   1 = Bisqwit (selection of 4op and 2op)\n";
        std::cout << "   2 = HMI (Descent, Asterix)\n";
        std::cout << "   3 = HMI (Descent:: Int)\n";
        std::cout << "   4 = HMI (Descent:: Ham)\n";
        std::cout << "   5 = HMI (Descent:: Rick)\n";
        std::cout << "   6 = HMI (Descent 2)\n";
        std::cout << "   7 = HMI (Normality)\n";
        std::cout << "   8 = HMI (Shattered Steel)\n";
        std::cout << "   9 = HMI (Theme Park)\n";
        std::cout << "  10 = HMI (3d Table Sports, Battle Arena Toshinden)\n";
        std::cout << "  11 = HMI (Aces of the Deep)\n";
        std::cout << "  12 = HMI (Earthsiege)\n";
        std::cout << "  13 = HMI (Anvil of Dawn)\n";
        std::cout << "  14 = DMX (Doom 2)\n";
        std::cout << "  15 = DMX (Hexen, Heretic)\n";
        std::cout << "  16 = DMX (DOOM, MUS Play)\n";
        std::cout << "  17 = AIL (Discworld, Grandest Fleet, etc.)\n";
        std::cout << "  18 = AIL (Warcraft 2)\n";
        std::cout << "  19 = AIL (Syndicate)\n";
        std::cout << "  20 = AIL (Guilty, Orion Conspiracy, TNSFC ::4op)\n";
        std::cout << "  21 = AIL (Magic Carpet 2) :NON-GM:\n";
        std::cout << "  22 = AIL (Nemesis)\n";
        std::cout << "  23 = AIL (Jagged Alliance) :NON-GM:\n";
        std::cout << "  24 = AIL (When Two Worlds War) :MISS-INS:\n";
        std::cout << "  25 = AIL (Bards Tale Construction) :MISS-INS:\n";
        std::cout << "  26 = AIL (Return to Zork) :NON-GM:\n";
        std::cout << "  27 = AIL (Theme Hospital)\n";
        std::cout << "  28 = AIL (National Hockey League PA)\n";
        std::cout << "  29 = AIL (Inherit The Earth) :NON-GM:\n";
        std::cout << "  30 = AIL (Inherit The Earth, file two) :NON-GM:\n";
        std::cout << "  31 = AIL (Little Big Adventure) :4op:\n";
        std::cout << "  32 = AIL (Wreckin Crew) :NON-GM:\n";
        std::cout << "  33 = AIL (Death Gate)\n";
        std::cout << "  34 = AIL (FIFA International Soccer)\n";
        std::cout << "  35 = AIL (Starship Invasion)\n";
        std::cout << "  36 = AIL (Super Street Fighter 2 :4op:)\n";
        std::cout << "  37 = AIL (Lords of the Realm) :MISS-INS:\n";
        std::cout << "  38 = AIL (SimFarm, SimHealth) :4op:\n";
        std::cout << "  39 = AIL (SimFarm, Settlers, Serf City)\n";
        std::cout << "  40 = AIL (Caesar 2) :p4op: :MISS-INS:\n";
        std::cout << "  41 = AIL (Syndicate Wars)\n";
        std::cout << "  42 = AIL (Bubble Bobble Feat. Rainbow Islands, Z)\n";
        std::cout << "  43 = AIL (Warcraft) :NON-GM:\n";
        std::cout << "  44 = AIL (Terra Nova Strike Force Centuri) :p4op:\n";
        std::cout << "  45 = AIL (System Shock) :p4op:\n";
        std::cout << "  46 = AIL (Advanced Civilization)\n";
        std::cout << "  47 = AIL (Battle Chess 4000) :p4op: :NON-GM:\n";
        std::cout << "  48 = AIL (Ultimate Soccer Manager :p4op:)\n";
        std::cout << "  49 = AIL (Air Bucks, Blue And The Gray, etc) :NON-GM:\n";
        std::cout << "  50 = AIL (Ultima Underworld 2) :NON-GM:\n";
        std::cout << "  51 = AIL (Kasparov's Gambit) :NON-GM:\n";
        std::cout << "  52 = AIL (High Seas Trader) :MISS-INS:\n";
        std::cout << "  53 = AIL (Master of Magic) :4op:\n";
        std::cout << "  54 = AIL (Master of Magic) :4op: orchestral drums\n";
        std::cout << "  55 = SB (Action Soccer)\n";
        std::cout << "  56 = SB (3d Cyberpuck :: melodic only)\n";
        std::cout << "  57 = SB (Simon the Sorcerer :: melodic only)\n";
        std::cout << "  58 = OP3 (The Fat Man 2op set; Win9x)\n";
        std::cout << "  59 = OP3 (The Fat Man 4op set)\n";
        std::cout << "  60 = OP3 (JungleVision 2op set :: melodic only)\n";
        std::cout << "  61 = OP3 (Wallace 2op set, Nitemare 3D :: melodic only)\n";
        std::cout << "  62 = TMB (Duke Nukem 3D)\n";
        std::cout << "  63 = TMB (Shadow Warrior)\n";
        std::cout << "  64 = DMX (Raptor)\n";
        std::cout << "  65 = SB (Modded GMOPL by Wohlstand)\n";
        std::cout << "  66 = SB (Jamie O'Connell's bank)\n";
        std::cout << "  67 = TMB (Apogee Sound System Default bank) :broken drums:\n";
        std::cout << "  68 = WOPL (4op bank by James Alan Nguyen and Wohlstand)\n";
        std::cout << "  69 = TMB (Blood)\n";
        std::cout << "  70 = TMB (Rise of the Triad)\n";
        std::cout << "  71 = TMB (Nam)\n";
        std::cout << "  72 = WOPL (DMXOPL3 bank by Sneakernets)\n";
        std::cout << "  73 = EA (Cartooners)\n";
        std::cout << "  74 = WOPL (Apogee IMF 90-ish)\n";
        std::cout << "  75 = The Lost Vikings\n";
        std::cout << "  76 = DMX (Strife)\n";
        std::cout << "  77 = WOPL (MS-AdLib, Windows 3.x)\n";
        exit(1);
    }

    std::string bankStr = "";

    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-b" && i + 1 < argc) {
            i++;
            bankStr = argv[i];
        }
        else if (arg == "-s") {
            Silenced = true;
        }
    }

    CAPTUREOPL_LOG("Starting " << GetFilename(argv[0]) << "\n");

    std::string inPath = argv[1];
    CAPTUREOPL_LOG("Input path: " << inPath << "\n");

    std::string outPath = RemoveExtension(inPath) + ".opb";

    if (inPath == outPath) {
        outPath = inPath + ".opb";
    }

    if (argc >= 3) {
        std::string tmp = argv[2];
        if (tmp.front() != '-') {
            outPath = tmp;
            if (inPath == outPath) {
                CAPTUREOPL_LOG("Input and outpath path cannot be the same\n");
                exit(1);
            }
        }
    }

    CAPTUREOPL_LOG("Output path: " << outPath << "\n");

    std::string woplPath = "";
    int embeddedIndex;

    if (!bankStr.empty()) {
        try {
            embeddedIndex = std::stoi(bankStr);
        }
        catch (...) {
            embeddedIndex = -1;
            woplPath = bankStr;
        }
    }
    else {
        embeddedIndex = -1;
    }

    CAPTUREOPL_LOG("Initializing libADLMIDI player\n");
    MidiPlayer = adl_init(44100);

    if (!MidiPlayer) {
        CAPTUREOPL_LOG("Couldn't initialize ADLMIDI: " << adl_errorString() << "\n");
        exit(1);
    }

    CAPTUREOPL_LOG("Setting number of emulated chips to 1\n");
    if (adl_setNumChips(MidiPlayer, 1)) {
        CAPTUREOPL_LOG("Couldn't set number of chips to 1: " << adl_errorString() << "\n");
        adl_close(MidiPlayer);
        exit(1);
    }

    CAPTUREOPL_LOG("Switching to capture chip\n");
    if (adl_switchEmulator(MidiPlayer, ADLMIDI_EMU_CAPTURE)) {
        CAPTUREOPL_LOG("Couldn't switch to capture chip: " << adl_errorString() << "\n");
        adl_close(MidiPlayer);
        exit(1);
    }

    if (embeddedIndex >= 0) {
        CAPTUREOPL_LOG("Using embedded bank " << embeddedIndex << "\n");
        if (adl_setBank(MidiPlayer, embeddedIndex)) {
            CAPTUREOPL_LOG("Couldn't open embedded bank " << embeddedIndex << ": " << adl_errorString() << "\n");
            adl_close(MidiPlayer);
            exit(1);
        }
    }
    else if (!woplPath.empty()) {
        CAPTUREOPL_LOG("Using external bank '" << woplPath << "'\n");
        if (adl_openBankFile(MidiPlayer, woplPath.c_str())) {
            CAPTUREOPL_LOG("Couldn't open bank file '" << woplPath << "': " << adl_errorString() << "\n");
            adl_close(MidiPlayer);
            exit(1);
        }
    }

    CAPTUREOPL_LOG("Initializing capture\n");
    InitCapture();
    adl_reset(MidiPlayer);

    CAPTUREOPL_LOG("Opening '" << inPath << "' for input\n");
    if (adl_openFile(MidiPlayer, inPath.c_str()) < 0) {
        CAPTUREOPL_LOG("Couldn't open midi file '" << inPath << "': " << adl_errorString() << "\n");
        adl_close(MidiPlayer);
        exit(1);
    }

    CAPTUREOPL_LOG("Recording OPL output\n");
    short buffer[4096];
    int samples_count = 0;
    do {
        samples_count = adl_play(MidiPlayer, 4096, buffer);
        if (samples_count > 0) {
            /* do something with the generated chunk */
        }
    } while (samples_count > 0);

    adl_close(MidiPlayer);

    // set OPBinaryLib logging function
    OPB_Log = OPB_LogImpl;

    if (OPB_OplToFile(OPB_Format_Default, CommandStream.data(), CommandStream.size(), outPath.c_str())) {
        CAPTUREOPL_LOG("Some errors occurred while converting to OPB\n");
    }
    else {
        CAPTUREOPL_LOG("Done!\n");
    }
}
