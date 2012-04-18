/*!
** This sample demonstrates basic audio playing abilities.
** It takes file names as command line arguments and plays their audio streams one after the other.
** To demonstrate that the main loop is free to do something else, the sample displays elapsed time
** for the first file.
** This sample accepts audio streams from either audio or video files indifferently.
*/

#include <vector>
#include <yuni/yuni.h>
#include <yuni/core/string.h>
#include <yuni/core/system/suspend.h>
#include <yuni/audio/queueservice.h>

using namespace Yuni;


// Display the time in seconds using an MM:SS format
template<typename T>
static void writeTime(T timeInSeconds)
{
	std::cout << (timeInSeconds / 60) << ":";
	if (timeInSeconds % 60 < 10)
		std::cout << 0;
	std::cout << (timeInSeconds % 60);
}



int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <file1> [ <file2>, ... ]" << std::endl;
		return 1;
	}

	// The audio queue service is the center point for audio manipulation
	Audio::QueueService audio;
	audio.start();

	if (!audio.running())
		return 1;

	// Generate a name for the emitter ...
	String emitterName = "Emitter 1";
	// ... and create it
	if (!audio.emitter.add(emitterName))
	{
		std::cerr << "Emitter creation failed !" << std::endl;
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		// Load sound file
		if (!audio.bank.load(argv[i]))
		{
			std::cerr << "Failed to load \"" << argv[i] << "\"" << std::endl;
			// Play the next song rather than just quitting
			continue;
		}

		// Attach the emitter to the buffer
		audio.emitter.attach(emitterName, argv[i]);
		// Start playback on the emitter
		audio.emitter.play(emitterName);

		Yuni::SuspendMilliSeconds(1000);
		// Get stream duration
		unsigned int duration = audio.bank.duration(argv[i]);
		std::cout << "Sound duration: ";
		writeTime(duration);
		std::cout << std::endl;

		sint64 elapsed = 0;
		// Keep playing while at least one emitter is playing
		while (audio.playing())
		{
			// Every 100ms
			Yuni::SuspendMilliSeconds(100);

			// Get elapsed playback time
			sint64 newTime = audio.emitter.elapsedTime(emitterName);
			// Only update if different
			if (newTime != elapsed)
			{
				// Display elapsed time
				elapsed = newTime;
				std::cout << "Elapsed time: ";
				writeTime(elapsed);
				std::cout << std::endl;
			}
		}
		audio.emitter.detach(emitterName);
		//audio.bank.unload(argv[i]);
	}

	// Properly stop the queue service and unload everything
	audio.stop();
	return 0;
}

