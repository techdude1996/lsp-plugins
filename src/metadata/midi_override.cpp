#include <metadata/plugins.h>
#include <metadata/ports.h>
#include <metadata/developers.h>
#include <metadata/metadata.h>
#include <metadata/midi_override.h>
#include <string>

using namespace lsp;

// MIDI Channel override controls macro:
#define MO_CONTROLS(ch)\
	INT_CONTROL("p" # ch, "Program", U_NONE, midi_override_metadata::PROGRAM),\
	SWITCH("pb" # ch, "Bypass", 1),\
	INT_CONTROL("v" # ch, "Velocity", U_NONE, midi_override_metadata::VELOCITY),\
	SWITCH("vb" # ch, "Bypass", 1),\
	INT_CONTROL("c" # ch, "Clamp Velocity", U_NONE, midi_override_metadata::CLAMP),\
	SWITCH("cb" # ch, "Enabled", 0),\
	INT_CONTROL("a" # ch, "Amplify", U_NONE, midi_override_metadata::AMP),\
	SWITCH("ab" # ch, "Before Clamp", 0)

static const port_t midi_edit_ports[] =
{
    MIDI_INPUT,
	MIDI_OUTPUT,
	BYPASS,
	MO_CONTROLS("1"),
	MO_CONTROLS("2"),
	MO_CONTROLS("3"),
	MO_CONTROLS("4"),
	MO_CONTROLS("5"),
	MO_CONTROLS("6"),
	MO_CONTROLS("7"),
	MO_CONTROLS("8"),
	MO_CONTROLS("9"),
	MO_CONTROLS("10"),
	MO_CONTROLS("11"),
	MO_CONTROLS("12"),
	MO_CONTROLS("13"),
	MO_CONTROLS("14"),
	MO_CONTROLS("15"),
	MO_CONTROLS("16"),
	PORTS_END
};

#undef MO_CONTROLS

const plugin_metadata_t midi_override_metadata::metadata =
{
		"MIDI Override",
		"Override parts of a MIDI stream",
		"MO",
		&developers::e_snyder,
		"midi_override",
		"", // FIXME
		0, // FIXME
		LSP_VERSION(1, 0, 0),
		NULL, // FIXME
		midi_edit_ports,
		"midi/midi_override.xml",
		NULL
};
