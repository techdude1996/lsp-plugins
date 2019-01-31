#ifndef CORE_PLUGINS_MIDI_EDIT_H_
#define CORE_PLUGINS_MIDI_EDIT_H_

#include <core/plugin.h>
#include <core/midi.h>
#include <core/util/Bypass.h>
#include <metadata/plugins.h>
#include <metadata/midi_override.h>

using namespace lsp;

class midi_override_base: public plugin_t, public midi_override_metadata
{
	protected:
		Bypass vBypass;
    public:
		         midi_override_base();
		virtual ~midi_override_base();
        void     init(IWrapper *wrapper);
		//void     update_sample_rate(long sr);
        void     destroy();
		//void     activated();
		//void     ui_activated();
		void     update_settings();
		//bool     set_position(const position_t *pos);
        void     process(size_t samples);
		//bool     inline_display(ICanvas *cv, size_t width, size_t height);
		//void     ui_deactivated();
		//void     deactivated();
		//void     configure();
	protected:
		// Ports
		IPort    *pMidiIn;          // MIDI in
		IPort    *pMidiOut;         // MIDI out
		IPort    *pBypass;          // Master plugin bypass switch

		// MIDI Override controls:
		IPort    *pProgram[16];
		IPort    *pProgramBypass[16];
		IPort    *pVelocity[16];
		IPort    *pVelocityBypass[16];
		IPort    *pVelocityClamp[16];
		IPort    *pVelocityClampBypass[16];
		IPort    *pAmp[16];
		IPort    *pAmpBC[16];

		// MIDI Override values:
		int      nProgram[16];
		int      nVelocity[16];
		int      nVelocityClamp[16];
		int      nAmp[16];
		bool     bProgramB[16];
		bool     bVelocityB[16];
		bool     bVelocityClampB[16];
		bool     bAmpBC[16];

		// Input MIDI values:

};

#endif /* CORE_PLUGINS_MIDI_EDIT_H_ */
