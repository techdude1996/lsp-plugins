#ifndef CORE_PLUGINS_MIDI_EDIT_H_
#define CORE_PLUGINS_MIDI_EDIT_H_

#include <core/plugin.h>
#include <core/midi.h>
#include <core/util/bypass.h>
#include <metadata/plugins.h>

using namespace lsp;

class midi_override_base: public plugin_t
{
	protected:
		Bypass vBypass;
    public:
		         midi_override_base();
		virtual ~midi_override_base();
        void     init(IWrapper *wrapper);
		//void     update_sample_rate(long sr);
        void     destroy();
		void     activated();
		void     ui_activated();
		void     update_settings();
		bool     set_position(const position_t *pos);
        void     process(size_t samples);
		bool     inline_display(ICanvas *cv, size_t width, size_t height);
		void     ui_deactivated();
		void     deactivated();
		void     configure();
	protected:
		// Ports
		IPort    *pMidiIn;          // MIDI in
		IPort    *pMidiOut;         // MIDI out
		IPort    *pBypass;          // Master plugin bypass switch

		// MIDI Override controls - canot use macro for these :(
		IPort    *pProgram_01;      // MIDI Channel 01 program number override
		IPort    *pProgram_02;      // MIDI Channel 02 program number override
		IPort    *pProgram_03;      // MIDI Channel 03 program number override
		IPort    *pProgram_04;      // MIDI Channel 04 program number override
		IPort    *pProgram_05;      // MIDI Channel 05 program number override
		IPort    *pProgram_06;      // MIDI Channel 06 program number override
		IPort    *pProgram_07;      // MIDI Channel 07 program number override
		IPort    *pProgram_08;      // MIDI Channel 08 program number override
		IPort    *pProgram_09;      // MIDI Channel 09 program number override
		IPort    *pProgram_10;      // MIDI Channel 10 program number override
		IPort    *pProgram_11;      // MIDI Channel 11 program number override
		IPort    *pProgram_12;      // MIDI Channel 12 program number override
		IPort    *pProgram_13;      // MIDI Channel 13 program number override
		IPort    *pProgram_14;      // MIDI Channel 14 program number override
		IPort    *pProgram_15;      // MIDI Channel 15 program number override
		IPort    *pProgram_16;      // MIDI Channel 16 program number override

		IPort    *pProgramBy_01;    // MIDI Channel 01 program number bypass
		IPort    *pProgramBy_02;    // MIDI Channel 02 program number bypass
		IPort    *pProgramBy_03;    // MIDI Channel 03 program number bypass
		IPort    *pProgramBy_04;    // MIDI Channel 04 program number bypass
		IPort    *pProgramBy_05;    // MIDI Channel 05 program number bypass
		IPort    *pProgramBy_06;    // MIDI Channel 06 program number bypass
		IPort    *pProgramBy_07;    // MIDI Channel 07 program number bypass
		IPort    *pProgramBy_08;    // MIDI Channel 08 program number bypass
		IPort    *pProgramBy_09;    // MIDI Channel 09 program number bypass
		IPort    *pProgramBy_10;    // MIDI Channel 10 program number bypass
		IPort    *pProgramBy_11;    // MIDI Channel 11 program number bypass
		IPort    *pProgramBy_12;    // MIDI Channel 12 program number bypass
		IPort    *pProgramBy_13;    // MIDI Channel 13 program number bypass
		IPort    *pProgramBy_14;    // MIDI Channel 14 program number bypass
		IPort    *pProgramBy_15;    // MIDI Channel 15 program number bypass
		IPort    *pProgramBy_16;    // MIDI Channel 16 program number bypass

		IPort    *pVelocity_01;     // MIDI Channel 01 velocity override
		IPort    *pVelocity_02;     // MIDI Channel 02 velocity override
		IPort    *pVelocity_03;     // MIDI Channel 03 velocity override
		IPort    *pVelocity_04;     // MIDI Channel 04 velocity override
		IPort    *pVelocity_05;     // MIDI Channel 05 velocity override
		IPort    *pVelocity_06;     // MIDI Channel 06 velocity override
		IPort    *pVelocity_07;     // MIDI Channel 07 velocity override
		IPort    *pVelocity_08;     // MIDI Channel 08 velocity override
		IPort    *pVelocity_09;     // MIDI Channel 09 velocity override
		IPort    *pVelocity_10;     // MIDI Channel 10 velocity override
		IPort    *pVelocity_11;     // MIDI Channel 11 velocity override
		IPort    *pVelocity_12;     // MIDI Channel 12 velocity override
		IPort    *pVelocity_13;     // MIDI Channel 13 velocity override
		IPort    *pVelocity_14;     // MIDI Channel 14 velocity override
		IPort    *pVelocity_15;     // MIDI Channel 15 velocity override
		IPort    *pVelocity_16;     // MIDI Channel 16 velocity override

		IPort    *pVelocityBy_01;   // MIDI Channel 01 velocity override bypass
		IPort    *pVelocityBy_02;   // MIDI Channel 02 velocity override bypass
		IPort    *pVelocityBy_03;   // MIDI Channel 03 velocity override bypass
		IPort    *pVelocityBy_04;   // MIDI Channel 04 velocity override bypass
		IPort    *pVelocityBy_05;   // MIDI Channel 05 velocity override bypass
		IPort    *pVelocityBy_06;   // MIDI Channel 06 velocity override bypass
		IPort    *pVelocityBy_07;   // MIDI Channel 07 velocity override bypass
		IPort    *pVelocityBy_08;   // MIDI Channel 08 velocity override bypass
		IPort    *pVelocityBy_09;   // MIDI Channel 09 velocity override bypass
		IPort    *pVelocityBy_10;   // MIDI Channel 10 velocity override bypass
		IPort    *pVelocityBy_11;   // MIDI Channel 11 velocity override bypass
		IPort    *pVelocityBy_12;   // MIDI Channel 12 velocity override bypass
		IPort    *pVelocityBy_13;   // MIDI Channel 13 velocity override bypass
		IPort    *pVelocityBy_14;   // MIDI Channel 14 velocity override bypass
		IPort    *pVelocityBy_15;   // MIDI Channel 15 velocity override bypass
		IPort    *pVelocityBy_16;   // MIDI Channel 16 velocity override bypass

		IPort    *pVelocityClamp_01; // MIDI Channel 01 velocity clamp
		IPort    *pVelocityClamp_02; // MIDI Channel 02 velocity clamp
		IPort    *pVelocityClamp_03; // MIDI Channel 03 velocity clamp
		IPort    *pVelocityClamp_04; // MIDI Channel 04 velocity clamp
		IPort    *pVelocityClamp_05; // MIDI Channel 05 velocity clamp
		IPort    *pVelocityClamp_06; // MIDI Channel 06 velocity clamp
		IPort    *pVelocityClamp_07; // MIDI Channel 07 velocity clamp
		IPort    *pVelocityClamp_08; // MIDI Channel 08 velocity clamp
		IPort    *pVelocityClamp_09; // MIDI Channel 09 velocity clamp
		IPort    *pVelocityClamp_10; // MIDI Channel 10 velocity clamp
		IPort    *pVelocityClamp_11; // MIDI Channel 11 velocity clamp
		IPort    *pVelocityClamp_12; // MIDI Channel 12 velocity clamp
		IPort    *pVelocityClamp_13; // MIDI Channel 13 velocity clamp
		IPort    *pVelocityClamp_14; // MIDI Channel 14 velocity clamp
		IPort    *pVelocityClamp_15; // MIDI Channel 15 velocity clamp
		IPort    *pVelocityClamp_16; // MIDI Channel 16 velocity clamp

		IPort    *pVelocityClampBy_01;     // MIDI Channel 01 velocity clamp bypass
		IPort    *pVelocityClampBy_02;     // MIDI Channel 02 velocity clamp bypass
		IPort    *pVelocityClampBy_03;     // MIDI Channel 03 velocity clamp bypass
		IPort    *pVelocityClampBy_04;     // MIDI Channel 04 velocity clamp bypass
		IPort    *pVelocityClampBy_05;     // MIDI Channel 05 velocity clamp bypass
		IPort    *pVelocityClampBy_06;     // MIDI Channel 06 velocity clamp bypass
		IPort    *pVelocityClampBy_07;     // MIDI Channel 07 velocity clamp bypass
		IPort    *pVelocityClampBy_08;     // MIDI Channel 08 velocity clamp bypass
		IPort    *pVelocityClampBy_09;     // MIDI Channel 09 velocity clamp bypass
		IPort    *pVelocityClampBy_10;     // MIDI Channel 10 velocity clamp bypass
		IPort    *pVelocityClampBy_11;     // MIDI Channel 11 velocity clamp bypass
		IPort    *pVelocityClampBy_12;     // MIDI Channel 12 velocity clamp bypass
		IPort    *pVelocityClampBy_13;     // MIDI Channel 13 velocity clamp bypass
		IPort    *pVelocityClampBy_14;     // MIDI Channel 14 velocity clamp bypass
		IPort    *pVelocityClampBy_15;     // MIDI Channel 15 velocity clamp bypass
		IPort    *pVelocityClampBy_16;     // MIDI Channel 16 velocity clamp bypass

		IPort    *pAmp_01;     // MIDI Channel 01 velocity amplification
		IPort    *pAmp_02;     // MIDI Channel 02 velocity amplification
		IPort    *pAmp_03;     // MIDI Channel 03 velocity amplification
		IPort    *pAmp_04;     // MIDI Channel 04 velocity amplification
		IPort    *pAmp_05;     // MIDI Channel 05 velocity amplification
		IPort    *pAmp_06;     // MIDI Channel 06 velocity amplification
		IPort    *pAmp_07;     // MIDI Channel 07 velocity amplification
		IPort    *pAmp_08;     // MIDI Channel 08 velocity amplification
		IPort    *pAmp_09;     // MIDI Channel 09 velocity amplification
		IPort    *pAmp_10;     // MIDI Channel 10 velocity amplification
		IPort    *pAmp_11;     // MIDI Channel 11 velocity amplification
		IPort    *pAmp_12;     // MIDI Channel 12 velocity amplification
		IPort    *pAmp_13;     // MIDI Channel 13 velocity amplification
		IPort    *pAmp_14;     // MIDI Channel 14 velocity amplification
		IPort    *pAmp_15;     // MIDI Channel 15 velocity amplification
		IPort    *pAmp_16;     // MIDI Channel 16 velocity amplification

		IPort    *pAmpBC_01;     // MIDI Channel 01 velocity amp before clamp
		IPort    *pAmpBC_02;     // MIDI Channel 02 velocity amp before clamp
		IPort    *pAmpBC_03;     // MIDI Channel 03 velocity amp before clamp
		IPort    *pAmpBC_04;     // MIDI Channel 04 velocity amp before clamp
		IPort    *pAmpBC_05;     // MIDI Channel 05 velocity amp before clamp
		IPort    *pAmpBC_06;     // MIDI Channel 06 velocity amp before clamp
		IPort    *pAmpBC_07;     // MIDI Channel 07 velocity amp before clamp
		IPort    *pAmpBC_08;     // MIDI Channel 08 velocity amp before clamp
		IPort    *pAmpBC_09;     // MIDI Channel 09 velocity amp before clamp
		IPort    *pAmpBC_10;     // MIDI Channel 10 velocity amp before clamp
		IPort    *pAmpBC_11;     // MIDI Channel 11 velocity amp before clamp
		IPort    *pAmpBC_12;     // MIDI Channel 12 velocity amp before clamp
		IPort    *pAmpBC_13;     // MIDI Channel 13 velocity amp before clamp
		IPort    *pAmpBC_14;     // MIDI Channel 14 velocity amp before clamp
		IPort    *pAmpBC_15;     // MIDI Channel 15 velocity amp before clamp
		IPort    *pAmpBC_16;     // MIDI Channel 16 velocity amp before clamp
};

class midi_override_imp: public midi_override_base, public midi_override_metadata
{

};

#endif /* CORE_PLUGINS_MIDI_EDIT_H_ */
