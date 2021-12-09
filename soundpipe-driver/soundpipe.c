#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#if defined(__APPLE__)
#import <Cocoa/Cocoa.h>
#endif

/* These are in charge of maintaining our sine function */
float sinPos;
float sinStep;

/* These are the audio card settings */
#define FREQ 44100
#define SAMPLES 8192 
#define TRUE 1

#define WAIT 0
#define WAITM 100000

/* This is basically an arbitrary number */
#define VOLUME 127.0

void populate(void* data, Uint8 *stream, int len) {
	int i=0;
	for (i=0; i<len; i++) {
		/* Just fill the stream with sine! */
		stream[i] = (Uint8) (VOLUME * sinf(sinPos))+127;
		sinPos += sinStep;
	}
}

int main() {
#if defined(__APPLE__)
	/* If on MacOS set soundpipe to not appear in the Dock */
	[NSApp setActivationPolicy: NSApplicationActivationPolicyAccessory];
#endif
	/* This will hold our data */
	SDL_AudioSpec spec;
	/* This will hold the requested frequency */
	long reqFreq = 440;
	/* This is the duration to hold the note for */
	float duration = 1.0;
	char freqstring[21];
	fd_set          input_set;
	struct timeval  timeout;
	int             ready_for_reading = 0;
	int             read_bytes = 0;

	/* Set up the requested settings */
	spec.freq = FREQ;
	spec.format = AUDIO_U8;
	spec.channels = 1;
	spec.samples = SAMPLES;
	spec.callback = (*populate);
	spec.userdata = NULL;

	/* Open the audio channel */
	if (SDL_OpenAudio(&spec, NULL) < 0) { 
		/* FAIL! */
		fprintf(stderr, "Failed to open audio: %s \n", SDL_GetError());
		exit(1); 
	} 

	while (TRUE) {
		char *ptr=NULL;
		/* Empty the FD Set */
		FD_ZERO(&input_set );
		/* Listen to the input descriptor */
		FD_SET(STDIN_FILENO, &input_set);
		/* Waiting for some seconds */
		timeout.tv_sec = WAIT;
		timeout.tv_usec = WAITM;
		// printf("Frequency: ");
		// fflush(stdout);
		/* Listening for input stream for any activity */
		ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
		/* Here, first parameter is number of FDs in the set, 
		 * second is our FD set for reading,
		 * third is the FD set in which any write activity needs to updated,
		 * which is not required in this case. 
		 * Fourth is timeout
		 */
		reqFreq=0;
		if (ready_for_reading == -1) {
			/* Some error has occured in input */
			printf("Unable to read your input\n");
			reqFreq=-1;
		} else {
			if (ready_for_reading) {
				if (fgets(freqstring, 20, stdin) == NULL) {
					strcpy(freqstring, "-1");
				} else {
					ptr=strchr(freqstring,'\n');
					if (ptr != NULL) {
						*ptr='\0';
					}
				}
				if (strlen(freqstring) == 0) {
					reqFreq=0;
				} else {
					reqFreq = strtol(freqstring, NULL, 10);
				}
			} else {
				reqFreq=0;
			}
		}

		// printf("Frequency=%d\n",reqFreq);
		/* Initialize the position of our sine wave */
		sinPos = 0;
		/* Calculate the step of our sin wave */
		sinStep = 2 * M_PI * reqFreq / FREQ;

		/* Now, run this thing */
		/* Then turn it off again */
		if (reqFreq <= 0) {
			SDL_PauseAudio(1);
		} else {
			SDL_PauseAudio(0);
		}

		/* Close audio channel */
		if (reqFreq < 0) {
			SDL_CloseAudio();
			exit(0);
		}
	}

}
