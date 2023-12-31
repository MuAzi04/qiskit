/*
	This is part of CFugue, a C++ Runtime for MIDI Score Programming
	Copyright (C) 2009 Gopalakrishna Palem

	For links to further information, or to contact the author,
	see <http://cfugue.sourceforge.net/>.

    $LastChangedDate: 2014-05-11 07:42:46 +0530 (Sun, 11 May 2014) $
    $Rev: 211 $
    $LastChangedBy: krishnapg $
*/

#ifndef MIDIRENDERER_H__9266AE56_84CB_4662_8328_ED088111CFE0__
#define MIDIRENDERER_H__9266AE56_84CB_4662_8328_ED088111CFE0__

#include "jdkmidi/manager.h"
#include "ParserListener.h"
#include "MidiEventManager.h"
#include "MidiTimer.h"

#ifndef MIDI_MAPPER
#define MIDI_MAPPER ((unsigned int)-1)
#endif // MIDI_MAPPER

namespace jdkmidi   { class MIDIDriverWin32; }
namespace CFugue    { class MIDIDriverAlsa; }

namespace CFugue
{
	///<Summary>Takes care of Rendering MIDI Output either to a file or to a MIDI out Port</Summary>
	class MIDIRenderer : MIDIEventManager, public CParserListener
	{
	    #if defined _WIN32
		jdkmidi::MIDIDriverWin32* m_pMIDIDriver;
		#else
		CFugue::MIDIDriverAlsa* m_pMIDIDriver;
		#endif

		jdkmidi::MIDIManager m_MIDIManager;

		long m_nSequenceTime;	// pseudo time tick to keep track of sequencer

		long m_lFirstNoteTime;	// time of first parallel note. Used for parallel notes

		/// <Summary>Event handler for Channel Pressure event Raised by Parser</Summary>
		virtual void OnChannelPressureEvent(const CParser* pParser, const ChannelPressure* pCP);

		/// <Summary>Event handler for Controller event Raised by Parser</Summary>
		virtual void OnControllerEvent(const CParser* pParser, const ControllerEvent* pConEvent);

		/// <Summary>Event handler for Instrument event Raised by Parser</Summary>
        virtual void OnInstrumentEvent(const CParser* pParser, const Instrument* pInstrument);

		/// <Summary>Event handler for KeySignature event Raised by Parser</Summary>
        virtual void OnKeySignatureEvent(const CParser* pParser, const KeySignature* pKeySig);

		/// <Summary>Event handler for Layer event Raised by Parser</Summary>
        virtual void OnLayerEvent(const CParser* pParser, const Layer* pLayer);

		/// <Summary>Event handler for PitchBend event Raised by Parser</Summary>
		virtual void OnPitchBendEvent(const CParser* pParser, const PitchBend* pPB);

		/// <Summary>Event handler for Polyphonic Pressure event Raised by Parser</Summary>
		virtual void OnPolyphonicPressureEvent(const CParser* pParser, const PolyphonicPressure* pPressure);

        /// <Summary>Event handler for Tempo event Raised by Parser </Summary>
        virtual void OnTempoEvent(const CParser* pParser, const Tempo* pTempo);

        /// <Summary>Event handler for Time event Raised by Parser </Summary>
        virtual void OnTimeEvent(const CParser* pParser, const Time* pTime);

		/// <Summary>Event handler for Voice event Raised by Parser</Summary>
        virtual void OnVoiceEvent(const CParser* pParser, const Voice* pVoice);

		/// <Summary>Event handler for Note event Raised by Parser</Summary>
		virtual void OnNoteEvent(const CParser* pParser, const Note* pNote);

	public:

		MIDIRenderer(void);

		~MIDIRenderer(void);

		/// <Summary>
		/// Clears the current state of the Renderer. Empties all the stored tracks and notes.
		/// </Summary>
		void Clear();

		/// <Summary>
		/// Starts Rendering the MIDI output to MIDI port.
		/// Each BeingPlayAsync call should have a matching EndPlayAsync call (no matter success or failure).
		/// @param nMIDIOutPortID is the ID of the MIDI output port to open for play.
		/// @param nTimerResolutionMS is the required minimum resolution for the MIDI timer (in MilliSeconds).
		/// @return false if Unable to open MIDI port or unable to create timer with specified resolution.
		/// </Summary>
		bool BeginPlayAsync(int nMIDIOutPortID = MIDI_MAPPER, unsigned int nTimerResolutionMS = 20);


		/// <Summary>
		/// Stops Rendering the MIDI output to MIDI port.
		/// Each BeingPlayAsync call should have a matching EndPlayAsync call.
		/// </Summary>
		void EndPlayAsync();

		/// <Summary>
		/// After starting MIDI play with BeginPlayAsync(), use WaitTillDone()
		/// to wait till the play is done. Caller gets blocked until completion.
		/// Returns immediately if no play is in progress.
		/// After WaitTillDone() returns, use EndPlayAsync() to release the resources.
		/// In case you do want to know the status without geting blocked,
		/// use IsPlaying() method.
		/// </Summary>
		void WaitTillDone();

		/// <Summary>
		/// Indicates if the MIDI output is still being rendered (to MIDI Port) or finished.
		/// </Summary>
		inline bool IsPlaying() const { return m_MIDIManager.IsSeqPlay(); }

        /// <Summary>
        /// Saves the current track/sequencer content to a MIDI Output file
        /// </Summary>
		bool SaveToFile(const char* szOutputFilePath); //TODO: Add the capatiblity to store custom MIDI Headers
	};

} // namespace CFugue

#endif // MIDIRENDERER_H__9266AE56_84CB_4662_8328_ED088111CFE0__
