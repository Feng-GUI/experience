
#include "FortniteFilter.h"
#include "plugin.h"

#include <memory>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <tchar.h>

// windows task
#include <Shlwapi.h>

// Windows AV run time stuff
#include <avrt.h>

//#include <sys/stat.h>

using namespace std;
using namespace cv;
//using namespace cv::dnn;

Plugin::FortniteFilter::FortniteFilter() {

	PLOG_INFO("FortniteFilter::const");

	memset(&filter, 0, sizeof(obs_source_info));
	filter.id = PLUGIN_ID;
	filter.type = OBS_SOURCE_TYPE_FILTER;
	filter.output_flags = OBS_SOURCE_VIDEO;

	filter.get_name = get_name;
	filter.create = create;
	filter.destroy = destroy;
	filter.video_render = Instance::video_render;
	filter.update = Instance::update;

	//filter.get_width = Instance::get_width;
	//filter.get_height = Instance::get_height;
	//filter.get_defaults = Instance::get_defaults;
	//filter.get_properties = Instance::get_properties;
	//filter.activate = Instance::activate;
	//filter.deactivate = Instance::deactivate;
	//filter.show = Instance::show;
	//filter.hide = Instance::hide;
	//filter.video_tick = Instance::video_tick;

	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	obs_register_source(&filter);
}

Plugin::FortniteFilter::~FortniteFilter() {

}

const char * Plugin::FortniteFilter::get_name(void *) {
	//Note: important to be different from PLUGIN_NAME
	return "Fortnite Justdance Filter";
}

void * Plugin::FortniteFilter::create(obs_data_t *data, obs_source_t *source) {
	
	PLOG_INFO("FortniteFilter::create");
	
	return new Instance(data, source);
}

void Plugin::FortniteFilter::destroy(void *ptr) {
	delete reinterpret_cast<Instance*>(ptr);
}

Plugin::FortniteFilter::Instance::Instance(obs_data_t *data, obs_source_t *source) :
	source(source) {

	PLOG_INFO("<%" PRIXPTR "> Instance Initializing...", this);

	counter = 0;
	interval = 600;

	obs_enter_graphics();

	// start with defaults.
	// width and height values are corrected again on render
	cxWidth = 1600;
	cyHeight = 900;

	render = gs_texrender_create(GS_RGBA, GS_ZS_NONE);
	obs_leave_graphics();

	check_size();
	reset_textures();

	// init open pose DNN network
	openpose_init();

	// initialize detection thread data
	PLOG_INFO("initialize detection thread data");
	{
		std::unique_lock<std::mutex> lock(detection.mutex);
		//detection.data = NULL;
		detection.data = (uint8_t*)bmalloc(sizeof(uint8_t) * cyHeight * cxWidth * 4);
		detection.shutdown = false;
		detection.ready = false;
		detection.thread = std::thread(StaticThreadMain, this);
	}

	this->update(data);

	PLOG_INFO("<%" PRIXPTR "> Instance Initialized.", this);
}

Plugin::FortniteFilter::Instance::~Instance() {
	PLOG_INFO("<%" PRIXPTR "> Finalizing...", this);

	// kill the thread
	PLOG_INFO("<%" PRIXPTR "> Stopping worker Threads...", this);
	{
		// block wait until main thread unlock detection.mutex
		//std::unique_lock<std::mutex> lock(detection.mutex);
		//detection.cv.wait(lock, [this] {return detection.ready; });
		//std::unique_lock<std::mutex> lock(detection.mutex);
		//detection.shutdown = true;
	}
	// wait for thread to die hang the program on shutdown. using detach instead of join.
	//detection.thread.join();
	PLOG_INFO("<%" PRIXPTR "> kill worker Thread.", this);
	detection.thread.detach();
	PLOG_INFO("<%" PRIXPTR "> Worker Thread stopped.", this);

	//if (taskHandle != NULL) {
	//	AvRevertMmThreadCharacteristics(taskHandle);
	//}

	PLOG_INFO("<%" PRIXPTR "> Finalized.", this);
}


void Plugin::FortniteFilter::Instance::update(void *ptr, obs_data_t *data) {
	if (ptr == NULL)
		return;
	return reinterpret_cast<Instance*>(ptr)->update(data);
}

void Plugin::FortniteFilter::Instance::update(obs_data_t *data) {
	UNUSED_PARAMETER(data);

	//TODO

}

uint32_t Plugin::FortniteFilter::Instance::OBSRenderer_MakeColor(uint8_t a, uint8_t b, uint8_t g,	uint8_t r) {
	return (((uint32_t)r << 24) & 0xFF000000) |
		(((uint32_t)g << 16) & 0x00FF0000) |
		(((uint32_t)b << 8) & 0x0000FF00) |
		(((uint32_t)a << 0) & 0x000000FF);
}

void Plugin::FortniteFilter::Instance::OBSRenderer_SetDrawColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
	gs_effect_t    *solid = obs_get_base_effect(OBS_EFFECT_SOLID);
	gs_eparam_t    *color = gs_effect_get_param_by_name(solid, "color");

	struct vec4 veccol;
	vec4_from_rgba(&veccol, OBSRenderer_MakeColor(red, green, blue, alpha));
	gs_effect_set_vec4(color, &veccol);
}

void Plugin::FortniteFilter::Instance::OBSRenderer_drawLine(int x1, int y1, int x2, int y2) {

	// make vb
	gs_render_start(true);
	// verts
	gs_vertex2f((float)x1, (float)y1);
	gs_vertex2f((float)x2, (float)y2);
	gs_vertbuffer_t *vertbuff = gs_render_save();

	while (gs_effect_loop(obs_get_base_effect(OBS_EFFECT_SOLID), "Solid")) {
		gs_load_vertexbuffer(vertbuff);
		gs_load_indexbuffer(NULL);
		gs_draw(GS_LINESTRIP, 0, 0);
	}
	gs_vertexbuffer_destroy(vertbuff);
}

void Plugin::FortniteFilter::Instance::OBSRenderer_drawLines(CvPoint2D32f pointsParam[], int start, int end, bool closed) {
	// make vb
	gs_render_start(true);
	// verts
	for (int i = start; i <= end; i++) {
		gs_vertex2f((float)pointsParam[i].x, (float)pointsParam[i].y);
	}
	if (closed) {
		gs_vertex2f((float)pointsParam[start].x, (float)pointsParam[start].y);
	}
	gs_vertbuffer_t *vertbuff = gs_render_save();

	while (gs_effect_loop(obs_get_base_effect(OBS_EFFECT_SOLID), "Solid")) {
		gs_load_vertexbuffer(vertbuff);
		gs_load_indexbuffer(NULL);
		gs_draw(GS_LINESTRIP, 0, 0);//orig GS_LINESTRIP
	}
	gs_vertexbuffer_destroy(vertbuff);
}

void Plugin::FortniteFilter::Instance::drawLine_thick(int x1, int y1, int x2, int y2)
{
	OBSRenderer_drawLine(x1 - 2, y1 - 2, x2 - 2, y2 - 2);
	OBSRenderer_drawLine(x1 - 1, y1 - 1, x2 - 1, y2 - 1);
	OBSRenderer_drawLine(x1, y1, x2, y2);
	OBSRenderer_drawLine(x1 + 1, y1 + 1, x2 + 1, y2 + 1);
	OBSRenderer_drawLine(x1 + 2, y1 + 2, x2 + 2, y2 + 2);

}
void Plugin::FortniteFilter::Instance::openpose_draw_skeleton() {

	PLOG_DEBUG("openpose_draw_skeleton");

	//TODO: PERF draw all lines at once to the vertex buffer
	OBSRenderer_SetDrawColor(255, 255, 0, 255);

	int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);
	for (int n = 0; n < nPairs; n++)
	{
		// lookup 2 connected body/hand parts
		CvPoint2D32f partA = points[POSE_PAIRS[n][0]];
		CvPoint2D32f partB = points[POSE_PAIRS[n][1]];

		if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
			continue;

		drawLine_thick((int)partA.x, (int)partA.y, (int)partB.x, (int)partB.y);
		//line(frame, partA, partB, Scalar(0, 255, 255), 8);
		//circle(frame, partA, 8, Scalar(0, 0, 255), -1);
		//circle(frame, partB, 8, Scalar(0, 0, 255), -1);
		//DrawCircleAtPos(10, partA.x, partA.y);
		//DrawCircleAtPos(10, partB.x, partB.y);
	}

}

void Plugin::FortniteFilter::Instance::DrawCircleAtPos(float radius, float x, float y)
{

	//TODO
	//TODO
}

void Plugin::FortniteFilter::Instance::reset_textures()
{
	PLOG_INFO("reset_textures");

	obs_enter_graphics();
	if (copy)
	{
		//PLOG_DEBUG("gs_stagesurface_destroy");
		gs_stagesurface_destroy(copy);
	}
	//PLOG_DEBUG("gs_stagesurface_create %i %i", cxWidth, cyHeight);
	copy = gs_stagesurface_create(cxWidth, cyHeight, GS_RGBA);
	obs_leave_graphics();

	PLOG_INFO("reset_textures finish");
}

int Plugin::FortniteFilter::Instance::openpose_init()
{
	std::string protoFile = "D:/fortnite/pose/dataset/mobilenet/graph_opt.pb"; //ORIG
	//std::string protoFile = "D:/fortnite/pose/dataset/mobilenet_thin/graph_freeze.pb";

	PLOG_INFO("openpose_init file %s", protoFile.c_str());

	// init DNN
	net = cv::dnn::readNetFromTensorflow(protoFile);

	return 0;
}

// reset points array results. not mandatory.
int Plugin::FortniteFilter::Instance::openpose_points_reset()
{
	for (int n = 0; n < POSE_POINTS; n++) {
		points[n].x = -1;
		points[n].y = -1;
	}

	return 0;
}

void Plugin::FortniteFilter::Instance::check_size()
{
	obs_source_t *target = obs_filter_get_target(source);

	target_valid = !!target;
	if (!target_valid)
	{
		PLOG_WARNING("check_size !target_valid 1");
		return;
	}

	uint32_t current_cx = obs_source_get_base_width(target);
	uint32_t current_cy = obs_source_get_base_height(target);
	PLOG_DEBUG("check_size current_cx current_cy %i %i", current_cx, current_cy);

	target_valid = !!current_cx && !!current_cy;
	if (!target_valid)
	{
		PLOG_WARNING("check_size !target_valid 2");
		return;
	}

	// update with height
	if (current_cx != cxWidth || current_cy != cyHeight) {
		PLOG_WARNING("updating width and height  %i %i", current_cx, current_cy);
		cxWidth = current_cx;
		cyHeight = current_cy;
		reset_textures();
		return;
	}
}

void Plugin::FortniteFilter::Instance::draw_render(gs_texrender_t *renderParam, uint32_t cxParam, uint32_t cyParam)
{
	gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	gs_texture_t *tex = gs_texrender_get_texture(renderParam);
	if (tex) {
		gs_eparam_t *image = gs_effect_get_param_by_name(effect, "image");
		gs_effect_set_texture(image, tex);

		while (gs_effect_loop(effect, "Draw"))
		{
			gs_draw_sprite(tex, 0, cxParam, cyParam);
		}
	}
}

void Plugin::FortniteFilter::Instance::video_render(void *ptr, gs_effect_t *effect) {
	if (ptr == NULL)
		return;
	reinterpret_cast<Instance*>(ptr)->video_render(effect);
}

void Plugin::FortniteFilter::Instance::video_render(gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	// filter begin
	if (!obs_source_process_filter_begin(source, GS_RGBA, OBS_ALLOW_DIRECT_RENDERING)) {
		PLOG_WARNING("video_render not ready");
		return;
	}

	// filter end
	gs_effect_t* defaultEffect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	obs_source_process_filter_end(source, defaultEffect, 0, 0);

	// reset counter every interval
	counter++;
	if (counter >= interval) {
		counter = 0;
	}
	// rendering every 5 frames.
	//if ((counter % 5) != 0) {
	//	obs_source_skip_video_filter(source);
	//	return;
	//}

	// ok to proceed with render
	PLOG_DEBUG("fortnite_source_render ok");

	// first, update dimensions
	// update the correct width and height
	check_size();

	//obs_source_t *source = source;
	//width = obs_source_get_width(source);
	//height = obs_source_get_height(source);
	//PLOG_INFO("fortnite_source_render dim: %i %i", width, height);

	PLOG_DEBUG("fortnite_source_render dim: cxWidth cyHeight %i %i", cxWidth, cyHeight);

	// defense
	if (cxWidth * cyHeight < 100) {
		//blog(LOG_WARNING, "fortnite_source_render dim are too small: cxWidth cyHeight %i %i", cxWidth, cyHeight);
		return;
	}

	obs_source_t *target = obs_filter_get_target(source);
	obs_source_t *parent = obs_filter_get_parent(source);

	gs_texrender_reset(render);
	gs_blend_state_push();
	gs_blend_function(GS_BLEND_ONE, GS_BLEND_ZERO);

	if (gs_texrender_begin(render, cxWidth, cyHeight)) {

		uint32_t parent_flags = obs_source_get_output_flags(target);
		bool custom_draw = (parent_flags & OBS_SOURCE_CUSTOM_DRAW) != 0;
		bool async = (parent_flags & OBS_SOURCE_ASYNC) != 0;

		struct vec4 clear_color;
		vec4_zero(&clear_color);
		gs_clear(GS_CLEAR_COLOR, &clear_color, 0.0f, 0);
		gs_ortho(0.0f, (float)cxWidth, 0.0f, (float)cyHeight, -100.0f, 100.0f);

		if (target == parent && !custom_draw && !async) {
			PLOG_INFO("fortnite_source_render obs_source_default_render");
			obs_source_default_render(target);
		}
		else
		{
			PLOG_DEBUG("fortnite_source_render obs_source_video_render");
			obs_source_video_render(target);
		}

		if ((counter % 2) == 0)
		{
			PLOG_INFO("<%" PRIXPTR "> counter %i", this, counter);

			gs_texture_t *tex = gs_texrender_get_texture(render);
			gs_stage_texture(copy, tex);

			if (gs_stagesurface_map(copy, &ptr, &linesize))
			{
				PLOG_DEBUG("map success linesize: %u", linesize);

				//
				// send frame to pose analysis
				//
				{
					std::lock_guard<std::mutex> lock(detection.mutex);
					detection.ready = true;
					PLOG_DEBUG("<%" PRIXPTR "> copy frame to processing. cyHeight %i linesize %i", this, cyHeight, linesize);

					//detection.data = (uint8_t*)bmalloc(sizeof(uint8_t) * cyHeight * linesize);
					//PLOG_INFO("<%" PRIXPTR "> memcpy.", this);
					memcpy(detection.data, ptr, sizeof(uint8_t) * cyHeight * linesize);

					// linesize is the most correct way to know the width 
					this->cxWidth = linesize / 4;
					this->cyHeight = cyHeight;

					// analyze frame on main thread
					//openpose_process_frame(cxWidth, cyHeight, ptr);

				} // lock block
				// notify worker thread
				detection.cv.notify_one();

				gs_stagesurface_unmap(copy);
				ptr = NULL;
			}
			else
			{
				PLOG_INFO("texture map failed %p", copy);
			}
		}

		//
		// draw pose analysis
		//
		openpose_draw_skeleton();

		gs_texrender_end(render);
	}
	gs_blend_state_pop();

	draw_render(render, cxWidth, cyHeight);
}

// process a frame
int Plugin::FortniteFilter::Instance::openpose_process_frame(int cols, int rows, unsigned char *data)
{
	//PLOG_INFO("<%" PRIXPTR "> openpose_process_frame - data <%X>", this, data);

	if (data == NULL) {
		PLOG_WARNING("openpose_process_frame data is null. return.");
		return 1;
	}

	int frameWidth = cols;
	int frameHeight = rows;

	int inWidth;
	int inHeight;
	double thresh = (double)0.01; //ORIG 0.01

	inWidth = frameWidth;
	inHeight = frameHeight;

	//MobileNet model was trained for 368x368 and heatmaps are 46x46 (368/8)

	// dynamic calculate of the scale factor to reach a fixed height 120
	double scaleFactor = (double)120 / (double)inHeight;

	PLOG_INFO("<%" PRIXPTR "> openpose_process_frame before create image from data  %i %i", this, inWidth, inHeight);
	//TODO: PERF: just set the data.
	frame = Mat(rows, cols, CV_8UC4, data);
	//imwrite("D:/frame.png", frame);//DEBUG

	// scale down - resize the input frame for faster performance
	inWidth = (int)((double)frameWidth * scaleFactor);
	inHeight = (int)((double)frameHeight * scaleFactor);
	PLOG_INFO("<%" PRIXPTR "> openpose_process_frame scale down image %i %i", this, inWidth, inHeight);
	// INTER_NEAREST is the fastest
	// INTER_LINEAR looks better than INTER_NEAREST, but slower.
	resize(frame, frameSmall, Size(), scaleFactor, scaleFactor, INTER_NEAREST);
	//imwrite("D:/frameSmall.png", frameSmall);//DEBUG

	// input a pre-scaled down smaller image.
	//RM: not sure that the blobFromImage support pre-processing scale down.
	Mat inpBlob = cv::dnn::blobFromImage(frameSmall, 1.0, Size(inWidth, inHeight), Scalar(127.5, 127.5, 127.5), /* BGR RGB */ false, false); //ORIG

	// performance: most of the time is spent here.
	net.setInput(inpBlob);
	Mat output1 = net.forward();

	// MobileNet output[1, 57, -1, -1], we only need the first 19 elements
	Mat output = output1(Range(0, 1), Range(0, 18)).clone();
	PLOG_INFO("<%" PRIXPTR "> dnn net after", this);

	//assert(len(BODY_PARTS) == out.shape[1])
	int H = output.size[2];
	int W = output.size[3];

	PLOG_DEBUG("find the position of the body parts");
	// find the position of the body parts
	openpose_points_reset();
	for (int n = 0; n < POSE_POINTS; n++)
	{
		// Probability map of corresponding body's part.
		Mat probMap(H, W, CV_32F, output.ptr(0, n));

		CvPoint2D32f p;
		p.x = -1; p.y = -1;
		Point maxLoc;
		double prob;
		minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
		if (prob > thresh)
		{
			p.x = (float)maxLoc.x;
			p.y = (float)maxLoc.y;
			p.x *= (float)frameWidth / W;
			p.y *= (float)frameHeight / H;

			//RM circle(frameCopy, cv::Point((int)p.x, (int)p.y), 8, Scalar(0,255,255), -1);
			//RM cv::putText(frameCopy, cv::format("%d", n), cv::Point((int)p.x, (int)p.y), cv::FONT_HERSHEY_COMPLEX, 1.1, cv::Scalar(0, 0, 255), 2);
		}
		else
		{
			p.x = -1; //RM
			p.y = -1; //RM
		}
		points[n] = p;
	}

	return 0;
}


int32_t Plugin::FortniteFilter::Instance::StaticThreadMain(Instance *ptr)
{
	return ptr->LocalThreadMain();
}

// detect pose in a thread
int32_t Plugin::FortniteFilter::Instance::LocalThreadMain()
{

	// thread run until shutdown
	std::chrono::time_point<std::chrono::steady_clock> lastTimestamp;
	while (true) {
		
		auto frameStart = std::chrono::system_clock::now();
		
		// detection
		{
			
			// if shutdown, exit while
			if (detection.shutdown) break;

			// block wait until main thread unlock detection.mutex
			std::unique_lock<std::mutex> lock(detection.mutex);
			detection.cv.wait(lock, [this] {return detection.ready; });

			openpose_process_frame(cxWidth, cyHeight, detection.data);

			// Manual unlocking is done before notifying, to avoid waking up
			// the waiting thread only to block again (see notify_one for details)
			detection.ready = false;
			lock.unlock();
			detection.cv.notify_one();
		}

		//
		// release resource
		//
		auto frameEnd = std::chrono::system_clock::now();
		auto elapsedMs = std::chrono::duration_cast<std::chrono::microseconds> (frameEnd - frameStart);
		//ORIG long long speedLimit = 24 * 1000;
		long long speedLimit = 100 * 1000;
		long long sleepTime = max(speedLimit - elapsedMs.count(),(long long)0);
		if (sleepTime > 0)
			std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
	
	}

	return 0;
}

