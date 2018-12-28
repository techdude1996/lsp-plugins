/*
 * reflections3d.cpp
 *
 *  Created on: 24 дек. 2018 г.
 *      Author: sadko
 */

#include <test/mtest.h>
#include <test/mtest/3d/common/X11Renderer.h>
#include <core/files/Model3DFile.h>

#include <core/types.h>
#include <core/debug.h>
#include <core/sugar.h>
#include <core/status.h>
#include <stdlib.h>
#include <errno.h>
#include <data/cstorage.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <sys/poll.h>

namespace mtest
{
    using namespace lsp;

    static const color3d_t C_RED        = { 1.0f, 0.0f, 0.0f, 0.0f };
    static const color3d_t C_GREEN      = { 0.0f, 1.0f, 0.0f, 0.0f };
    static const color3d_t C_BLUE       = { 0.0f, 0.0f, 1.0f, 0.0f };
    static const color3d_t C_MAGENTA    = { 1.0f, 0.0f, 1.0f, 0.0f };
    static const color3d_t C_YELLOW     = { 1.0f, 1.0f, 0.0f, 0.0f };
    static const color3d_t C_ORANGE     = { 1.0f, 0.5f, 0.0f, 0.0f };
    static const color3d_t C_GRAY       = { 0.75f, 0.75f, 0.75f, 0.0f };

    typedef struct wfront_t
    {
        ray3d_t  r[3];      // Rays, counter-clockwise order
        point3d_t s;        // Source point
    } wfront_t;

    typedef struct object_t
    {
        bound_box3d_t               box;      // Bounding box for each object
        size_t                      nt;     // Number of triangles
        v_triangle3d_t             *t;      // List of triangles for each object
    } object_t;

    typedef struct context_t
    {
        wfront_t                    front;      // Wave front
        cstorage<v_triangle3d_t>    source;     // Triangles for processing
        cstorage<v_triangle3d_t>   *matched;    // List of matched triangles (for debug)
        cstorage<v_triangle3d_t>   *ignored;    // List of ignored triangles (for debug)
        cvector<object_t>          *scene;      // Overall scene
        bool                        scan;       // Fully scan scene
    } context_t;

    static void calc_plane_vector_p3(vector3d_t *v, const point3d_t *p0, const point3d_t *p1, const point3d_t *p2)
    {
        // Calculate edge parameters
        vector3d_t d[2];
        d[0].dx     = p1->x - p0->x;
        d[0].dy     = p1->y - p0->y;
        d[0].dz     = p1->z - p0->z;
        d[0].dw     = p1->w - p0->w;

        d[1].dx     = p2->x - p0->x;
        d[1].dy     = p2->y - p0->y;
        d[1].dz     = p2->z - p0->z;
        d[1].dw     = p2->w - p0->w;

        // Do vector multiplication to calculate the normal vector
        v->dx       = + d[0].dy*d[1].dz - d[0].dz*d[1].dy;
        v->dy       = - d[0].dx*d[1].dz + d[0].dz*d[1].dx;
        v->dz       = + d[0].dx*d[1].dy - d[0].dy*d[1].dx;
        dsp::normalize_vector(v);
        v->dw       = - ( v->dx * p0->x + v->dy * p0->y + v->dz * p0->z); // Parameter for the plane equation
    }

    /**
     * Calculate vector that represents plane equation
     * @param v target vector to store equation
     * @param r ray representing first vector
     * @param u another vector representing plane
     */
    static void calc_plane_vector_rv(vector3d_t *v, const ray3d_t *r, const vector3d_t *u)
    {
        // Calculate edge parameters
        const vector3d_t  *w  = &r->v;
        const point3d_t *p    = &r->z;

        // Do vector multiplication to calculate the normal vector
        v->dx       = + w->dy * u->dz - w->dz*u->dy;
        v->dy       = - w->dx * u->dz + w->dz*u->dx;
        v->dz       = + w->dx * u->dy - w->dy*u->dx;
        dsp::normalize_vector(v);
        v->dw       = - ( v->dx * p->x + v->dy * p->y + v->dz * p->z); // Parameter for the plane equation
    }

    /**
     * Split triangle with plane
     * @param out array of vertexes above plane
     * @param n_out counter of vertexes above plane (multiple of 3), should be initialized
     * @param in array of vertexes below plane
     * @param n_in counter of vertexes below plane (multiple of 3), should be iniitialized
     * @param pl plane equation
     * @param pv triangle to perform the split
     */
    static void split_triangle(
            v_triangle3d_t *out,
            size_t *n_out,
            v_triangle3d_t *in,
            size_t *n_in,
            const vector3d_t *pl,
            const v_triangle3d_t *pv
        )
    {
        point3d_t sp[2];    // Split point
        vector3d_t d[2];    // Delta vector
        point3d_t p[3];     // Triangle sources
        float k[3];         // Co-location of points
        float t[2];

        in     += *n_in;
        out    += *n_out;

        p[0]    = pv->p[0];
        p[1]    = pv->p[1];
        p[2]    = pv->p[2];

        k[0]    = pl->dx*p[0].x + pl->dy*p[0].y + pl->dz*p[0].z + pl->dw;
        k[1]    = pl->dx*p[1].x + pl->dy*p[1].y + pl->dz*p[1].z + pl->dw;
        k[2]    = pl->dx*p[2].x + pl->dy*p[2].y + pl->dz*p[2].z + pl->dw;

        // Check that the whole triangle lies above the plane or below the plane
        if (k[0] < 0.0f)
        {
            if ((k[1] <= 0.0f) && (k[2] <= 0.0f))
            {
                in[0].p[0]      = p[0];
                in[0].p[1]      = p[1];
                in[0].p[2]      = p[2];
                *n_in          += 1;
                return;
            }
        }
        else if (k[0] > 0.0f)
        {
            if ((k[1] >= 0.0f) && (k[2] >= 0.0f))
            {
                out[0].p[0]     = p[0];
                out[0].p[1]     = p[1];
                out[0].p[2]     = p[2];
                *n_out         += 1;
                return;
            }
        }
        else // (k[0] == 0)
        {
            if ((k[1] >= 0.0f) && (k[2] >= 0.0f))
            {
                out[0].p[0]     = p[0];
                out[0].p[1]     = p[1];
                out[0].p[2]     = p[2];
                *n_out         += 1;
                return;
            }
            else if ((k[1] <= 0.0f) && (k[2] <= 0.0f))
            {
                in[0].p[0]      = p[0];
                in[0].p[1]      = p[1];
                in[0].p[2]      = p[2];
                *n_in          += 1;
                return;
            }
        }

        // There is an intersection with plane, we need to analyze it
        // Rotate triangle until vertex 0 is above the split plane
        while (k[0] <= 0)
        {
            t[0]    = k[0];
            sp[0]   = p[0];

            k[0]    = k[1];
            p[0]    = p[1];
            k[1]    = k[2];
            p[1]    = p[2];
            k[2]    = t[0];
            p[2]    = sp[0];
        }

        // Now we have p[0] guaranteed to be above plane, analyze p[1] and p[2]
        if (k[1] < 0)
        {
            d[0].dx = p[0].x - p[1].x;
            d[0].dy = p[0].y - p[1].y;
            d[0].dz = p[0].z - p[1].z;

            t[0]    = -k[0] / (pl->dx*d[0].dx + pl->dy*d[0].dy + pl->dz*d[0].dz);

            sp[0].x = p[0].x + d[0].dx * t[0];
            sp[0].y = p[0].y + d[0].dy * t[0];
            sp[0].z = p[0].z + d[0].dz * t[0];
            sp[0].w = 1.0f;

            if (k[2] < 0)
            {
                d[1].dx = p[0].x - p[2].x;
                d[1].dy = p[0].y - p[2].y;
                d[1].dz = p[0].z - p[2].z;

                t[1]    = -k[0] / (pl->dx*d[1].dx + pl->dy*d[1].dy + pl->dz*d[1].dz);

                sp[1].x = p[0].x + d[1].dx * t[1];
                sp[1].y = p[0].y + d[1].dy * t[1];
                sp[1].z = p[0].z + d[1].dz * t[1];
                sp[1].w = 1.0f;

                // 1 triangle above plane, 2 below
                out[0].p[0]     = p[0];
                out[0].p[1]     = sp[0];
                out[0].p[2]     = sp[1];
                *n_out         += 1;

                in[0].p[0]      = p[1];
                in[0].p[1]      = p[2];
                in[0].p[2]      = sp[0];
                in[1].p[0]      = p[2];
                in[1].p[1]      = sp[1];
                in[1].p[2]      = sp[0];
                *n_in          += 2;
            }
            else if (k[2] > 0)
            {
                d[1].dx = p[2].x - p[1].x;
                d[1].dy = p[2].y - p[1].y;
                d[1].dz = p[2].z - p[1].z;

                t[1]    = -k[2] / (pl->dx*d[1].dx + pl->dy*d[1].dy + pl->dz*d[1].dz);

                sp[1].x = p[2].x + d[1].dx * t[1];
                sp[1].y = p[2].y + d[1].dy * t[1];
                sp[1].z = p[2].z + d[1].dz * t[1];
                sp[1].w = 1.0f;

                // 2 triangles above plane, 1 below
                out[0].p[0]     = p[2];
                out[0].p[1]     = p[0];
                out[0].p[2]     = sp[0];
                out[1].p[0]     = p[2];
                out[1].p[1]     = sp[0];
                out[1].p[2]     = sp[1];
                *n_out         += 2;

                in[0].p[0]      = p[1];
                in[0].p[1]      = sp[1];
                in[0].p[2]      = sp[0];
                *n_in          += 1;
            }
            else // k[2] == 0
            {
                // 1 triangle above plane, 1 below
                out[0].p[0]     = p[2];
                out[0].p[1]     = p[0];
                out[0].p[2]     = sp[0];
                *n_out         += 1;

                in[0].p[0]      = p[1];
                in[0].p[1]      = p[2];
                in[0].p[2]      = sp[0];
                *n_in          += 1;
            }
        }
        else // (k[1] > 0) && (k[2] < 0)
        {
            d[0].dx = p[0].x - p[2].x;
            d[0].dy = p[0].y - p[2].y;
            d[0].dz = p[0].z - p[2].z;

            d[1].dx = p[1].x - p[2].x;
            d[1].dy = p[1].y - p[2].y;
            d[1].dz = p[1].z - p[2].z;

            t[0]    = -k[0] / (pl->dx*d[0].dx + pl->dy*d[0].dy + pl->dz*d[0].dz);
            t[1]    = -k[1] / (pl->dx*d[1].dx + pl->dy*d[1].dy + pl->dz*d[1].dz);

            sp[0].x = p[0].x + d[0].dx * t[0];
            sp[0].y = p[0].y + d[0].dy * t[0];
            sp[0].z = p[0].z + d[0].dz * t[0];
            sp[0].w = 1.0f;

            sp[1].x = p[1].x + d[1].dx * t[1];
            sp[1].y = p[1].y + d[1].dy * t[1];
            sp[1].z = p[1].z + d[1].dz * t[1];
            sp[1].w = 1.0f;

            // 2 triangles above plane, 1 below
            out[0].p[0]     = p[0];
            out[0].p[1]     = p[1];
            out[0].p[2]     = sp[1];
            out[1].p[0]     = p[0];
            out[1].p[1]     = sp[1];
            out[1].p[2]     = sp[0];
            *n_out         += 2;

            in[0].p[0]      = p[2];
            in[0].p[1]      = sp[0];
            in[0].p[2]      = sp[1];
            *n_in          += 1;
        }
    }

    /**
     * Project triangle to the plane
     * @param pv array of 3 points to store projected points
     * @param fp focus point, the point where all projective lines do intersect
     * @param pl plane equation vector
     * @param tv triangle points
     */
    static void project_triangle(
        point3d_t *pv,
        const point3d_t *fp,
        const vector3d_t *pl,
        const point3d_t *tv
    )
    {
        vector3d_t d[3];
        float k, t[3];

        d[0].dx     = fp->x - tv[0].x;
        d[0].dy     = fp->y - tv[0].y;
        d[0].dz     = fp->z - tv[0].z;
        d[0].dw     = 0.0f;

        d[1].dx     = fp->x - tv[1].x;
        d[1].dy     = fp->y - tv[1].y;
        d[1].dz     = fp->z - tv[1].z;
        d[1].dw     = 0.0f;

        d[2].dx     = fp->x - tv[2].x;
        d[2].dy     = fp->y - tv[2].y;
        d[2].dz     = fp->z - tv[2].z;
        d[2].dw     = 0.0f;

        k           = - (pl->dx*fp->x + pl->dy*fp->y + pl->dz*fp->z + pl->dw);
        t[0]        = k / (pl->dx*d[0].dx + pl->dy*d[0].dy + pl->dz*d[0].dz);
        t[1]        = k / (pl->dx*d[1].dx + pl->dy*d[1].dy + pl->dz*d[1].dz);
        t[2]        = k / (pl->dx*d[2].dx + pl->dy*d[2].dy + pl->dz*d[2].dz);

        pv[0].x     = fp->x + t[0] * d[0].dx;
        pv[0].y     = fp->y + t[0] * d[0].dy;
        pv[0].z     = fp->z + t[0] * d[0].dz;
        pv[0].w     = 1.0f;

        pv[1].x     = fp->x + t[1] * d[1].dx;
        pv[1].y     = fp->y + t[1] * d[1].dy;
        pv[1].z     = fp->z + t[1] * d[1].dz;
        pv[1].w     = 1.0f;

        pv[2].x     = fp->x + t[2] * d[2].dx;
        pv[2].y     = fp->y + t[2] * d[2].dy;
        pv[2].z     = fp->z + t[2] * d[2].dz;
        pv[2].w     = 1.0f;
    }

    static const size_t bbox_map[] =
    {
        0, 1, 2,
        0, 2, 3,
        6, 5, 4,
        6, 4, 7,
        1, 0, 4,
        1, 4, 5,
        3, 2, 6,
        3, 6, 7,
        1, 5, 2,
        2, 5, 6,
        0, 3, 4,
        3, 7, 4
    };

    static void destroy_scene(cvector<object_t> &list)
    {
        for (size_t i=0, n=list.size(); i<n; ++i)
        {
            object_t *obj = list.get(i);
            if (obj != NULL)
                free(obj);
        }
        list.flush();
    }

    static status_t prepare_scene(cstorage<v_triangle3d_t> &ignored, cvector<object_t> &scene, Scene3D *s)
    {
        status_t res        = STATUS_OK;
        size_t hsize        = ALIGN_SIZE(sizeof(object_t), DEFAULT_ALIGN);

        cvector<object_t> ol;

        for (size_t i=0, n=s->num_objects(); i<n; ++i)
        {
            // Get scene object
            Object3D *obj   = s->get_object(i);
            if ((obj == NULL) || (!obj->is_visible()))
                continue;

            // Add bounding box
            bound_box3d_t *bbox = s->get_bound_box(i);
            if (bbox == NULL)
                continue;

            // Initialize pointers
            matrix3d_t *om      = obj->get_matrix();
            point3d_t *tr       = obj->get_vertexes();
            vector3d_t *tn      = obj->get_normals();
            vertex_index_t *vvx = obj->get_vertex_indexes();
            vertex_index_t *vnx = obj->get_normal_indexes();

            // Allocate object descriptor
            size_t nt           = obj->get_triangles_count();
            size_t osize        = hsize + ALIGN_SIZE(sizeof(v_triangle3d_t) * nt, DEFAULT_ALIGN);
            object_t *o         = reinterpret_cast<object_t *>(malloc(osize));
            if (o == NULL)
            {
                res             = STATUS_NO_MEM;
                break;
            }
            if (!ol.add(o))
            {
                free(o);
                res             = STATUS_NO_MEM;
                break;
            }

            o->t                = reinterpret_cast<v_triangle3d_t *>(reinterpret_cast<uint8_t *>(o) + hsize);
            o->box              = *bbox;
            o->nt               = nt;

            // Apply object matrix to vertexes and produce final array
            v_triangle3d_t *t   = o->t;

            for (size_t j=0; j < nt; ++j, ++t)
            {
                dsp::apply_matrix3d_mp2(&t->p[0], &tr[*(vvx++)], om);
                dsp::apply_matrix3d_mp2(&t->p[1], &tr[*(vvx++)], om);
                dsp::apply_matrix3d_mp2(&t->p[2], &tr[*(vvx++)], om);

                dsp::apply_matrix3d_mv2(&t->n[0], &tn[*(vnx++)], om);
                dsp::apply_matrix3d_mv2(&t->n[1], &tn[*(vnx++)], om);
                dsp::apply_matrix3d_mv2(&t->n[2], &tn[*(vnx++)], om);
            }
        }

        if (res == STATUS_OK)
            ol.swap_data(&scene);

        destroy_scene(ol);
        return res;
    }

    /**
     * Scan scene for triangles laying inside the viewing area of wave front
     * @param ctx wave front context
     * @return statuf of operation
     */
    static status_t scan_scene(context_t *ctx)
    {
        if (!ctx->scan)
            return STATUS_OK;

        // Check crossing with bounding box
        cstorage<v_triangle3d_t> source;
        vector3d_t pl[4];
        calc_plane_vector_rv(&pl[0], &ctx->front.r[0], &ctx->front.r[1].v);
        calc_plane_vector_rv(&pl[1], &ctx->front.r[1], &ctx->front.r[2].v);
        calc_plane_vector_rv(&pl[2], &ctx->front.r[2], &ctx->front.r[0].v);
        calc_plane_vector_p3(&pl[3], &ctx->front.r[0].z, &ctx->front.r[1].z, &ctx->front.r[2].z);

        v_triangle3d_t out[16], buf1[16], buf2[16], *q, *in, *tmp;
        size_t n_out, n_buf1, n_buf2, *n_q, *n_in, *n_tmp;

        // STEP 1
        // Check for crossing with all bounding boxes
        for (size_t i=0, n=ctx->scene->size(); i<n; ++i)
        {
            object_t *obj = ctx->scene->get(i);
            if (obj->nt < 16)
            {
                if (obj->nt == 0)
                    continue;

                if (!source.append(obj->t, obj->nt))
                    return STATUS_NO_MEM;
                continue;
            }

            // Cull each triangle of bounding box with four scissor planes
            for (size_t j=0, m = sizeof(bbox_map)/sizeof(size_t); j < m; )
            {
                // Initialize input and queue buffer
                q = buf1, in = buf2;
                n_q = &n_buf1, n_in = &n_buf2;

                // Put to queue with updated matrix
                *n_q        = 1;
                n_out       = 0;
                q->p[0]     = obj->box.p[bbox_map[j++]];
                q->p[1]     = obj->box.p[bbox_map[j++]];
                q->p[2]     = obj->box.p[bbox_map[j++]];

                // Cull triangle with planes
                for (size_t k=0; ; )
                {
                    // Reset counters
                    *n_in   = 0;

                    // Split all triangles:
                    // Put all triangles above the plane to out
                    // Put all triangles below the plane to in
                    for (size_t l=0; l < *n_q; ++l)
                        split_triangle(out, &n_out, in, n_in, &pl[k], &q[l]);

                    // Interrupt cycle if there is no data to process
                    if ((*n_in <= 0) || ((++k) >= 4))
                       break;

                    // Swap buffers buf0 <-> buf1
                    n_tmp = n_in, tmp = in;
                    n_in = n_q, in = q;
                    n_q = n_tmp, q = tmp;
                }

                if (*n_in > 0) // Is there intersection with bounding box?
                {
                    if (!source.append(obj->t, obj->nt))
                        return STATUS_NO_MEM;
                }
                else
                {
                    if (!ctx->ignored->append(obj->t, obj->nt))
                        return STATUS_NO_MEM;
                }
            }
        }

        ctx->scan = false;
        if (source.size() <= 0)
            return STATUS_OK;

        // STEP 2
        // Cull each triangle with four scissor planes
        for (ssize_t i=0, n=source.size(); i < n; ++i)
        {
            // Get next triangle for processing
            v_triangle3d_t t   = *(source.at(i));

            // Initialize input and queue buffer
            q = buf1, in = buf2;
            n_q = &n_buf1, n_in = &n_buf2;

            // Put triangle to queue
            *n_q        = 1;
            n_out       = 0;
            *q          = t;

            // Cull triangle with planes
            for (size_t k=0; ; )
            {
                // Reset counters
                *n_in   = 0;
                // Split all triangles:
                // Put all triangles above the plane to out
                // Put all triangles below the plane to in
                for (size_t l=0; l < *n_q; ++l)
                    split_triangle(out, &n_out, in, n_in, &pl[k], &q[l]);

                // Interrupt cycle if there is no data to process
                if ((*n_in <= 0) || ((++k) >= 4))
                   break;

                // Swap buffers buf0 <-> buf1
                n_tmp = n_in, tmp = in;
                n_in = n_q, in = q;
                n_q = n_tmp, q = tmp;
            }

            // Emit all triangles above the plane (outside vision) as ignored
            for (size_t l=0; l < n_out; ++l)
            {
                t.p[0]              = out[l].p[0];
                t.p[1]              = out[l].p[1];
                t.p[2]              = out[l].p[2];
                if (!ctx->ignored->add(&t))
                    return STATUS_NO_MEM;
            }

            // The final set of triangles inside vision is in 'q' buffer, put them as visible
            for (size_t l=0; l < *n_in; ++l)
            {
                t.p[0]              = in[l].p[0];
                t.p[1]              = in[l].p[1];
                t.p[2]              = in[l].p[2];
                if (!ctx->source.add(&t))
                    return STATUS_NO_MEM;
            }
        }

        return STATUS_OK;
    }

    static status_t perform_raytrace(
            cvector<context_t> &tasks
        )
    {
        context_t *ctx = NULL;

        while (tasks.size() > 0)
        {
            // Get next context from queue
            if (!tasks.pop(&ctx))
                return STATUS_CORRUPTED;

            // Check that we need to perform a scan
            if (ctx->scan)
                scan_scene(ctx);

            if (!ctx->matched->add_all(&ctx->source))
                return STATUS_NO_MEM;

            ctx->source.flush();
            delete ctx;
        }

        return STATUS_OK;
    }

    static void destroy_tasks(cvector<context_t> &tasks)
    {
        for (size_t i=0, n=tasks.size(); i<n; ++i)
        {
            context_t *ctx = tasks.get(i);
            if (ctx == NULL)
                continue;

            ctx->ignored    = NULL;
            ctx->matched    = NULL;
            ctx->scene      = NULL;
            ctx->source.flush();

            delete ctx;
        }

        tasks.flush();
    }
} // Namespace mtest

MTEST_BEGIN("3d", reflections)

    class Renderer: public X11Renderer
    {
        private:
            Scene3D        *pScene;
            wfront_t        sFront;
            bool            bBoundBoxes;

        public:
            explicit Renderer(Scene3D *scene, View3D *view): X11Renderer(view)
            {
                pScene = scene;
                bBoundBoxes = true;

                point3d_t p[4];
                dsp::init_point_xyz(&p[0], 0.0f, 1.0f, 0.0f);
                dsp::init_point_xyz(&p[1], -1.0f, -0.5f, 0.0f);
                dsp::init_point_xyz(&p[2], 1.0f, -0.5f, 0.0f);
                dsp::init_point_xyz(&p[3], 0.0f, 0.0f, 1.0f);

                vector3d_t v[3];
                dsp::init_vector_p2(&v[0], &p[3], &p[0]);
                dsp::init_vector_p2(&v[1], &p[3], &p[1]);
                dsp::init_vector_p2(&v[2], &p[3], &p[2]);

                dsp::init_ray_pdv(&sFront.r[0], &p[0], &v[0]);
                dsp::init_ray_pdv(&sFront.r[1], &p[1], &v[1]);
                dsp::init_ray_pdv(&sFront.r[2], &p[2], &v[2]);
                sFront.s = p[3];

                update_view();
            }

            virtual ~Renderer()
            {
            }

        public:
            virtual void on_key_press(const XKeyEvent &ev, KeySym key)
            {
                switch (key)
                {
                    case XK_F1:
                    {
                        float incr = (ev.state & ShiftMask) ? 0.25f : -0.25f;
                        sFront.r[0].z.x += incr;
                        sFront.r[1].z.x += incr;
                        sFront.r[2].z.x += incr;
                        sFront.s.x += incr;
                        update_view();
                        break;
                    }

                    case XK_F2:
                    {
                        float incr = (ev.state & ShiftMask) ? 0.25f : -0.25f;
                        sFront.r[0].z.y += incr;
                        sFront.r[1].z.y += incr;
                        sFront.r[2].z.y += incr;
                        sFront.s.y += incr;
                        update_view();
                        break;
                    }

                    case XK_F3:
                    {
                        float incr = (ev.state & ShiftMask) ? 0.25f : -0.25f;
                        sFront.r[0].z.z += incr;
                        sFront.r[1].z.z += incr;
                        sFront.r[2].z.z += incr;
                        sFront.s.z += incr;
                        update_view();
                        break;
                    }

                    case XK_F4:
                    case XK_F5:
                    case XK_F6:
                    {
                        matrix3d_t m;
                        float incr = (ev.state & ShiftMask) ? M_PI/16.0f : -M_PI/16.0f;

                        for (size_t i=0; i<3; ++i)
                        {
                            sFront.r[i].z.x -= sFront.s.x;
                            sFront.r[i].z.y -= sFront.s.y;
                            sFront.r[i].z.z -= sFront.s.z;
                        }
                        if (key == XK_F4)
                            dsp::init_matrix3d_rotate_x(&m, incr);
                        else if (key == XK_F5)
                            dsp::init_matrix3d_rotate_y(&m, incr);
                        else
                            dsp::init_matrix3d_rotate_z(&m, incr);
                        for (size_t i=0; i<3; ++i)
                        {
                            dsp::apply_matrix3d_mp1(&sFront.r[i].z, &m);
                            dsp::apply_matrix3d_mv1(&sFront.r[i].v, &m);
                        }
                        for (size_t i=0; i<3; ++i)
                        {
                            sFront.r[i].z.x += sFront.s.x;
                            sFront.r[i].z.y += sFront.s.y;
                            sFront.r[i].z.z += sFront.s.z;
                        }
                        update_view();
                        break;
                    }

                    case 'b':
                    {
                        bBoundBoxes = ! bBoundBoxes;
                        update_view();
                        break;
                    }

                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    {
                        Object3D *obj = pScene->get_object(key - '0');
                        if (obj != NULL)
                        {
                            obj->set_visible(!obj->is_visible());
                            update_view();
                        }
                        break;
                    }
                    default:
                        X11Renderer::on_key_press(ev, key);
                        break;
                }
            }

        protected:
            status_t    update_view()
            {
                v_segment3d_t s;
                v_vertex3d_t v[3];

                // Clear view state
                pView->clear_all();

                // List of ignored and matched triangles
                cstorage<v_triangle3d_t> ignored, matched;
                cvector<object_t> scene;
                cvector<context_t> tasks;

                // Create initial context
                context_t *ctx = new context_t;
                if (ctx == NULL)
                    return STATUS_NO_MEM;

                ctx->scan       = true;
                ctx->front      = sFront;
                ctx->ignored    = &ignored;
                ctx->matched    = &matched;
                ctx->scene      = &scene;

                // Add context to tasks
                if (!tasks.add(ctx))
                {
                    delete ctx;
                    return STATUS_NO_MEM;
                }

                // Prepare scene for analysis
                status_t res = prepare_scene(ignored, scene, pScene);
                if (res != STATUS_OK)
                {
                    tasks.flush();
                    delete ctx;
                    return res;
                }

                // Render bounding boxes of the scene
                s.c = C_ORANGE;
                for (size_t i=0, n=scene.size(); i<n; ++i)
                {
                    object_t *o = scene.at(i);
                    bound_box3d_t *bbox = &o->box;

                    if (bBoundBoxes)
                    {
                        for (size_t i=0; i<4; ++i)
                        {
                            s.p[0] = bbox->p[i];
                            s.p[1] = bbox->p[(i+1)%4];
                            pView->add_segment(&s);
                            s.p[0] = bbox->p[i];
                            s.p[1] = bbox->p[i+4];
                            pView->add_segment(&s);
                            s.p[0] = bbox->p[i+4];
                            s.p[1] = bbox->p[(i+1)%4 + 4];
                            pView->add_segment(&s);
                        }
                    }
                }

                // Clear allocated resources, tasks and ctx should be already deleted
                res = perform_raytrace(tasks);

                destroy_tasks(tasks);
                destroy_scene(scene);

                // Build final scene from matched and ignored items
                for (size_t i=0, m=ignored.size(); i < m; ++i)
                {
                    v_triangle3d_t *t = ignored.at(i);
                    v[0].p     = t->p[0];
                    v[0].n     = t->n[0];
                    v[0].c     = C_GRAY;

                    v[1].p     = t->p[1];
                    v[1].n     = t->n[1];
                    v[1].c     = C_GRAY;

                    v[2].p     = t->p[2];
                    v[2].n     = t->n[2];
                    v[2].c     = C_GRAY;

                    pView->add_triangle(v);
                }
                ignored.flush();

                for (size_t i=0, m=matched.size(); i < m; ++i)
                {
                    v_triangle3d_t *t = matched.at(i);
                    v[0].p     = t->p[0];
                    v[0].n     = t->n[0];
                    v[0].c     = C_RED;

                    v[1].p     = t->p[1];
                    v[1].n     = t->n[1];
                    v[1].c     = C_GREEN;

                    v[2].p     = t->p[2];
                    v[2].n     = t->n[2];
                    v[2].c     = C_BLUE;

                    pView->add_triangle(v);
                }
                matched.flush();

//                    project_triangle(&out[0], &wf->s, &pl[3], &t.p[0]);
//                    vs.p[0]             = out[0];
//                    vs.p[1]             = out[1];
//                    view->add_segment(&vs);
//
//                    vs.p[0]             = out[1];
//                    vs.p[1]             = out[2];
//                    view->add_segment(&vs);
//
//                    vs.p[0]             = out[2];
//                    vs.p[1]             = out[0];
//                    view->add_segment(&vs);

                // Calc scissor planes' normals
                vector3d_t pl[4];
                calc_plane_vector_rv(&pl[0], &sFront.r[0], &sFront.r[1].v);
                calc_plane_vector_rv(&pl[1], &sFront.r[1], &sFront.r[2].v);
                calc_plane_vector_rv(&pl[2], &sFront.r[2], &sFront.r[0].v);
                calc_plane_vector_p3(&pl[3], &sFront.r[0].z, &sFront.r[1].z, &sFront.r[2].z);

                // Draw front
                v_ray3d_t r;
                s.c = C_MAGENTA;

                for (size_t i=0; i<3; ++i)
                {
                    // State
                    r.p = sFront.r[i].z;
                    r.v = sFront.r[i].v;
                    r.c = C_MAGENTA;
                    pView->add_ray(&r);

                    s.p[0] = sFront.s;
                    s.p[1] = sFront.r[i].z;
                    pView->add_segment(&s);

                    s.p[0] = sFront.r[(i+1)%3].z;
                    pView->add_segment(&s);

                    // Normals
                    r.p = sFront.r[i].z;
                    r.v = pl[i];
                    r.v.dw = 0.0f;
                    r.c = C_YELLOW;
                    pView->add_ray(&r);

                    r.p = sFront.r[(i+1)%3].z;
                    pView->add_ray(&r);

                    r.v = pl[3];
                    r.v.dw = 0.0f;
                    pView->add_ray(&r);
                }

                return res;
            }
    };

    MTEST_MAIN
    {
        const char *scene_file = (argc < 1) ? "res/test/3d/cross.obj" : argv[0];

        // Load scene
        Scene3D s;
        View3D v;
        status_t res = Model3DFile::load(&s, scene_file, true);
        MTEST_ASSERT_MSG(res == STATUS_OK, "Error loading scene from file %s", scene_file);

        // Initialize renderer
        Renderer r(&s, &v);
        MTEST_ASSERT_MSG(r.init() == STATUS_OK, "Error initializing renderer");
        r.run();
        r.destroy();

        // Destroy scene
        s.destroy();
    }

MTEST_END



