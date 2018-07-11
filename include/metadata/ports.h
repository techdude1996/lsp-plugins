/*
 * ports.h
 *
 *  Created on: 07 апр. 2016 г.
 *      Author: sadko
 */

#ifndef METADATA_PORTS_H_
#define METADATA_PORTS_H_

// Port definitions for metadata
#define AUDIO_INPUT(id, label) \
    { id, label, U_NONE, R_AUDIO, F_IN, 0, 0, 0, 0, NULL, NULL    }
#define AUDIO_OUTPUT(id, label) \
    { id, label, U_NONE, R_AUDIO, F_OUT, 0, 0, 0, 0, NULL, NULL    }
#define MIDI_CHANNEL(id, direction, label) \
    { id, label, U_NONE, R_MIDI, direction, 0, 0, 0, 0, NULL, NULL    }
#define FILE_CHANNEL(id, label) \
    { id, label, U_ENUM, R_CONTROL, F_IN | F_INT, 0, 0, 0, 0, file_channels, NULL }
#define AMP_GAIN(id, label, dfl, max) \
    { id, label, U_GAIN_AMP, R_CONTROL, F_IN | F_LOG | F_UPPER | F_LOWER | F_STEP, 0, max, dfl, 0.1, NULL, NULL }
#define AMP_GAIN10(id, label, dfl)  AMP_GAIN(id, label, dfl, 10.0f)
#define STATUS(id, label) \
    { id, label, U_NONE, R_METER, F_OUT | F_INT | F_UPPER | F_LOWER, 0, STATUS_MAX, STATUS_UNSPECIFIED, 0, NULL, NULL }
#define MESH(id, label, dim, points) \
    { id, label, U_NONE, R_MESH, F_OUT, 0.0, 0.0, points, dim, NULL, NULL }
#define PATH(id, label) \
    { id, label, U_STRING, R_PATH, F_IN, 0, 0, 0, 0, NULL, NULL }
#define TRIGGER(id, label)  \
    { id, label, U_BOOL, R_CONTROL, F_IN | F_TRG, 0, 0, 0.0f, 0, NULL, NULL }
#define SWITCH(id, label, dfl)  \
    { id, label, U_BOOL, R_CONTROL, F_IN, 0, 0, dfl, 0, NULL, NULL }
#define COMBO(id, label, dfl, list) \
    { id, label, U_ENUM, R_CONTROL, F_IN, 0, 0, dfl, 0, list, NULL }
#define BLINK(id, label) \
    { id, label, U_BOOL, R_METER, F_OUT, 0, 0, 0, 0, NULL, NULL }
#define CONTROL(id, label, units, limits) \
    { id, label, units, R_CONTROL, F_IN | F_LOWER | F_UPPER | F_STEP, \
        limits ## _MIN, limits ## _MAX, limits ## _DFL, limits ## _STEP, NULL, NULL }
#define PORT_SET(id, label, keys, ports)  \
    { id, label, U_ENUM, R_PORT_SET, F_IN, 0, 0, 0, 0, keys, ports }
#define PAN_CTL(id, label, dfl) \
    { id, label, U_PERCENT, R_CONTROL, F_IN | F_LOWER | F_UPPER | F_STEP, -100.0f, 100.0f, dfl, 0.1, NULL, NULL }

#define PORTS_END   \
    { NULL, NULL }

// Reduced ports
#define AUDIO_INPUT_MONO    AUDIO_INPUT("in", "Input")
#define AUDIO_INPUT_LEFT    AUDIO_INPUT(PORT_NAME_INPUT_L, "Input L")
#define AUDIO_INPUT_RIGHT   AUDIO_INPUT(PORT_NAME_INPUT_R, "Input R")
#define AUDIO_INPUT_A       AUDIO_INPUT("in_a", "Input A")
#define AUDIO_INPUT_B       AUDIO_INPUT("in_b", "Input B")
#define AUDIO_INPUT_N(n)    AUDIO_INPUT("in" #n, "Input " #n)

#define AUDIO_OUTPUT_MONO   AUDIO_OUTPUT("out", "Output")
#define AUDIO_OUTPUT_LEFT   AUDIO_OUTPUT("out_l", "Output L")
#define AUDIO_OUTPUT_RIGHT  AUDIO_OUTPUT("out_r", "Output R")
#define AUDIO_OUTPUT_A      AUDIO_OUTPUT("out_a", "Output A")
#define AUDIO_OUTPUT_B      AUDIO_OUTPUT("out_b", "Output B")
#define AUDIO_OUTPUT_N(n)   AUDIO_OUTPUT("out" #n, "Output " #n)

#define MIDI_INPUT          MIDI_CHANNEL(LSP_LV2_MIDI_PORT_IN, F_IN, "Midi input")
#define MIDI_OUTPUT         MIDI_CHANNEL(LSP_LV2_MIDI_PORT_OUT, F_OUT, "Midi output")

#define OUT_GAIN            AMP_GAIN10("g_out", "Output gain", 1.0f)

#define DRY_GAIN(g)         AMP_GAIN10("dry", "Dry amount", g)
#define DRY_GAIN_L(g)       AMP_GAIN10("dry_l", "Dry amount L", g)
#define DRY_GAIN_R(g)       AMP_GAIN10("dry_r", "Dry amount R", g)

#define WET_GAIN(g)         AMP_GAIN10("wet", "Wet amount", g)
#define WET_GAIN_L(g)       AMP_GAIN10("wet_l", "Wet amount L", g)
#define WET_GAIN_R(g)       AMP_GAIN10("wet_r", "Wet amount R", g)

#define BYPASS              SWITCH("bypass", "Bypass", 0.0f)


// Port configurations
#define PORTS_MONO_PLUGIN   \
    AUDIO_INPUT_MONO,       \
    AUDIO_OUTPUT_MONO       \

#define PORTS_STEREO_PLUGIN \
    AUDIO_INPUT_LEFT,       \
    AUDIO_INPUT_RIGHT,      \
    AUDIO_OUTPUT_LEFT,      \
    AUDIO_OUTPUT_RIGHT

#define PORTS_MIDI_CHANNEL  \
    MIDI_INPUT,             \
    MIDI_OUTPUT

// Port groups
#define STEREO_PORT_GROUP_PORTS(id, a, b) \
    static const port_group_item_t id ## _ports[] = \
    { \
        { a, PGR_LEFT  }, \
        { b, PGR_RIGHT }, \
        { NULL } \
    }

namespace lsp
{
    // Common stereo port names
    extern const char PORT_NAME_INPUT_L[];
    extern const char PORT_NAME_INPUT_R[];
    extern const char PORT_NAME_OUTPUT_L[];
    extern const char PORT_NAME_OUTPUT_R[];

    // Port groups
    extern const port_group_item_t stereo_in_group_ports[];
    extern const port_group_item_t stereo_out_group_ports[];

    extern const port_group_t stereo_plugin_port_groups[];

    // Miscellaneous lists
    extern const char *file_channels[];

    extern const char *midi_channels[];

    extern const char *octaves[];

    extern const char *notes[];
}


#endif /* METADATA_PORTS_H_ */