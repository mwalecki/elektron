#include "ui.h"

#define WHOLE_NOTE		640
#define HALF_NOTE		WHOLE_NOTE/2
#define QUARTER_NOTE	WHOLE_NOTE/4
#define EIGHTH_NOTE		WHOLE_NOTE/8
#define SIXTEENTH_NOTE	WHOLE_NOTE/16

#define SND_POWER_UP \
	UI_SpeakerPlay(LS_noteG4, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteH4, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteD5, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteG5, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteH5, 4*EIGHTH_NOTE/5);\
	\
	UI_SpeakerPlay(LS_noteG4x, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteC5, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteD5x, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteG5x, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteC6, 4*EIGHTH_NOTE/5);\
	\
	UI_SpeakerPlay(LS_noteA4x, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteD5, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteF5, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteA5x, 4*EIGHTH_NOTE/5);\
	UI_SpeakerPlay(LS_noteD6, 4*EIGHTH_NOTE/5)

#define SND_COIN \
	UI_SpeakerPlay(LS_noteH5, EIGHTH_NOTE);\
	UI_SpeakerPlay(LS_noteE6, QUARTER_NOTE + EIGHTH_NOTE + HALF_NOTE)

#define SND_DEATH \
	UI_SpeakerPlay(LS_noteC5, SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_noteC5x, SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_noteD5, SIXTEENTH_NOTE + QUARTER_NOTE + HALF_NOTE);\
	\
	UI_SpeakerPlay(LS_noteH4, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteF5, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_PAUSE, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteF5, QUARTER_NOTE);\
	\
	UI_SpeakerPlay(LS_noteF5, SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_PAUSE, 4*SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_noteE5, SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_PAUSE, 4*SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_noteD5, SIXTEENTH_NOTE);\
	UI_SpeakerPlay(LS_PAUSE, 5*SIXTEENTH_NOTE);\
	\
	UI_SpeakerPlay(LS_noteC5, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteE4, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteG3, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteE4, QUARTER_NOTE);\
	\
	UI_SpeakerPlay(LS_noteC4, QUARTER_NOTE)

#define SND_DAMAGE \
	UI_SpeakerPlay(LS_noteA3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteE3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteA2, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_PAUSE, QUARTER_NOTE);\
	UI_SpeakerPlay(LS_noteA3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteE3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteA2, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_PAUSE, QUARTER_NOTE);\
	\
	UI_SpeakerPlay(LS_noteA3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteE3, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_noteA2, 2*EIGHTH_NOTE/3);\
	UI_SpeakerPlay(LS_PAUSE, QUARTER_NOTE)


