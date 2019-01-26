/*
 * LSPHyperlink.cpp
 *
 *  Created on: 23 окт. 2017 г.
 *      Author: sadko
 */

#include <ui/tk/tk.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t LSPHyperlink::metadata = { "LSPHyperlink", &LSPLabel::metadata };

        LSPHyperlink::LSPHyperlink(LSPDisplay *dpy):
            LSPLabel(dpy),
            sStdMenu(dpy)
        {
            pClass      = &metadata;
            nMFlags     = 0;
            nState      = 0;
            pPopup      = &sStdMenu;

            vStdItems[0]    = NULL;
            vStdItems[1]    = NULL;
        }

        LSPHyperlink::~LSPHyperlink()
        {
        }

        status_t LSPHyperlink::init()
        {
            status_t res = LSPLabel::init();
            if (res != STATUS_SUCCESS)
                return res;

            init_color(C_HLINK_TEXT, sFont.color());
            init_color(C_HLINK_HOVER, &sHoverColor);
            set_cursor(MP_HAND); // Initialize pointer
            sFont.set_underline();

            ui_handler_id_t id = 0;

            LSP_STATUS_ASSERT(sStdMenu.init());
            LSPMenuItem *mi = new LSPMenuItem(pDisplay);
            if (mi == NULL)
                return STATUS_NO_MEM;
            vStdItems[0] = mi;
            LSP_STATUS_ASSERT(mi->init());
            LSP_STATUS_ASSERT(sStdMenu.add(mi));
            LSP_STATUS_ASSERT(mi->set_text("Copy link"));
            id = mi->slots()->bind(LSPSLOT_SUBMIT, slot_copy_link_action, self());
            if (id < 0)
                return -id;

            mi = new LSPMenuItem(pDisplay);
            if (mi == NULL)
                return STATUS_NO_MEM;
            vStdItems[1] = mi;
            LSP_STATUS_ASSERT(mi->init());
            LSP_STATUS_ASSERT(sStdMenu.add(mi));
            LSP_STATUS_ASSERT(mi->set_text("Follow link"));
            id = mi->slots()->bind(LSPSLOT_SUBMIT, slot_on_submit, self());
            if (id < 0)
                return -id;

            id = sSlots.add(LSPSLOT_SUBMIT, slot_on_submit, self());
            if (id < 0) return -id;

            return STATUS_OK;
        }

        status_t LSPHyperlink::slot_on_submit(LSPWidget *sender, void *ptr, void *data)
        {
            LSPHyperlink *_this = widget_ptrcast<LSPHyperlink>(ptr);
            return (_this != NULL) ? _this->on_submit() : STATUS_BAD_ARGUMENTS;
        }

        status_t LSPHyperlink::slot_copy_link_action(LSPWidget *sender, void *ptr, void *data)
        {
            LSPHyperlink *_this = widget_ptrcast<LSPHyperlink>(ptr);
            if (_this == NULL)
                return STATUS_BAD_ARGUMENTS;

            LSPTextClipboard *cb = new LSPTextClipboard();
            if (cb == NULL)
                return STATUS_NO_MEM;
            status_t result = cb->update_text(&_this->sUrl);
            if (result == STATUS_OK)
                _this->pDisplay->write_clipboard(CBUF_CLIPBOARD, cb);
            return cb->close();
        }

        status_t LSPHyperlink::on_submit()
        {
            lsp_trace("hyperlink submitted");

            int status = 0;
            const char *url = sUrl.get_native();

            pid_t pid = fork();
            if (pid == 0)
            {
                lsp_trace("url is: %s", url);
                execlp("xdg-open", "xdg-open", url, NULL);
                exit(1);
            }
            else if (pid < 0)
            {
                lsp_trace("bad fork");
            }
            else
            {
                waitpid(pid, &status, WNOHANG);
                lsp_trace("Child process exted with status %d", status);
            }

            return STATUS_OK;
        }

        void LSPHyperlink::destroy()
        {
            for (size_t i=0; i<2; ++i)
                if (vStdItems[i] != NULL)
                {
                    vStdItems[i]->destroy();
                    delete vStdItems[i];
                    vStdItems[i] = NULL;
                }

            LSPLabel::destroy();
        }

        status_t LSPHyperlink::set_url(const char *url)
        {
            return (sUrl.set_native(url)) ? STATUS_OK : STATUS_NO_MEM;
        }

        status_t LSPHyperlink::set_url(const LSPString *url)
        {
            return (sUrl.set(url)) ? STATUS_OK : STATUS_NO_MEM;
        }

        void LSPHyperlink::draw(ISurface *s)
        {
            // Draw background
            s->fill_rect(0, 0, sSize.nWidth, sSize.nHeight, sBgColor);

            // Get text parameters
            font_parameters_t fp;
            text_parameters_t tp;
            sFont.get_parameters(s, &fp);

            // Estimate number of lines
            ssize_t n_lines = 1 + sText.count('\n');
            ssize_t dy      = sSize.nHeight - fp.Height*n_lines - (nBorder << 1);
            ssize_t y       = nBorder - fp.Descent + dy * fVAlign;

            // Determine which color to use
            Color *cl       = (nState & F_MOUSE_IN) ? &sHoverColor : sFont.color();

            // Estimate text size
            ssize_t last = 0, curr = 0, tail = 0, len = sText.length();

            while (curr < len)
            {
                // Get next line indexes
                curr    = sText.index_of(last, '\n');
                if (curr < 0)
                {
                    curr        = len;
                    tail        = len;
                }
                else
                {
                    tail        = curr;
                    if ((tail > last) && (sText.at(tail-1) == '\r'))
                        --tail;
                }

                // Calculate text location
                sFont.get_text_parameters(s, &tp, &sText, last, tail);
                ssize_t dx  = sSize.nWidth - tp.Width - (nBorder << 1);
                ssize_t x   = nBorder + dx * fHAlign - tp.XBearing;
                y          += fp.Height;

                sFont.draw(s, x, y, *cl, &sText, last, tail);
                last    = curr + 1;
            }
        }

        status_t LSPHyperlink::on_mouse_in(const ws_event_t *e)
        {
            LSPWidget::on_mouse_in(e);

            if (nState & F_MOUSE_IGN)
                return STATUS_OK;
            size_t flags = nState;
            if ((nState & F_MOUSE_DOWN) && (nMFlags == (1 << MCB_LEFT)))
                nState |= F_MOUSE_IN;
            else if (nMFlags == 0)
                nState |= F_MOUSE_IN;
            else
                nState &= ~F_MOUSE_IN;
            if (flags != nState)
                query_draw();
            return STATUS_OK;
        }

        status_t LSPHyperlink::on_mouse_out(const ws_event_t *e)
        {
            LSPWidget::on_mouse_out(e);

            if (nState & F_MOUSE_IGN)
                return STATUS_OK;
            size_t flags = nState;
            if ((nState & F_MOUSE_DOWN) && (nMFlags == (1 << MCB_LEFT)))
                nState |= F_MOUSE_IN;
            else
                nState &= ~F_MOUSE_IN;
            if (flags != nState)
                query_draw();
            return STATUS_OK;
        }

        status_t LSPHyperlink::on_mouse_move(const ws_event_t *e)
        {
            if (nState & F_MOUSE_IGN)
                return STATUS_OK;
            size_t flags = nState;
            if ((nState & F_MOUSE_DOWN) && (nMFlags == (1 << MCB_LEFT)) && (inside(e->nLeft, e->nTop)))
                nState |= F_MOUSE_IN;
            else if (nMFlags == 0)
                nState |= F_MOUSE_IN;
            else
                nState &= ~F_MOUSE_IN;
            if (flags != nState)
                query_draw();
            return STATUS_OK;
        }

        status_t LSPHyperlink::on_mouse_down(const ws_event_t *e)
        {
            size_t flags = nState;

            if (nMFlags == 0)
            {
                if (e->nCode == MCB_LEFT)
                    nState |= F_MOUSE_DOWN | F_MOUSE_IN;
                else
                    nState |= F_MOUSE_IGN;
            }

            nMFlags |= 1 << e->nCode;

            if ((nState & F_MOUSE_DOWN) && (nMFlags == (1 << MCB_LEFT)) && (inside(e->nLeft, e->nTop)))
                nState |= F_MOUSE_IN;
            else if (nMFlags == 0)
                nState |= F_MOUSE_IN;
            else
                nState &= ~F_MOUSE_IN;

            if (flags != nState)
                query_draw();
            return STATUS_OK;
        }

        status_t LSPHyperlink::on_mouse_up(const ws_event_t *e)
        {
            size_t flags = nMFlags;
            nMFlags &= ~ (1 << e->nCode);
            if (nMFlags == 0)
                nState      = 0;

            if ((nState & F_MOUSE_DOWN) && (nMFlags == (1 << MCB_LEFT)) && (inside(e->nLeft, e->nTop)))
                nState |= F_MOUSE_IN;
            else if (nMFlags == 0)
                nState |= F_MOUSE_IN;
            else
                nState &= ~F_MOUSE_IN;

            if (flags != nState)
                query_draw();

            // Trigger submit action
            if (inside(e->nLeft, e->nTop))
            {
                if ((flags == (1 << MCB_LEFT)) && (e->nCode == MCB_LEFT))
                    sSlots.execute(LSPSLOT_SUBMIT, this);
                else if ((flags == (1 << MCB_RIGHT)) && (e->nCode == MCB_RIGHT) && (pPopup != NULL))
                    pPopup->show(this, e);
            }

            return STATUS_OK;
        }
    } /* namespace tk */
} /* namespace lsp */
