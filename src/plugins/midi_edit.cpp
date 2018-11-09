/*************************************************
 * Midi Editor                                   *
 * Created by: Ethan Snyder                      *
 * Created on: 11 November, 2018                 *
 *************************************************/

 #include <dsp/dsp.h>

 #include <core/debug.h>
 #include <plugins/midi_edit.h>
 #include <math.h>

 #include <string.h>

 using namespace lsp;

/**
 * Constructor
 **/
 midi_edit::midi_edit()
 {
	 lsp_trace("constructor");
 }

/**
 * Destructor
 **/
 midi_edit::~midi_edit()
 {
	 destroy();
 }

 void midi_edit::init()
 {

 }

 void midi_edit::destroy()
 {
	 plugin_t::destroy();
 }

 void midi_edit::configure()
 {

 }

void midi_edit::process()
{

}
