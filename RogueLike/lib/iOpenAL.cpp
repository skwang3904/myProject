#include "iOpenAL.h"

#include "iStd.h"

static iOpenAL* al;

iOpenAL::iOpenAL(int audioNum)
{
	bufCount = audioNum;
	gBuffer = (ALuint*)malloc(sizeof(ALuint) * audioNum);
	gSource = (ALuint*)malloc(sizeof(ALuint) * audioNum);

	ALCdevice* newDevice = NULL;
	ALCcontext* newContext = NULL;
	ALenum error;

	newDevice = alcOpenDevice(NULL);
	if (newDevice)
	{
		const ALCint context_attribs[] = {ALC_FREQUENCY, 22050, 0};
		newContext = alcCreateContext(newDevice, context_attribs);
		if (newContext)
		{
			alcMakeContextCurrent(newContext);

			alGenBuffers(bufCount, gBuffer);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				printf("alGenBuffers %d\n", error);
				return;
			}

			alGenSources(bufCount, gSource);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				printf("alGenSources %d\n", error);
				return;
			}
		}
	}

	// clear any errors
	alGetError();

	bgm = 1.0f;
	sfx = 1.0f;
}

ALvoid* gStaticBufferData = NULL;

iOpenAL::~iOpenAL()
{
	ALCcontext* context = NULL;
	ALCdevice* device = NULL;

	ALCuint returnedNames[1024];// bufCount
	alDeleteBuffers(bufCount, returnedNames);
	alDeleteBuffers(bufCount, returnedNames);

	context = alcGetCurrentContext();
	device = alcGetContextsDevice(context);
	alcDestroyContext(context);
	alcCloseDevice(device);
	if (gStaticBufferData)
		free(gStaticBufferData);
}

typedef struct _WaveHeader0 {
	char riff[4];				// Chunk ID: "RIFF"
	unsigned int riffSize;		// Chunk size: 4+n
	char wave[4];				// WAVE ID: "WAVE"

	char fmt[4];				// Chunk ID: "fmt"
	unsigned int fmtSize;		// Chunk size: 16 or 18 or 40
	unsigned short format;		// Format code
	unsigned short channels;	// Number of interleaved channels
	unsigned int samplesPerSec;	// Sampling rate (blocks per second)
	unsigned int bytesPerSec;	// Data rate
	unsigned short blockAlign;	// Data block size(bytes)
	unsigned short bitsPerSample;// Bits per sample
} WaveHeader0;

typedef struct _WaveHeader1 {
	char data[4];				//'data'
	unsigned int dataSize;
} WaveHeader1;

void iOpenAL::initBuffer(int idx, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	int bufLen;
	char* buf = loadFile(szText, bufLen);

	WaveHeader0* head0 = (WaveHeader0*)buf;
	WaveHeader1* head1 = (WaveHeader1*)&buf[sizeof(WaveHeader0) + (head0->fmtSize - 16)];

	ALuint buffer = 0;
	ALuint format = 0;
	switch (head0->bitsPerSample) {
	case 8:	format = (head0->channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8);	break;
	case 16:format = (head0->channels == 1 ? AL_FORMAT_MONO16: AL_FORMAT_STEREO16); break;
	default:return;
	}

	ALsizei samplesPerSec = head0->samplesPerSec;

	char* data = &buf[sizeof(WaveHeader0) + (head0->fmtSize - 16) + sizeof(WaveHeader1)];

	//if (idx == 2 )
	//{
	//	head1->dataSize = 84000;
	//}
	//if (idx == 5)
	//{
	//	head1->dataSize = 700000;
	//}
	int dataLength = head1->dataSize;

	//printf("%d번째------------------- \n",idx );
	//printfAL(head0, head1);


	alBufferData(gBuffer[idx], format, data, dataLength, samplesPerSec);

	free(buf);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("initBuffer %s %d\n", szText, error);
}

void iOpenAL::initSource(int idx, bool repeat)
{
	ALenum error;
	error = alGetError();// clear the error

	alSourcei(gSource[idx], AL_LOOPING, repeat);

	const float sourcePosAL[] = { 0.0f, 0.0f, 0.0f };
	alSourcefv(gSource[idx], AL_POSITION, sourcePosAL);

	alSourcef(gSource[idx], AL_REFERENCE_DISTANCE, 50.0f);

	alSourcei(gSource[idx], AL_BUFFER, gBuffer[idx]);

	if ((error = alGetError()) != AL_NO_ERROR)
		printf("initSource %d\n", error);
}

void iOpenAL::play(int idx)
{
	alSourcePlay(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("play %d\n", error);
}

void iOpenAL::pause(int idx)
{
	alSourcePause(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("pause %d\n", error);
}

void iOpenAL::stop(int idx)
{
	alSourceStop(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("stop %d\n", error);
}

void iOpenAL::volume(int idx, float vol)
{
	alSourcef(gSource[idx], AL_GAIN, vol);
}

int prevBgm;
void loadAudio(AudioInfo* ai, int aiNum)
{
	al = new iOpenAL(aiNum);

	for (int i = 0; i < aiNum; i++)
	{
		al->initBuffer(i, ai[i].path);
		al->initSource(i, ai[i].repeat);
		al->volume(i, ai[i].volume);
	}

	prevBgm = -1;
}

void freeAudio()
{
	delete al;
}

void pauseAudio()
{
	for (int i = 0; i < al->bufCount; i++)
		al->pause(i);
}

void resumeAudio()
{
	if (prevBgm != -1)
		audioPlay(prevBgm);
}

void audioPlay(int sndIdx)
{
	al->play(sndIdx);

	ALint repeat;
	alGetSourcei(al->gSource[sndIdx], AL_LOOPING, &repeat);
	if (repeat)
		prevBgm = sndIdx;
}

void audioStop()
{
	for (int i = 0; i < al->bufCount; i++)
		al->stop(i);
}

void audioStop(int sndIdx)
{
	al->stop(sndIdx);
}

void audioVolume(float bgm, float sfx, int sfxNum)
{
	int i;
	float vol;
	for (i = 0; i < sfxNum; i++)
	{
		//alGetSourcef(al->gSource[i], AL_GAIN, &vol);
		//vol /= al->sfx;
		//al->volume(i, vol * sfx);

		al->volume(i, sfx);
	}
	al->sfx = sfx;

	for (i = sfxNum; i < al->bufCount; i++)
	{
		//alGetSourcef(al->gSource[i], AL_GAIN, &vol);
		//vol /= al->bgm;
		//al->volume(i, vol * bgm);

		al->volume(i, bgm);
	}
	al->bgm = bgm;
}

void audioGetVolume(float& bgm, float& sfx)
{
	bgm = al->bgm;
	sfx = al->sfx;
}