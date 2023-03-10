#include <cstdio>
#include <chrono>
#include <string>
#include <regex>
#include <Processing.NDI.Lib.h>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

int main(int argc, char* argv[])
{
	// Not required, but "correct" (see the SDK documentation).
	if (!NDIlib_initialize())
		return 0;

	// Create a finder
	NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
	if (!pNDI_find)
		return 0;

	// Wait until there is one source
	uint32_t no_sources = 0;
	const NDIlib_source_t* p_sources = NULL;
	std::string source_name = argv[1] ? argv[1] : "My Video";
	std::regex str_expr("[A-Za-z0-9\\-_\\.]* \\(([A-Za-z0-9\\-_]*)\\)");

	// Wait until the sources on the network have changed
	printf("Looking for sources ...\n");
	NDIlib_find_wait_for_sources(pNDI_find, 1000/* One second */);
	p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

	const NDIlib_source_t* req_source =NULL;

	for(int i = 0; i < no_sources; i++) {
		std::smatch sm;
		std::string ndi_name = std::string(p_sources[i].p_ndi_name);
		printf("p_sources->p_ndi_name=%s %d\n", ndi_name.c_str(), no_sources);
		std::regex_match(ndi_name, sm, str_expr);
		if(sm.size() == 0) {
			printf("No sources found\n");
			return 0;
		}
		printf("Found source %s\n", sm[1].str().c_str());
		if(sm[1].str() == source_name) {
			req_source = p_sources + i;
		}
	}

	if(req_source == NULL) {
		printf("Source \"%s\" not found\n", source_name.c_str());
		return 0;
	}

   printf("no sources=%u\n", no_sources);

	// We now have at least one source, so we create a receiver to look at it.
	NDIlib_recv_instance_t pNDI_recv = NDIlib_recv_create_v3();
	if (!pNDI_recv)
		return 0;

	// Connect to our sources
	NDIlib_recv_connect(pNDI_recv, req_source);

	// Destroy the NDI finder. We needed to have access to the pointers to p_sources[0]
	NDIlib_find_destroy(pNDI_find);

	// Run for one minute
	using namespace std::chrono;
	for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(5);) {
		// The descriptors
		NDIlib_video_frame_v2_t video_frame;
		NDIlib_audio_frame_v2_t audio_frame;

		switch (NDIlib_recv_capture_v2(pNDI_recv, &video_frame, &audio_frame, nullptr, 5000)) {
			// No data
			case NDIlib_frame_type_none:
				printf("No data received.\n");
				break;

				// Video data
			case NDIlib_frame_type_video:
				printf("Video data received (%dx%d).\n", video_frame.xres, video_frame.yres);
				NDIlib_recv_free_video_v2(pNDI_recv, &video_frame);
				break;

				// Audio data
			case NDIlib_frame_type_audio:
				printf("Audio data received (%d samples).\n", audio_frame.no_samples);
				NDIlib_recv_free_audio_v2(pNDI_recv, &audio_frame);
				break;
		}
	}

	// Destroy the receiver
	NDIlib_recv_destroy(pNDI_recv);

	// Not required, but nice
	NDIlib_destroy();

	// Finished
	return 0;
}
