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
/*
midi_override_base::midi_override_base()
{
	pMidiIn  = NULL;
	pMidiOut = NULL;
	pBypass  = NULL;

	for(int i = 0; i < 16; i++)
		{
			pProgram[i]             = NULL;
			pProgramBypass[i]       = NULL;
			pVelocity[i]            = NULL;
			pVelocityBypass[i]      = NULL;
			pVelocityClamp[i]       = NULL;
	 		pVelocityClampBypass[i] = NULL;
			pAmp[i]                 = NULL;
			pAmpBC[i]               = NULL;
		}
}
*/
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

	for(int i = 0; i < 16; i++)
	{
		lsp_trace("Channel %02d", i + 1);

		// Program Number Settings
		lsp_trace("Program = %d", nProgram[i]);
		lsp_trace("Program Bypass = %s", (bProgramB[i]) ? "true" : "false");

		// Velocity Settings
		lsp_trace("Velocity = %d", nVelocity[i]);
		lsp_trace("Velocity Bypass = %s", (bVelocityB[i]) ? "true" : "false");
		lsp_trace("Velocity Clamp = %d", nVelocityClamp[i]);
		lsp_trace("Velocity Clamp Enabled = %s", (bVelocityClampB[i]) ? "true" : "false");
		lsp_trace("Amplify = %d", nAmp[i]);
		lsp_trace("Amp Before Clamp = %s", (bAmpBC[i]) ? "true" : "false");
	}
}

void midi_override_base::process(size_t samples)
{
	// Get the MIDI Input and Output:
	midi_t *in  = (pMidiIn != NULL) ? reinterpret_cast<midi_t *>(pMidiIn->getBuffer()) : NULL;
	midi_t *out = (pMidiOut != NULL) ? reinterpret_cast<midi_t *>(pMidiOut->getBuffer()) : NULL;

	if(in == NULL)
		return;

	// Does not compile!

	for(size_t i=0; i < in->nEvents; i++)
	{
		const midi_event_t *me = &in->vEvents[i];
		const uint8_t current_channel = me->channel;
		midi_event_t ev = *me;

		if(ev.type == MIDI_MSG_NOTE_ON)
		{
			if(!bProgramB[current_channel])
			{
				// Edit Program Number
				ev.program = nProgram[current_channel];
			}

			if(!bVelocityB[current_channel])
			{
				// Edit Velocity
				ev.note.velocity = nVelocity[current_channel];
			}

			if(bVelocityClampB[current_channel])
			{
				// Clamp Velocity (Check for Amp before!)
				if(bAmpBC[current_channel])
				{
					ev.note.velocity += nAmp[current_channel];
				}
				if(ev.note.velocity > nVelocityClamp[current_channel])
				{
					ev.note.velocity = nVelocityClamp[current_channel];
				}
			}

			if(!bAmpBC[ev.channel])
			{
				// Amp post clamp
				if(((ev.note.velocity + nAmp[i]) <= 255) && ((ev.note.velocity + nAmp[i]) >= 0))
				{
					ev.note.velocity += nAmp[i];
				}
				else if((ev.note.velocity + nAmp[i]) > 255)
				{
					ev.note.velocity = 255;
				}
				else if((ev.note.velocity + nAmp[i]) < 0)
				{
					ev.note.velocity = 0;
				}
			}
		}
		// Push all changes to MIDI Out:
		out->push(ev);
	}
}

void midi_override_base::destroy()
{
	for(int i = 0; i < 16; i++)
	{
		pProgram[i]             = NULL;
		pProgramBypass[i]       = NULL;
		pVelocity[i]            = NULL;
		pVelocityBypass[i]      = NULL;
		pVelocityClamp[i]       = NULL;
 		pVelocityClampBypass[i] = NULL;
		pAmp[i]                 = NULL;
		pAmpBC[i]               = NULL;
	}
	pMidiIn  = NULL;
	pMidiOut = NULL;
	pBypass  = NULL;
}
