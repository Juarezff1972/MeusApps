#include <iostream>
#include <process.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <dos.h>
#include <math.h>
#include <malloc.h>
#include "portaudio.h"
#include <unistd.h> // for usleep()

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

using namespace std;

typedef struct
{
    uint32_t total_count;
    uint32_t up_count;

    uint32_t counter;
    uint32_t prev_freq;
    uint32_t freq;
} paTestData;

int xmin, ymin, xmax, ymax, x, y, z, t, w, q, r, e, xx, yy, rr, graphdriver, graphmode;
unsigned char c;
int raio, angulo, tiro, disparo, angshot, explosao, pontos;
char far *canhao[40];
char far *alvo;

int x1, y1a, x2, y2;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback (const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
    paTestData *data = (paTestData*) userData;
    uint8_t *out = (uint8_t*) outputBuffer;
    unsigned long i;
    uint32_t freq = data->freq;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for (i = 0; i < framesPerBuffer; i++)
    {
        if (data->up_count > 0 && data->total_count == data->up_count)
        {
            *out++ = 0x00;
            continue;
        }

        data->total_count++;

        if (freq != data->prev_freq)
            data->counter = 0;

        if (freq)
        {
            int overflow_max = SAMPLE_RATE / freq;
            uint32_t data_cnt = data->counter % overflow_max;

            if (data_cnt > overflow_max / 2)
                *out++ = 0xff;
            else
                *out++ = 0x00;

            data->counter++;
        }
        else
        {
            data->counter = 0;
            *out++ = 0;
        }

        data->prev_freq = freq;
    }

    return paContinue;
}

static PaStream *stream;
static paTestData data;


void buzzer_set_freq (int frequency)
{
    data.up_count = 0; // do not stop!
    data.freq = frequency;
}

void buzzer_beep (int frequency, int msecs)
{
    data.total_count = 0;
    data.up_count = SAMPLE_RATE * msecs / 1000;
    data.freq = frequency;
}

int buzzer_start (void)
{
    PaStreamParameters outputParameters;
    PaError err;
    int i;
    err = Pa_Initialize();

    if (err != paNoError) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParameters.channelCount = 1;       /* stereo output */
    outputParameters.sampleFormat = paUInt8; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream (
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &data);

    if (err != paNoError) goto error;

    err = Pa_StartStream (stream);

    if (err != paNoError) goto error;

    return err;
error:
    Pa_Terminate();
    fprintf (stderr, "An error occured while using the portaudio stream\n");
    fprintf (stderr, "Error number: %d\n", err);
    fprintf (stderr, "Error message: %s\n", Pa_GetErrorText (err));
    return err;
}

int buzzer_stop()
{
    PaError err = 0;
    err = Pa_StopStream (stream);

    if (err != paNoError) goto error;

    err = Pa_CloseStream (stream);

    if (err != paNoError) goto error;

    Pa_Terminate();
    return err;
error:
    Pa_Terminate();
    fprintf (stderr, "An error occured while using the portaudio stream\n");
    fprintf (stderr, "Error number: %d\n", err);
    fprintf (stderr, "Error message: %s\n", Pa_GetErrorText (err));
    return err;
}
void msleep (int d)
{
    usleep (d * 1000);
}

int random (int seed)
{
    return rand() % (seed + 1);
}

int main()
{
    graphdriver = DETECT;
    //graphmode = CGAC1;
    //initgraph (&graphdriver, &graphmode, "");
    initwindow (800, 600, "Bola");
    xmax = getmaxx();
    ymax = getmaxy();
    //cout << xmax << " x " << ymax << "\n";
    raio = 12;
    tiro = 13;
    pontos = 0;
    explosao = 0;
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, xmax, ymax);
    int txtTam = 9;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    outtextxy (55, 40, "HUNTING");
    outtextxy (108, 40 + (txtTam * 10) + 10, "BALL");
    buzzer_start();
    buzzer_set_freq (261);
    msleep (250);
    buzzer_set_freq (293);
    msleep (250);
    buzzer_set_freq (329);
    msleep (250);
    buzzer_set_freq (349);
    msleep (250);
    buzzer_set_freq (392);
    msleep (250);
    buzzer_set_freq (440);
    msleep (250);
    buzzer_set_freq (494);
    msleep (250);
    buzzer_beep (523, 200);
    msleep (250);
    buzzer_stop();

    for (angulo = 15; angulo < 176; angulo = angulo + 4)
    {
        // 145, 169, 171, 197
        x1 = getmaxx() / 2 ;
        y1a = getmaxy() ;
        x2 = getmaxx() / 2 ;
        y2 = getmaxy() ;
        delay (50);
        setfillstyle (SOLID_FILL, 0);
        bar (x1 - 15, y1a - 31, x2 + 11, y2 - 2);
        setcolor (2);
        setfillstyle (SOLID_FILL, 2);
        bar (x1 - 10, y1a - 9, x2 + 10, y2 - 2);
        setcolor (3);
        arc (x1, y1a - 10, 0, 180, 4);
        line (x1 - 4, y1a - 10, x2 + 4, y2 - 10);
        setfillstyle (SOLID_FILL, 3);
        floodfill (x1, y1a - 11, 3);
        setlinestyle (SOLID_LINE, 0, THICK_WIDTH);
        setcolor (3);
        line (x1, y1a - 10, (x1 + (raio * cos (angulo * 3.1415926 / 180))), ( (y1a - 10) + (-raio * sin (angulo * 3.1415926 / 180)) / 1.1));
        canhao[ (angulo - 14) / 4] = (char far*) malloc ( (unsigned long) imagesize (x1 - 15, y1a - 31, x2 + 11, y2 - 2));
        getimage (x1 - 15, y1a - 31, x2 + 11, y2 - 2, canhao[ (angulo - 14) / 4]);
    }

    q = 1;
    w = 1;
    c = 0;
    x = 15;
    y = 15;
    pontos = 0;
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    setcolor (2);
    circle (x, y, 10);
    setfillstyle (SOLID_FILL, 2);
    floodfill (x, y, 2);
    alvo = (char far*) malloc ( (unsigned long) imagesize (x - 11, y - 11, x + 11, y + 11));
    getimage (x - 11, y - 11, x + 11, y + 11, alvo);
    xmax = getmaxx() - 1;
    xmin = 11;
    ymax = getmaxy() - 1;
    ymin = 11;

    while (kbhit() != 0)
    {
        c = getch();    /* Zera buffer do teclado */
    }

    while (kbhit() == 0)
    {
        putimage (x - 11, y - 11, alvo, OR_PUT);
        delay (10);
        putimage (x - 11, y - 11, alvo, COPY_PUT);
        x = x + q;
        y = y + w;

        if ( (x == xmax - 3) | (x == xmin + 3) | (getpixel (x + 11, y) == 3) | (getpixel (x - 11, y) == 3))
            q = q * (-1);

        if ( (y == ymax - 3) | (y == ymin + 3) | (getpixel (x, y + 10) == 3) | (getpixel (x, y - 10) == 3))
            w = w * (-1);
    }

comecar:
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    cleardevice();
    setcolor (3);
    rectangle (2, 2, getmaxx() - 1, getmaxy() - 1);

    while (kbhit() != 0)
        c = getch();

    q = 1;
    w = 1;
    c = 0;
    z = xmin;
    t = ymin;
    raio = 12;
    angulo = 90;
    tiro = 13;
    disparo = 0;
    //randomize();
    //cout << "Max X: " << getmaxx() << "\n";
    //cout << "Max Y: " << getmaxy() << "\n";
    x = random (getmaxx() - 30) + 10;
    y = random (getmaxy() - 30) + 10;

    for (rr = 1; rr < 1001; ++rr)
    {
        xx = (random (253) * 5) + 15;
        yy = (random (94) * 10) + 20;
        line (xx, yy, xx + 5, yy);
    }

    x1 = getmaxx() / 2 ;
    y1a = getmaxy() ;
    x2 = getmaxx() / 2 ;
    y2 = getmaxy() ;
    setcolor (2);
    putimage (x1 - 15, y1a - 31, canhao[ (angulo - 14) / 4], COPY_PUT);
    setcolor (0);
    putimage (x - 11, y - 11, alvo, COPY_PUT);
    buzzer_start();
    buzzer_set_freq (0);

    while (c != 27)
    {
        setcolor (2);
        putimage (x - 11, y - 11, alvo, OR_PUT);
        //gotoxy (2, 23);
        //printf ("%d\n", pontos);
        //cout << pontos << "\n";

        if (kbhit() != 0)
            c = getch();

        if (c == 27)
            goto fim;

        if (c == 32)
        {
            disparo = 1;
            tiro = 13;
        }

        if (disparo == 1)
        {
            setcolor (0);

            if (c == 32)
            {
                angshot = angulo;
                //Beep (440, 200);
                //buzzer_start();
                buzzer_beep (440, 200);
                //buzzer_stop();
                //sound(440);
                //delay(2);
                //nosound();
            }

            circle (z, t, 2);
            z = (x1 + (tiro * cos (angshot * 3.1415926 / 180)));
            t = ( (y1a - 10) + (-tiro * sin (angshot * 3.1415926 / 180)) / 1.1);
            tiro = tiro + 7;
            setcolor (15);
            circle (z, t, 2);
        }

        putimage (x1 - 15, y1a - 31, canhao[ (angulo - 14) / 4], COPY_PUT);

        if (c == 75)
            angulo = angulo + 4;

        if (c == 77)
            angulo = angulo - 4;

        if (angulo < 15)
            angulo = 15;

        if (angulo > 165)
            angulo = 165;

        putimage (x1 - 15, y1a - 31, canhao[ (angulo - 14) / 4], COPY_PUT);
        c = 0;
        delay (10);
        putimage (x - 11, y - 11, alvo, COPY_PUT);
        x = x + q;
        y = y + w;

        if ( (x == xmax - 3) | (x == xmin + 3) | (getpixel (x + 11, y) == 3) | (getpixel (x - 11, y) == 3))
            q = q * (-1);

        if ( (y == ymax - 3) | (y == ymin + 3) | (getpixel (x, y + 10) == 3) | (getpixel (x, y - 10) == 3))
            w = w * (-1);

        if ( (t <= ymin) | (z <= xmin) | (z >= xmax))
        {
            disparo = 0;
            tiro = 13;
            setcolor (0);
            circle (z, t, 1);
            z = (x1 + (tiro * cos (angulo * 3.1415926 / 180)));
            t = ( (y1a - 10) + (-tiro * sin (angulo * 3.1415926 / 180)) / 1.1);
            setcolor (3);
            rectangle (2, 2, getmaxx() - 1, getmaxy() - 1);
        };

        if (getpixel (z, t) == 2)
            c = 27;
    }

    buzzer_stop();
    buzzer_start();

    for (explosao = 0; explosao < 24; explosao = explosao + 2)
    {
        setcolor (9);
        circle (x, y, explosao);
        //Beep (70 * explosao, 200);
        buzzer_set_freq (70 * explosao);
        //buzzer_beep (70 * explosao, 2);
        //msleep (2);
        //sound(70*explosao);
        //delay(2);
        //nosound();
        delay (350);
    }

    for (explosao = 0; explosao < 24; explosao = explosao + 2)
    {
        setcolor (0);
        circle (x, y, explosao);
        //Beep (840 + 70 * explosao, 200);
        buzzer_set_freq (840 + 70 * explosao);
        //buzzer_beep (840 + 70 * explosao, 2);
        //msleep (2);
        //sound(840+70*explosao);
        //delay(2);
        //nosound();
        delay (350);
    }

    buzzer_stop();
    pontos = pontos + 1;
    goto comecar;
fim :
    closegraph();
    exit (pontos);
    return 0;
}
