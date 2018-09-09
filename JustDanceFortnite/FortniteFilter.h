#pragma once

#include "plugin.h"

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>

#include <opencv2/opencv.hpp>

/*
code example forked from https://github.com/quanhua92/human-pose-estimation-opencv/blob/master/openpose.py

const char BODY_PARTS =
{ "Nose": 0, "Neck" : 1, "RShoulder" : 2, "RElbow" : 3, "RWrist" : 4,
"LShoulder" : 5, "LElbow" : 6, "LWrist" : 7, "RHip" : 8, "RKnee" : 9,
"RAnkle" : 10, "LHip" : 11, "LKnee" : 12, "LAnkle" : 13, "REye" : 14,
"LEye" : 15, "REar" : 16, "LEar" : 17, "Background" : 18 };

POSE_PAIRS = [["Neck", "RShoulder"], ["Neck", "LShoulder"], ["RShoulder", "RElbow"],
["RElbow", "RWrist"], ["LShoulder", "LElbow"], ["LElbow", "LWrist"],
["Neck", "RHip"], ["RHip", "RKnee"], ["RKnee", "RAnkle"], ["Neck", "LHip"],
["LHip", "LKnee"], ["LKnee", "LAnkle"], ["Neck", "Nose"], ["Nose", "REye"],
["REye", "REar"], ["Nose", "LEye"], ["LEye", "LEar"]]
*/

const int POSE_PAIRS[17][2] =
{
	{ 1,2 },{ 1,5 },{ 2,3 },
	{ 3,4 },{ 5,6 },{ 6,7 },
	{ 1,8 },{ 8,9 },{ 9,10 },
	{ 1,11 },{ 11,12 },{ 12,13 },
	{ 1,0 },{ 0,14 },
	{ 14,16 },{ 0,15 },{ 15,17 }
};

const int POSE_POINTS = 18;

namespace Plugin {

	class FortniteFilter {
	public:
		FortniteFilter();
		~FortniteFilter();

		static const char *get_name(void *);
		static void *create(obs_data_t *, obs_source_t *);
		static void destroy(void *);

	private:
		obs_source_info filter;

		class Instance {
		public:
			Instance(obs_data_t *, obs_source_t *);
			~Instance();

			// OBS API
			//static uint32_t get_width(void *);
			//uint32_t get_width();
			//static uint32_t get_height(void *);
			//uint32_t get_height();
			//static void get_defaults(obs_data_t *);
			//static obs_properties_t *get_properties(void *);
			//void get_properties(obs_properties_t *pr);
			
			static void update(void *, obs_data_t *);
			void update(obs_data_t *);
			
			//static void activate(void *);
			//void activate();
			//static void deactivate(void *);
			//void deactivate();
			//static void show(void *);
			//void show();
			//static void hide(void *);
			//void hide();
			//static void video_tick(void *, float);
			//void video_tick(float);
			static void video_render(void *, gs_effect_t *);
			void video_render(gs_effect_t *);

		protected:

			// detection thread
			static int32_t StaticThreadMain(Instance*);
			int32_t LocalThreadMain();

			int openpose_init();
			int openpose_process_frame(int cols, int rows, unsigned char *data);
			int openpose_points_reset();

			void reset_textures();
			void check_size();
			void draw_render(gs_texrender_t *render, uint32_t cx, uint32_t cy);
			void openpose_draw_skeleton();

			// draw
			uint32_t OBSRenderer_MakeColor(uint8_t a, uint8_t b, uint8_t g, uint8_t r);
			void OBSRenderer_SetDrawColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
			void OBSRenderer_drawLine(int x1, int y1, int x2, int y2);
			void OBSRenderer_drawLines(CvPoint2D32f points[], int start, int end, bool closed);
			void drawLine_thick(int x1, int y1, int x2, int y2);
			void DrawCircleAtPos(float radius, float x, float y);

		private:

			cv::dnn::Net net;
			cv::Mat frame;
			cv::Mat frameSmall;

			CvPoint2D32f points[POSE_POINTS];

			obs_source_t *source = nullptr;
			gs_texrender_t *render = nullptr;
			gs_stagesurf_t *copy = nullptr;

			uint32_t cxWidth;
			uint32_t cyHeight;
			uint32_t linesize;
			uint8_t *ptr = nullptr;
			bool target_valid;

			uint32_t counter;
			uint32_t interval;

			//std::mutex mutex;

			// Detection
			struct ThreadData {
				std::thread thread;
				std::mutex mutex;
				std::condition_variable cv;
				bool ready = false;
				bool shutdown;
				uint8_t *data = NULL;

			} detection;
		};
	};
}
