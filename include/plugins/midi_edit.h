#ifndef CORE_PLUGINS_MIDI_EDIT_H_
#define CORE_PLUGINS_MIDI_EDIT_H_

#include <core/plugin.h>
#include <core/midi.h>

#include <metadata/plugins.h>

using namespace lsp;

class midi_edit
{
    public:
        void init();
        void destroy();
        void process();
};

#endif /* CORE_PLUGINS_MIDI_EDIT_H_ */
