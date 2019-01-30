/*************************************************
 * MIDI Override                                 *
 * Created by: Ethan Snyder                      *
 * Created on: 11 November, 2018                 *
 *************************************************/

#include <dsp/dsp.h>
#include <core/debug.h>
#include <core/midi.h>
#include <plugins/midi_override.h>
#include <cmath>
#include <string>

// Macros
#define TRACE_PORT(p) lsp_trace("  port id=%s", (p)->metadata()->id);

using namespace lsp;

midi_override_base::~midi_override_base()
{
	 destroy();
}

void midi_override_base::init(IWrapper *wrapper)
{
	// Pass wrapper
	plugin_t::init(wrapper);

	// Bind Ports:
	size_t port_id = 0;
	TRACE_PORT(vPorts[port_id]);
	pMidiIn = vPorts[port_id++];

	TRACE_PORT(vPorts[port_id]);
	pMidiOut = vPorts[port_id++];

	TRACE_PORT(vPorts[port_id]);
	pBypass = vPorts[port_id++];

	// Loop through all controls for the 16 MIDI Channels
	for(int i = 0; i < 16; i++)
	{
		TRACE_PORT(vPorts[port_id]);
		pProgram[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
		pProgramBypass[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
		pVelocity[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
		pVelocityBypass[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
		pVelocityClamp[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
 		pVelocityClampBypass[i] = vPorts[port_id++];

 		TRACE_PORT(vPorts[port_id]);
		pAmp[i] = vPorts[port_id++];

		TRACE_PORT(vPorts[port_id]);
		pAmpBC[i] = vPorts[port_id++];
	}
}

void midi_override_base::update_settings()
{
	if (pBypass != NULL)
	{
		vBypass.set_bypass(pBypass->getValue() >= 0.5f);
	}

	for(int i = 0; i < 16; i++)
	{
		nProgram[i]        = int(pProgram[i]->getValue());
		nVelocity[i]       = int(pVelocity[i]->getValue());
		nVelocityClamp[i]  = int(pVelocityClamp[i]->getValue());
		nAmp[i]            = int(pAmp[i]->getValue());

		bProgramB[i]       = pProgramBypass[i]->getValue() >= 0.5f;
		bVelocityB[i]      = pVelocityBypass[i]->getValue() >= 0.5f;
		bVelocityClampB[i] = pVelocityClampBypass[i]->getValue() >= 0.5f;
		bAmpBC[i]          = pAmpBC[i]->getValue() >= 0.5f;
	}
}
