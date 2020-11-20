/* Copyright (C) 2009 Alessandro Ghedini <al3xbio@gmail.com>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 */

#include "SoundServer.h"

#include <alsa/asoundlib.h>
#include <stdio.h>

#define PCM_DEVICE "default"

int SoundServer::play(const Sound &sound)
{
    unsigned int pcm, tmp;
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    char *buff;
    int buff_size;

    /* Open the PCM device in playback mode */
    if ((pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
                    SND_PCM_STREAM_PLAYBACK, 0) < 0))
        printf("ERROR: Can't open \"%s\" PCM device. %s\n",
                    PCM_DEVICE, snd_strerror(pcm));

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_alloca(&params);

    snd_pcm_hw_params_any(pcm_handle, params);

    /* Set parameters */
    if ((pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
                    SND_PCM_ACCESS_RW_INTERLEAVED) < 0))
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

    if ((pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
                        SND_PCM_FORMAT_S16_LE) < 0))
        printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

    if ((pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, sound.channels) < 0))
        printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

    unsigned int rate = sound.sampleRate;
    if ((pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0))
        printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

    /* Write parameters */
    if ((pcm = snd_pcm_hw_params(pcm_handle, params) < 0))
        printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

    snd_pcm_hw_params_get_channels(params, &tmp);

    snd_pcm_hw_params_get_rate(params, &tmp, 0);

    int samples = sound.dataSize / (sound.channels * (sound.bytesPerSecond / sound.sampleRate));

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(params, &frames, 0);

    buff_size = frames * sound.channels * (sound.bytesPerSecond / sound.sampleRate);
    buff = new char[buff_size];

    snd_pcm_hw_params_get_period_time(params, &tmp, NULL);


    for (int i = 0; i < samples; i += buff_size)
    {
        if ((pcm = snd_pcm_writei(pcm_handle, &sound.data[i], frames) == -EPIPE)) {
            printf("XRUN.\n");
            snd_pcm_prepare(pcm_handle);
        }
    }

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buff);

    return 0;
}
