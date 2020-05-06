/*
 * LSPSaveFile.h
 *
 *  Created on: 20 нояб. 2017 г.
 *      Author: sadko
 */

#ifndef UI_TK_WIDGETS_LSPSAVEFILE_H_
#define UI_TK_WIDGETS_LSPSAVEFILE_H_

namespace lsp
{
    namespace tk
    {
        enum save_file_state_t
        {
            SFS_SELECT,
            SFS_SAVING,
            SFS_SAVED,
            SFS_ERROR
        };

        class LSPSaveFile: public LSPWidget
        {
            public:
                static const w_class_t    metadata;

                enum _save_file_state_t
                {
                    SFS_TOTAL   = SFS_ERROR + 1,

                    S_PRESSED   = 1 << 0
                };

            protected:
                typedef struct state_t
                {
                    LSPColor       *pColor;
                    LSPString       sText;
                } state_t;

            protected:
                save_file_state_t   nState;
                state_t             vStates[SFS_TOTAL];
                float               fProgress;
                size_t              nButtons;
                size_t              nBtnState;
                ssize_t             nSize;
                LSPFont             sFont;
                LSPFileDialog       sDialog;
                ISurface           *pDisk;
                LSPString           sPath;

            protected:
                ISurface           *render_disk(ISurface *s, ssize_t w, const Color &c, const Color &bg);
                static status_t     slot_on_activate(LSPWidget *sender, void *ptr, void *data);
                static status_t     slot_on_submit(LSPWidget *sender, void *ptr, void *data);
                static status_t     slot_on_close(LSPWidget *sender, void *ptr, void *data);
                static status_t     slot_on_file_submit(LSPWidget *sender, void *ptr, void *data);
                static status_t     slot_on_dialog_close(LSPWidget *sender, void *ptr, void *data);

            public:
                explicit LSPSaveFile(LSPDisplay *dpy);
                virtual ~LSPSaveFile();

                virtual status_t init();
                virtual void destroy();

            public:
                inline save_file_state_t    state() const { return nState; }
                LSPColor                   *state_color(size_t i);
                const char                 *state_text(size_t i) const;
                status_t                    get_state_text(size_t i, LSPString *dst);
                inline float                progress() const { return fProgress; };
                const char                 *file_name() const;
                status_t                    get_file_name(LSPString *dst);
                inline LSPFont             *font() { return &sFont; }
                inline LSPFileFilter       *filter() { return sDialog.filter(); }
                inline status_t             get_path(LSPString *dst) const { return (dst->set(&sPath)) ? STATUS_OK : STATUS_NO_MEM; }
                inline const char          *get_path() const { return sPath.get_native(); }
                inline ssize_t              size() const { return nSize; }

            public:
                status_t    set_state(save_file_state_t state);
                status_t    set_state_text(size_t i, const char *s);
                status_t    set_state_text(size_t i, const LSPString *s);
                status_t    set_progress(float value);
                status_t    set_path(const LSPString *path);
                status_t    set_path(const char *path);
                void        set_size(ssize_t size);

            public:
                virtual void draw(ISurface *s);
                virtual void size_request(size_request_t *r);
                virtual status_t on_mouse_down(const ws_event_t *e);
                virtual status_t on_mouse_up(const ws_event_t *e);
                virtual status_t on_mouse_move(const ws_event_t *e);
                virtual status_t on_activate();
                virtual status_t on_submit();
                virtual status_t on_close();
        };
    
    } /* namespace tk */
} /* namespace lsp */

#endif /* UI_TK_WIDGETS_LSPSAVEFILE_H_ */
