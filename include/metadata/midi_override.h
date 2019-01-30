#ifndef METADATA_MIDI_OVERRIDE_H_
#define METADATA_MIDI_OVERRIDE_H_

#include <metadata/metadata.h>

namespace lsp
{
	struct midi_override_metadata
	{
		/* 0-127 used internally */
		static const float PROGRAM_MIN   = 1.0f;
		static const float PROGRAM_MAX   = 128.0f;
		static const float PROGRAM_DFL   = 1.0f;
		static const float PROGRAM_STEP  = 1.0f;

		static const float VELOCITY_MIN  = 0.0f;
		static const float VELOCITY_MAX  = 255.0f;
		static const float VELOCITY_DFL  = 128.0f;
		static const float VELOCITY_STEP = 1.0f;

		static const float CLAMP_MIN     = 0.0f;
		static const float CLAMP_MAX     = 255.0f;
		static const float CLAMP_DFL     = 128.0f;
		static const float CLAMP_STEP    = 1.0f;

		static const float AMP_MIN       = -255.0f;
		static const float AMP_MAX       = +255.0f;
		static const float AMP_DFL       = 0.0f;
		static const float AMP_STEP      = 1.0f;

    	static const plugin_metadata_t metadata;
	};
}
#endif /* METADATA_MIDI_OVERRIDE_H_ */
