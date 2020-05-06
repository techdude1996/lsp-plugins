/*
 * mb_expander.h
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: sadko
 */

#ifndef PLUGINS_MB_EXPANDER_H_
#define PLUGINS_MB_EXPANDER_H_

#include <metadata/plugins.h>
#include <core/util/Bypass.h>
#include <core/util/Sidechain.h>
#include <core/util/Delay.h>
#include <core/util/MeterGraph.h>
#include <core/util/MeterGraph.h>
#include <core/util/Analyzer.h>
#include <core/dynamics/Expander.h>
#include <core/filters/DynamicFilters.h>
#include <core/filters/Equalizer.h>
#include <core/plugin.h>

namespace lsp
{
    class mb_expander_base: public plugin_t
    {
        protected:
            enum c_mode_t
            {
                MBEM_MONO,
                MBEM_STEREO,
                MBEM_LR,
                MBEM_MS
            };

            enum sync_t
            {
                S_COMP_CURVE    = 1 << 0,
                S_EQ_CURVE      = 1 << 1,

                S_ALL           = S_COMP_CURVE | S_EQ_CURVE
            };

            typedef struct exp_band_t
            {
                Sidechain       sSC;                // Sidechain module
                Equalizer       sEQ[2];             // Sidechain equalizers
                Expander        sExp;               // Expander
                Filter          sPassFilter;        // Passing filter for 'classic' mode
                Filter          sRejFilter;         // Rejection filter for 'classic' mode
                Filter          sAllFilter;         // All-pass filter for phase compensation
                Delay           sDelay;             // Delay for lookahead purpose

                float          *vTr;                // Transfer function
                float          *vVCA;               // Voltage-controlled amplification value for each band
                float           fScPreamp;          // Sidechain preamp

                float           fFreqStart;
                float           fFreqEnd;

                float           fFreqHCF;           // Cutoff frequency for low-pass filter
                float           fFreqLCF;           // Cutoff frequency for high-pass filter
                float           fMakeup;            // Makeup gain
                float           fEnvLevel;          // Envelope level
                float           fGainLevel;         // Gain adjustment level
                size_t          nLookahead;         // Lookahead amount

                bool            bEnabled;           // Enabled flag
                bool            bCustHCF;           // Custom frequency for high-cut filter
                bool            bCustLCF;           // Custom frequency for low-cut filter
                bool            bMute;              // Mute channel
                bool            bSolo;              // Solo channel
                bool            bExtSc;             // External sidechain
                size_t          nSync;              // Synchronize output data flags
                size_t          nFilterID;          // Identifier of the filter

                IPort          *pExtSc;             // External sidechain
                IPort          *pScSource;          // Sidechain source
                IPort          *pScMode;            // Sidechain mode
                IPort          *pScLook;            // Sidechain lookahead
                IPort          *pScReact;           // Sidechain reactivity
                IPort          *pScPreamp;          // Sidechain preamp
                IPort          *pScLpfOn;           // Sidechain low-pass on
                IPort          *pScHpfOn;           // Sidechain hi-pass on
                IPort          *pScLcfFreq;         // Sidechain low-cut frequency
                IPort          *pScHcfFreq;         // Sidechain hi-cut frequency
                IPort          *pScFreqChart;       // Sidechain band frequency chart

                IPort          *pMode;              // Mode of expander: upward/downward
                IPort          *pEnable;            // Enable expander
                IPort          *pSolo;              // Soloing
                IPort          *pMute;              // Muting
                IPort          *pAttLevel;          // Attack level
                IPort          *pAttTime;           // Attack time
                IPort          *pRelLevel;          // Release level
                IPort          *pRelTime;           // Release time
                IPort          *pRatio;             // Ratio
                IPort          *pKnee;              // Knee
                IPort          *pMakeup;            // Makeup gain
                IPort          *pFreqEnd;           // Frequency range end
                IPort          *pCurveGraph;        // Expander curve graph
                IPort          *pRelLevelOut;       // Release level out
                IPort          *pEnvLvl;            // Envelope level meter
                IPort          *pCurveLvl;          // Reduction curve level meter
                IPort          *pMeterGain;         // Reduction gain meter
            } exp_band_t;

            typedef struct split_t
            {
                bool            bEnabled;           // Split band is enabled
                float           fFreq;              // Split band frequency

                IPort          *pEnabled;           // Enable port
                IPort          *pFreq;              // Split frequency
            } split_t;

            typedef struct channel_t
            {
                Bypass          sBypass;            // Bypass
                Filter          sEnvBoost[2];       // Envelope boost filter
                Delay           sDelay;             // Delay for lookahead purpose

                exp_band_t      vBands[mb_expander_base_metadata::BANDS_MAX];       // Expander bands
                split_t         vSplit[mb_expander_base_metadata::BANDS_MAX-1];     // Split bands
                exp_band_t     *vPlan[mb_expander_base_metadata::BANDS_MAX];        // Execution plan (band indexes)
                size_t          nPlanSize;              // Plan size

                float          *vIn;                // Input data buffer
                float          *vOut;               // Output data buffer
                float          *vScIn;              // Sidechain data buffer (if present)

                float          *vBuffer;            // Common data processing buffer
                float          *vScBuffer;          // Sidechain buffer
                float          *vExtScBuffer;       // External sidechain buffer
                float          *vTr;                // Transfer function
                float          *vTrMem;             // Transfer buffer (memory)

                size_t          nAnInChannel;       // Analyzer channel used for input signal analysis
                size_t          nAnOutChannel;      // Analyzer channel used for output signal analysis
                bool            bInFft;             // Input signal FFT enabled
                bool            bOutFft;            // Output signal FFT enabled

                IPort          *pIn;                // Input
                IPort          *pOut;               // Output
                IPort          *pScIn;              // Sidechain
                IPort          *pFftIn;             // Pre-processing FFT analysis data
                IPort          *pFftInSw;           // Pre-processing FFT analysis control port
                IPort          *pFftOut;            // Post-processing FFT analysis data
                IPort          *pFftOutSw;          // Post-processing FFT analysis controlport
                IPort          *pAmpGraph;          // Expander's amplitude graph
                IPort          *pInLvl;             // Input level meter
                IPort          *pOutLvl;            // Output level meter
            } channel_t;

        protected:
            Analyzer        sAnalyzer;              // Analyzer
            DynamicFilters  sFilters;               // Dynamic filters for each band in 'modern' mode
            size_t          nMode;                  // Expander channel mode
            bool            bSidechain;             // External side chain
            bool            bEnvUpdate;             // Envelope filter update
            bool            bModern;                // Modern mode
            size_t          nEnvBoost;              // Envelope boost
            channel_t      *vChannels;              // Expander channels
            float           fInGain;                // Input gain
            float           fDryGain;               // Dry gain
            float           fWetGain;               // Wet gain
            float           fZoom;                  // Zoom
            uint8_t        *pData;                  // Aligned data pointer
            float          *vSc[2];                 // Sidechain signal data
            float          *vBuffer;                // Temporary buffer
            float          *vEnv;                   // Expander envelope buffer
            float          *vTr;                    // Transfer buffer
            float          *vPFc;                   // Pass filter characteristics buffer
            float          *vRFc;                   // Reject filter characteristics buffer
            float          *vFreqs;                 // Analyzer FFT frequencies
            float          *vCurve;                 // Curve
            uint32_t       *vIndexes;               // Analyzer FFT indexes
            float_buffer_t *pIDisplay;              // Inline display buffer

            IPort          *pBypass;                // Bypass port
            IPort          *pMode;                  // Global operating mode
            IPort          *pInGain;                // Input gain port
            IPort          *pOutGain;               // Output gain port
            IPort          *pDryGain;               // Dry gain port
            IPort          *pWetGain;               // Wet gain port
            IPort          *pReactivity;            // Reactivity
            IPort          *pShiftGain;             // Shift gain port
            IPort          *pZoom;                  // Zoom port
            IPort          *pEnvBoost;              // Envelope adjust

        protected:
            static bool compare_bands_for_sort(const exp_band_t *b1, const exp_band_t *b2);

        public:
            explicit mb_expander_base(const plugin_metadata_t &metadata, bool sc, size_t mode);
            virtual ~mb_expander_base();

        public:
            virtual void init(IWrapper *wrapper);
            virtual void destroy();

            virtual void update_settings();
            virtual void update_sample_rate(long sr);
            virtual void ui_activated();

            virtual void process(size_t samples);
            virtual bool inline_display(ICanvas *cv, size_t width, size_t height);
    };

    //-------------------------------------------------------------------------
    // Different expander implementations
    class mb_expander_mono: public mb_expander_base, public mb_expander_mono_metadata
    {
        public:
            mb_expander_mono();
    };

    class mb_expander_stereo: public mb_expander_base, public mb_expander_stereo_metadata
    {
        public:
            mb_expander_stereo();
    };

    class mb_expander_lr: public mb_expander_base, public mb_expander_lr_metadata
    {
        public:
            mb_expander_lr();
    };

    class mb_expander_ms: public mb_expander_base, public mb_expander_ms_metadata
    {
        public:
            mb_expander_ms();
    };

    class sc_mb_expander_mono: public mb_expander_base, public sc_mb_expander_mono_metadata
    {
        public:
            sc_mb_expander_mono();
    };

    class sc_mb_expander_stereo: public mb_expander_base, public sc_mb_expander_stereo_metadata
    {
        public:
            sc_mb_expander_stereo();
    };

    class sc_mb_expander_lr: public mb_expander_base, public sc_mb_expander_lr_metadata
    {
        public:
            sc_mb_expander_lr();
    };

    class sc_mb_expander_ms: public mb_expander_base, public sc_mb_expander_ms_metadata
    {
        public:
            sc_mb_expander_ms();
    };
}

#endif /* PLUGINS_MB_EXPANDER_H_ */
