#include <metadata/plugins.h>
#include <metadata/ports.h>
#include <metadata/developers.h>
#include <string>

using namespace lsp;

// MIDI Channel override controls macro:
#define MO_CONTROLS(ch)\
	INT_CONTROL("p" + std::to_string(ch), "Program", U_NONE, midi_edit_metadata::PROGRAM),\
	SWITCH("pb" + std::to_string(ch), "Bypass", 1),\
	INT_CONTROL("v" + std::to_string(ch), "Velocity", U_NONE, midi_edit_metadata::VELOCITY),\
	SWITCH("vb" + std::to_string(ch), "Bypass", 1),\
	INT_CONTROL("c" + std::to_string(ch), "Clamp Velocity", U_NONE, midi_edit_metadata::CLAMP),\
	SWITCH("cb" + std::to_string(ch), "Enabled", 0),\
	INT_CONTROL("a" + std::to_string(ch), "Amplify", U_NONE, midi_edit_metadata::AMP),\
	SWITCH("ab" + std::to_string(ch), "Before Clamp", 0)

static const port_t midi_edit_ports[] =
{
    MIDI_INPUT,
	MIDI_OUTPUT,
	BYPASS,
	MO_CONTROLS(1),
	MO_CONTROLS(2),
	MO_CONTROLS(3),
	MO_CONTROLS(4),
	MO_CONTROLS(5),
	MO_CONTROLS(6),
	MO_CONTROLS(7),
	MO_CONTROLS(8),
	MO_CONTROLS(9),
	MO_CONTROLS(10),
	MO_CONTROLS(11),
	MO_CONTROLS(12),
	MO_CONTROLS(13),
	MO_CONTROLS(14),
	MO_CONTROLS(15),
	MO_CONTROLS(16),
	PORTS_END
};

#undef MO_CONTROLS

const plugin_metadata_t midi_edit_metadata::metadata =
{
		"MIDI Override",
		"Override parts of a MIDI stream",
		"MO",
		&developers::e_snyder,
		"midi_override",
		"", // FIXME
		0, // FIXME
		LSP_VERSION(1, 0, 0),
		/* Classes Go Here*/, // FIXME
		midi_edit_ports,
		"midi/midi_override.xml",
		NULL
};
