#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <AL/alext.h>
#include <AL/efx.h>
#include <AL/efx-presets.h>
#include "types.h"

#define AUDIONAME "../utils/beep.wav"

/* Effect object functions */
static LPALGENEFFECTS alGenEffects;
static LPALDELETEEFFECTS alDeleteEffects;
static LPALISEFFECT alIsEffect;
static LPALEFFECTI alEffecti;
static LPALEFFECTIV alEffectiv;
static LPALEFFECTF alEffectf;
static LPALEFFECTFV alEffectfv;
static LPALGETEFFECTI alGetEffecti;
static LPALGETEFFECTIV alGetEffectiv;
static LPALGETEFFECTF alGetEffectf;
static LPALGETEFFECTFV alGetEffectfv;

/* Auxiliary Effect Slot object functions */
static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;


ALvoid DisplayALError(char * text, ALint errorcode) {
    printf("%s%s", text, alGetString(errorcode));
}


void initReverb(EFXEAXREVERBPROPERTIES * reverb) {
    reverb - > flDensity = 1.0000 f;
    reverb - > flDiffusion = 1.0000 f;
    reverb - > flGain = 0.3162 f;
    reverb - > flGainHF = 0.8913 f;
    reverb - > flGainLF = 1.0000 f;
    reverb - > flDecayTime = 10.0 f;
    reverb - > flDecayHFRatio = 0.8300 f;
    reverb - > flDecayLFRatio = 1.0000 f;
    reverb - > flReflectionsGain = 0.0500 f;
    reverb - > flReflectionsDelay = 0.0070 f;
    reverb - > flReflectionsPan[0] = 0.0000 f;
    reverb - > flReflectionsPan[1] = 0.0000 f;
    reverb - > flReflectionsPan[2] = 0.0000 f;
    reverb - > flLateReverbGain = 1.2589 f;
    reverb - > flLateReverbDelay = 0.0110 f;
    reverb - > flLateReverbPan[0] = 0.0000 f;
    reverb - > flLateReverbPan[1] = 0.0000 f;
    reverb - > flLateReverbPan[2] = 0.0000 f;
    reverb - > flEchoTime = 0.2500 f;
    reverb - > flEchoDepth = 0.0000 f;
    reverb - > flModulationTime = 0.2500 f;
    reverb - > flModulationDepth = 0.0000 f;
    reverb - > flAirAbsorptionGainHF = 0.9943 f;
    reverb - > flHFReference = 5000.0000 f;
    reverb - > flLFReference = 250.0000 f;
    reverb - > flRoomRolloffFactor = 0.0000 f;
    reverb - > iDecayHFLimit = 0x1;
}


void initOpenAL(ALuint* source) {
    ALCenum error;

    ALCdevice device = alcOpenDevice(NULL);

    if (device) {
        ALCcontext context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
    }

    if (alcIsExtensionPresent(pDevice, "ALC_EXT_EFX") == AL_FALSE) {
        printf("Initialization failure : EFX Extension not found\n");
        return;
    }

    /* Retrieve the actual number of Aux Sends */
    /* available on each Source */
    alcGetIntegerv(pDevice, ALC_MAX_AUXILIARY_SENDS, 1, & iSends);


    /* Define a macro to help load the function pointers. */
#define LOAD_PROC(T, x)((x) = (T) alGetProcAddress(#x))
    LOAD_PROC(LPALGENEFFECTS, alGenEffects);
    LOAD_PROC(LPALDELETEEFFECTS, alDeleteEffects);
    LOAD_PROC(LPALISEFFECT, alIsEffect);
    LOAD_PROC(LPALEFFECTI, alEffecti);
    LOAD_PROC(LPALEFFECTIV, alEffectiv);
    LOAD_PROC(LPALEFFECTF, alEffectf);
    LOAD_PROC(LPALEFFECTFV, alEffectfv);
    LOAD_PROC(LPALGETEFFECTI, alGetEffecti);
    LOAD_PROC(LPALGETEFFECTIV, alGetEffectiv);
    LOAD_PROC(LPALGETEFFECTF, alGetEffectf);
    LOAD_PROC(LPALGETEFFECTFV, alGetEffectfv);

    LOAD_PROC(LPALGENAUXILIARYEFFECTSLOTS, alGenAuxiliaryEffectSlots);
    LOAD_PROC(LPALDELETEAUXILIARYEFFECTSLOTS, alDeleteAuxiliaryEffectSlots);
    LOAD_PROC(LPALISAUXILIARYEFFECTSLOT, alIsAuxiliaryEffectSlot);
    LOAD_PROC(LPALAUXILIARYEFFECTSLOTI, alAuxiliaryEffectSloti);
    LOAD_PROC(LPALAUXILIARYEFFECTSLOTIV, alAuxiliaryEffectSlotiv);
    LOAD_PROC(LPALAUXILIARYEFFECTSLOTF, alAuxiliaryEffectSlotf);
    LOAD_PROC(LPALAUXILIARYEFFECTSLOTFV, alAuxiliaryEffectSlotfv);
    LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTI, alGetAuxiliaryEffectSloti);
    LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTIV, alGetAuxiliaryEffectSlotiv);
    LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTF, alGetAuxiliaryEffectSlotf);
    LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTFV, alGetAuxiliaryEffectSlotfv);#
#undef LOAD_PROC

    alGetError();
    
    // On génère le buffer
    ALuint buffer = alutCreateBufferFromFile(AUDIONAME);

    if ((error = alGetError()) != AL_NO_ERROR) {
        DisplayALError("alGenBuffers :", error);
        return;
    }

    // Generate Sources
    alGenSources(1, source);

    if ((error = alGetError()) != AL_NO_ERROR) {
        DisplayALError("alGenSources 1 : ", error);
        return;
    }

    // On charge le buffer dans la source
    alSourcei(*source, AL_BUFFER, (ALint) buffer);

    // On définit l'orientation du listener
    ALfloat orientation[6] = { 0.0, 0.0, -1.0, 0.0, 0.0, 0.0 }; // Nord par défaut
    alListenerfv(AL_ORIENTATION, orientation);

    alSourcePlay(*source);
}


void loadEffectWithReverb(ALuint * effect,
    const EFXEAXREVERBPROPERTIES reverb) {
    ALCenum error;
    * effect = 0;

    // On crée l'objet effet
    alGenEffects(1, effect);

    // On s'assure que EAXREVERB est dispo et on applique les paramètres reverb
    if (alGetEnumValue("AL_EFFECT_EAXREVERB") != 0) {
        alEffecti( * effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
        alEffectf( * effect, AL_EAXREVERB_DENSITY, reverb.flDensity);
        alEffectf( * effect, AL_EAXREVERB_DIFFUSION, reverb.flDiffusion);
        alEffectf( * effect, AL_EAXREVERB_GAIN, reverb.flGain);
        alEffectf( * effect, AL_EAXREVERB_GAINHF, reverb.flGainHF);
        alEffectf( * effect, AL_EAXREVERB_GAINLF, reverb.flGainLF);
        alEffectf( * effect, AL_EAXREVERB_DECAY_TIME, reverb.flDecayTime);
        alEffectf( * effect, AL_EAXREVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
        alEffectf( * effect, AL_EAXREVERB_DECAY_LFRATIO, reverb.flDecayLFRatio);
        alEffectf( * effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
        alEffectf( * effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
        alEffectfv( * effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb.flReflectionsPan);
        alEffectf( * effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
        alEffectf( * effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
        alEffectfv( * effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb.flLateReverbPan);
        alEffectf( * effect, AL_EAXREVERB_ECHO_TIME, reverb.flEchoTime);
        alEffectf( * effect, AL_EAXREVERB_ECHO_DEPTH, reverb.flEchoDepth);
        alEffectf( * effect, AL_EAXREVERB_MODULATION_TIME, reverb.flModulationTime);
        alEffectf( * effect, AL_EAXREVERB_MODULATION_DEPTH, reverb.flModulationDepth);
        alEffectf( * effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
        alEffectf( * effect, AL_EAXREVERB_HFREFERENCE, reverb.flHFReference);
        alEffectf( * effect, AL_EAXREVERB_LFREFERENCE, reverb.flLFReference);
        alEffectf( * effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
        alEffecti( * effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);
    }

    // Gestion des erreurs
    error = alGetError();
    if (error != AL_NO_ERROR) {
        fprintf(stderr, "OpenAL error: %s\n", alGetString(error));
        if (alIsEffect( * effect))
            alDeleteEffects(1, effect);
        return;
    }
}


void playSourceWithReverb(ALuint source, EFXEAXREVERBPROPERTIES reverb) {
    ALuint effect, slot;
    ALenum state;

    // Position par rapport au joueur : (0, 0, 0) puisque c'est lui qui l'émet
    alSource3f(source, AL_POSITION, 0, 0, 0);

    // Charger le reverb dans un effet
    loadEffectWithReverb( & effect, reverb);

    // On crée le slot, qui va connecter l'effet et la source
    slot = 0;
    alGenAuxiliaryEffectSlots(1, & slot);

    // On affecte l'effet et la source au slot
    alAuxiliaryEffectSloti(slot, AL_EFFECTSLOT_EFFECT, (ALint) effect);
    alSource3i(source, AL_AUXILIARY_SEND_FILTER, (ALint) slot, 0, AL_FILTER_NULL);

    alSourcePlay(source);
    alGetSourcei(source, AL_SOURCE_STATE, & state);

    while (alGetError() == AL_NO_ERROR && state == AL_PLAYING)
        alGetSourcei(source, AL_SOURCE_STATE, & state);
}


void setOrientation(int value) {
    ALCenum error;
    ALfloat orientation[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    switch (value) {
        case 1:
            orientation[2] = -1.0 f;
            break;

        case 2:
            orientation[0] = -1.0 f;
            break;

        case 3:
            orientation[0] = 1.0 f;
            break;

        case 4:
            orientation[2] = 1.0 f;
            break;

        default:
            break;
    }

    alListenerfv(AL_ORIENTATION, orientation);

    error = alGetError();
    if (error != AL_NO_ERROR) {
        DisplayALError("alListenerfv POSITION : ", error);
        return;
    }
}

void closeOpenAL() {
    ALCcontext context = alcGetCurrentContext();
    ALCdevice device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}