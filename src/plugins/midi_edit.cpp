/*************************************************
 * Midi Override                                 *
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

void midi_override_base::update_settings()
{

}
