//----------------------------------------------------------
// Guitar heronoid
//
// Yuval Tal, Tsachi Zigdon
// GarageGeeks
// October 2007
//
//----------------------------------------------------------

#define GH1
//#define GH2
//#define DO_FILE
//#undef USE_TIMER

#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_draw.h>

typedef int bool;
#define false 0
#define true 1

//-------------------------------------------
// Notes
//-------------------------------------------

#define NUM_NOTES 5

#define NOTE_GREEN_IDX  0
#define NOTE_RED_IDX    1
#define NOTE_YELLOW_IDX 2
#define NOTE_BLUE_IDX   3
#define NOTE_ORANGE_IDX 4

#define NOTE_GREEN      1
#define NOTE_RED        2
#define NOTE_YELLOW     4
#define NOTE_BLUE       8
#define NOTE_ORANGE     16

#define PIN_GREEN         16
#define PIN_RED           8
#define PIN_YELLOW        4
#define PIN_BLUE          2
#define PIN_ORANGE        1
#define PIN_STRUM         32

//-------------------------------------------
// Events
//-------------------------------------------

#define ACTION_STRUM              1
#define ACTION_FRETS              2
#define ACTION_RELEASE_ALL        3

typedef struct {
   int id;
   int action;
   Uint32 ticks;
   int frets;   // only set if action == STRUM_AND_FRETS
} gh_event;

//-------------------------------------------
// States
//-------------------------------------------

// Beginning of a new game
#define STATE_INIT	      0
// Wait for a new game
#define STATE_SYNC            1
// Calculate the speed of the lunch
#define STATE_CALIBRATE	      2
// Game playing
#define STATE_PLAY	      3

int state = STATE_INIT;

//---------------------------------------------------
// Run-time options (can be modified with key press)
//---------------------------------------------------
bool paused = false;
bool verbose = false;
bool pause_detect = false;
bool print_pushed_events = false;
bool highlight_found_objects = true;

//-------------------------------------------
// Other globals
//-------------------------------------------
Uint64    frame_counter;
int       fd;
int       event_counter;

//------------------------------------------------
// Notes to be found during gameplay
//------------------------------------------------

// Green line: SDL_Rect item[2] = { { 250, 260, 7, 5 } };
// Red line: SDL_Rect item[1] = { { 279, 260, 7, 5 } };
// Yellow line: SDL_Rect item[1] = { { 311, 260, 7, 5 } };

#ifdef GH2
SDL_Rect notes_rect[NUM_NOTES] = { { 250, 260, 7, 5 }, { 279, 260, 7, 5 }, { 311, 260, 7, 5 }, { 339, 260, 7, 5}, { 368, 260, 7, 5} };
#else
SDL_Rect notes_rect[NUM_NOTES] = { { 250, 278, 7, 6 }, { 280, 279, 7, 6 }, { 312, 278, 7, 6 }, { 339, 260, 7, 5}, { 368, 260, 7, 5} };
#endif
bool notes_found_in_previous_frame;

//----------------------------------------------
// Notes to be found at beginning of game
//----------------------------------------------

// green; red; yellow; blue; orange
#ifdef GH2
SDL_Rect sync_item[NUM_NOTES] = { {174, 411, 14, 6}, {239, 411, 14, 6}, {302, 411, 14, 6}, {365, 411, 14, 6}, {428, 411, 14, 6} };
#else
SDL_Rect sync_item[NUM_NOTES] = { {174, 425, 14, 6}, {239, 425, 14, 6}, {302, 425, 14, 6}, {365, 425, 14, 6}, {428, 425, 14, 6} };
#endif

bool sync_item_on[NUM_NOTES];

//-----------------------------------------------------
// Calibration lines to calculate speed of game
//-----------------------------------------------------
#ifdef GH2
SDL_Rect calibrate_rect = { 148, 429, 326, 1 };
#else
SDL_Rect calibrate_rect = { 159, 448, 312, 1 };
#endif

typedef struct {
   Uint32 ticks;   
   Uint64 frame;
} line_found_info;
int lines_found;
int total_lines_found;
line_found_info lines_found_info[2];

//-----------------------------------------------
// A heap (priorty queue) to store all the events
// that need to be send to the robot.
//-----------------------------------------------
#ifndef USE_TIMER

#define MAX_SIZE 100 

typedef struct {
  gh_event* packets[MAX_SIZE];
  unsigned int size;
} PacketHeap;

void heap_init(PacketHeap* h) 
{
   h->size=0;
}

void heap_heapify(PacketHeap* h,int i) 
{
	int l,r,smallest;
	gh_event* tmp;
	l=2*i; /*left child*/
	r=2*i+1; /*right child*/

	if ((l < h->size)&&(h->packets[l]->ticks < h->packets[i]->ticks))
		smallest=l;
	else 
		smallest=i;
	if ((r < h->size)&&(h->packets[r]->ticks < h->packets[smallest]->ticks))
		smallest=r;
	if (smallest!=i) {
		/*exchange to maintain heap property*/
		tmp=h->packets[smallest];
		h->packets[smallest]=h->packets[i];
		h->packets[i]=tmp;
		heap_heapify(h,smallest);
	}
}

void heap_addItem(PacketHeap* h,gh_event* packet) 
{
	unsigned int i,parent;	
	h->size=h->size+1;
	i=h->size-1;
	parent=i/2;
	/*find the correct place to insert*/
	while ((i > 0)&&(h->packets[parent]->ticks > packet->ticks)) {
		h->packets[i]=h->packets[parent];
		i=parent;
		parent=i/2;
	}
	h->packets[i]=packet;
}

gh_event* heap_extractMin(PacketHeap* h) 
{
	gh_event* max;
	if (heap_isEmpty(h))
		return 0;
	max=h->packets[0];
	h->packets[0]=h->packets[h->size-1];
	h->size=h->size-1;
	heap_heapify(h,0);
	return max;
}

int heap_isEmpty(PacketHeap *h) 
{
	return h->size==0;
}

int heap_isFull(PacketHeap *h) 
{
	return h->size>=MAX_SIZE;
}

// Priority queue
PacketHeap q;

// Next event that will be sent to the robot
gh_event* next_event;

#endif

// Write a character to the parallel port
void write_parallel_port(unsigned char data)
{
   ioctl(fd, PPWDATA, &data);
}

// Open the parallel port for writing
void open_parallel_port()
{
  // Open the port
  fd = open("/dev/parport0", O_WRONLY);
  if (fd == -1)
  {
     printf("Error opening /dev/parport0: %s\n", strerror(errno));
     exit(0);
  }

  // Claim access to the port
  if (ioctl(fd, PPCLAIM)) 
  {
     printf("Error claiming access to parallel port: %s\n", strerror(errno));
     exit(0);
  }
}

// Calculate the average color of a rectange in a surface
Uint32 average_color(SDL_Surface* surface, SDL_Rect* r)
{   
   // Point to the beginning of the rectangle in the surface
   unsigned char* p = (unsigned char*) surface->pixels + (r->y * surface->pitch) + (r->x * 3);
   
   unsigned long red, green, blue;
   int x, y;
  
   // Iterate over all the pixels 
   for (y = 0; y < r->h; y++)
   {
      for (x = 0; x < r->w; x++)
      {
         // If it's first time just take the values otherwise aggregate them
         if (x == 0 && y == 0)
         {
            red = (int) *p; p++;
            green = (int) *p; p++;
            blue = (int) *p; p++;
         }
         else
         {
            red = (red + *p); p++;
            green = (green + *p); p++;
            blue = (blue + *p); p++;
         }
      }
      
      // Move pointer to next line
      p += surface->pitch - (r->w * 3);
   }

   // Divide by number of pixels to calculate the average
   red = red / (r->w * r->h);
   green = green / (r->w * r->h);
   blue = blue / (r->w * r->h);
   
   return (blue << 16) | (green << 8) | red;
}

// Check whether the color is a GuitarHero green 
bool is_green(unsigned char r, unsigned char g, unsigned char b)
{
#ifdef GH2
   if (g < 100)
     return false;
     
   if (g - r < 30 || g - b < 30)
     return false;
#else
   if (g < 100) return false;

   if (g - r < 30 || g - b < 30)
     return false;
#endif
   if (verbose)
      printf("found green!!!\n");
     
   return true;
}

// Check whether the color is a GuitarHero red
bool is_red(unsigned char r, unsigned char g, unsigned char b)
{
#ifdef GH2
   if (r < 100)
     return false;

   if (r - g < 40 || r - b < 40)
     return false;
#else
   if (r < 90)
     return false;

   if (r - g < 40 || r - b < 40)
     return false;
#endif
     
   if (verbose)
      printf("found red!!!\n");
     
   return true;
}

// Check whether the color is a GuitarHero yellow
bool is_yellow(unsigned char r, unsigned char g, unsigned char b)
{
   if (r < 120 || g < 120)
     return false;

   if (r - b < 25 || g - b < 25)
     return false;
   
   if (verbose)
     printf("found yellow!!!\n");
     
   return true;
}

// Check whether the color is a GuitarHero blue
bool is_blue(unsigned char r, unsigned char g, unsigned char b)
{
   if (b < 130 || g < 90)
     return false;

   if (verbose)
     printf("found blue!!!\n");
    
   return true;
}

// Check whether the color is a GuitarHero orange
bool is_orange(unsigned char r, unsigned char g, unsigned char b)
{
   if (r < 120 || g < 70)
     return false;

   if (verbose)
     printf("found orange!!!\n");
     
   return true;
}

int color_dist(int r1, int g1, int b1, int r2, int g2, int b2)
{
   return (abs(r1-r2)+abs(g1-g2)+abs(b1-b2)) * 100 / (255*3);
}

Uint32 scan_rect(SDL_Surface* surface, SDL_Rect* rect, int r, int g, int b, int tolerance)
{   
   // Point to the beginning of the rectangle in the surface
   unsigned char* p = (unsigned char*) surface->pixels + (rect->y * surface->pitch) + (rect->x * 3);
   
   int x, y;
   int hit_count = 0;
  
   // Iterate over all the pixels 
   for (y = 0; y < rect->h; y++)
   {
      for (x = 0; x < rect->w; x++)
      {
         unsigned long red, green, blue;
         red = (int) *p; p++;
         green = (int) *p; p++;
         blue = (int) *p; p++;
         
         if (color_dist(red, green, blue, r, g, b) < tolerance) hit_count++;   
      }
      
      // Move pointer to next line
      p += surface->pitch - (rect->w * 3);
   }
   
   return hit_count;
}


// Send a command to the robot
void send_command(gh_event event)
{   
   unsigned char data = 0;
   
   if (event.action == ACTION_STRUM)
   {
      data = PIN_STRUM;
   }   

   if ((event.frets & NOTE_GREEN) == NOTE_GREEN)    data |= PIN_GREEN;
   if ((event.frets & NOTE_RED) == NOTE_RED)        data |= PIN_RED;
   if ((event.frets & NOTE_YELLOW) == NOTE_YELLOW)  data |= PIN_YELLOW;
   if ((event.frets & NOTE_BLUE) == NOTE_BLUE)      data |= PIN_BLUE;
   if ((event.frets & NOTE_ORANGE) == NOTE_ORANGE)  data |= PIN_ORANGE;   

   write_parallel_port(data);
}

// Callback which is called whenever a timer expired and an 
// event needs to be sent to the robot 
Uint32 timer_callback(Uint32 interval, void *param)
{
   gh_event* event = (gh_event*) param;

   // Send the event to the robot
   send_command(*event);
  
   // Print debug information about it
   if (print_pushed_events)
   {
     if (event->action == ACTION_STRUM)
     {
        printf("%lu Do event #%d: STRUM_ONLY\n", SDL_GetTicks(), event->id);
     }
     else if (event->action == ACTION_FRETS)
     {
        printf("%lu Do event #%d: STRUM_AND_FRETS for note(s): ", SDL_GetTicks(), event->id);
        if ((event->frets & NOTE_GREEN) == NOTE_GREEN) printf("green ");
        if ((event->frets & NOTE_RED) == NOTE_RED) printf("red ");
        if ((event->frets & NOTE_YELLOW) == NOTE_YELLOW) printf("yellow ");
        if ((event->frets & NOTE_BLUE) == NOTE_BLUE) printf("blue ");
        if ((event->frets & NOTE_ORANGE) == NOTE_ORANGE) printf("orange ");
        printf("\n");
     }
     else if (event->action == ACTION_RELEASE_ALL)
     {
        printf("%lu Do event #%d: RELEASE_FRETS\n", SDL_GetTicks(), event->id);
     }
   }
   
   free(event);
   
   return 0;
}

#define TIME_BEFORE 170
#define TIME_AFTER 20
#define LATENCY 0


// You really got me
//Uint32 lines_delay = 200;
//#define LINES 5.0f

// Message in a bottle
//#define LINES 5.0f   

// Heart shaped box
//#define LINES 4.0f      

// Gonna rock ya
//#define LINES 7.0f     

// Infected
Uint32 lines_delay = 230;
#define LINES 4.0f

#ifndef USE_TIMER
#define SDL_AddTimer(a,b,c) My_AddTimer(c)

void My_AddTimer(gh_event* event)
{
   event->ticks += SDL_GetTicks();

   if (print_pushed_events)
   {
     if (event->action == ACTION_STRUM)
        printf("Push event #%d: at %lu STRUM_ONLY\n", event->id, event->ticks);
     else if (event->action == ACTION_FRETS)
     {
        printf("Push event #%d: at %lu STRUM_AND_FRETS for note(s): ", event->id, event->ticks);
        if ((event->frets & NOTE_GREEN) == NOTE_GREEN) printf("green ");
        if ((event->frets & NOTE_RED) == NOTE_RED) printf("red ");
        if ((event->frets & NOTE_YELLOW) == NOTE_YELLOW) printf("yellow ");
        if ((event->frets & NOTE_BLUE) == NOTE_BLUE) printf("blue ");
        if ((event->frets & NOTE_ORANGE) == NOTE_ORANGE) printf("orange ");
        printf("\n");
     }
     else if (event->action == ACTION_RELEASE_ALL)
        printf("Push event #%d: at %lu RELEASE_FRETS\n", event->id, event->ticks);
   }
   
   heap_addItem(&q, event);
}

#endif

void push_event(gh_event event)
{
   gh_event* param = malloc(sizeof(gh_event));
   memcpy(param, &event, sizeof(gh_event)); 
   
   if (param->action == ACTION_FRETS)
   {  
      // Frets 50ms before
      param->ticks = (Uint32) (LINES * lines_delay) - TIME_BEFORE - LATENCY;
      param->id = event_counter;
      SDL_AddTimer(param->ticks, timer_callback, param);
      
      // Strum on time    
      gh_event* strum_param = malloc(sizeof(gh_event));
      strum_param->id = event_counter;
      strum_param->action = ACTION_STRUM;
      strum_param->frets = param->frets;
      strum_param->ticks = (Uint32) (LINES * lines_delay) - LATENCY;      
      SDL_AddTimer(strum_param->ticks, timer_callback, strum_param);
   }
   else if (event.action == ACTION_RELEASE_ALL)
   {
      param->id = event_counter;   
      param->ticks = (Uint32) (LINES * lines_delay) + TIME_AFTER - LATENCY;      
      SDL_AddTimer(param->ticks, timer_callback, param);
   }
   
   event_counter++;
}     

void process_video(SDL_Surface* surface, int width, int height)
{
   Uint32 c;
   int red, green, blue;        
   int notes_found = 0;
   int i;
   
   for (i = 0; i < NUM_NOTES; i++)
   {   
      if (i == 0 && scan_rect(surface, &notes_rect[i], 0x11, 0x6c, 0x19, 7) > 0)
      {
		   notes_found |= NOTE_GREEN;
         if (highlight_found_objects)
            Draw_FillRect(surface, notes_rect[i].x, notes_rect[i].y, notes_rect[i].w, notes_rect[i].h, 0x00ffffffL);
      }

      if (i == 1 && scan_rect(surface, &notes_rect[i], 0x84, 0x10, 0x1c, 7) > 0)
      {
		   notes_found |= NOTE_RED;
         if (highlight_found_objects)
            Draw_FillRect(surface, notes_rect[i].x, notes_rect[i].y, notes_rect[i].w, notes_rect[i].h, 0x00ffffffL);
      }      
     
      if (i == 2 && scan_rect(surface, &notes_rect[i], 0x8a, 0x84, 0x00, 10) > 0)
      {
		   notes_found |= NOTE_YELLOW;
         if (highlight_found_objects)
            Draw_FillRect(surface, notes_rect[i].x, notes_rect[i].y, notes_rect[i].w, notes_rect[i].h, 0x00ffffffL);
      }
         
   }

/*
   if (notes_found != 0 && !notes_found_in_previous_frame)
      printf("#%d: Found new something @ %lu\n", event_counter, SDL_GetTicks());
*/

   // If a note found but not found in previous frame, it means we found a circle.
   // Send a FRET & STRUM event.
   if (notes_found && !notes_found_in_previous_frame)
   {
      gh_event event;
      event.action = ACTION_FRETS;
      event.frets = notes_found;
      event.ticks = SDL_GetTicks();      
      push_event(event);     
   }
   // If no note found but found in previous frame, it means we were on a line or circle.
   // Send an event to release the frets
   else if (!notes_found && notes_found_in_previous_frame)
   {
      gh_event event;
      event.action = ACTION_RELEASE_ALL;
      event.ticks = SDL_GetTicks();
      event.frets = 0;
      push_event(event);     
   }
   
   notes_found_in_previous_frame = (notes_found != 0);
}

void process_video_sync(SDL_Surface* surface, SDL_Rect* polled_areas, int num_of_areas, int width, int height)
{
   Uint32 c;
   int red, green, blue;
   int i;
        
   for (i = 0; i < num_of_areas; i++)
   { 
      c = average_color(surface, &polled_areas[i]);
      if (highlight_found_objects)
         Draw_FillRect(surface, polled_areas[i].x, polled_areas[i].y, polled_areas[i].w, polled_areas[i].h, c);
      
      red = c & 0x000000ff;
      green = (c & 0x0000ff00) >> 8;
      blue = (c & 0x00ff0000) >> 16;

      if (verbose)
         printf("%d out of %d: r=%lu,g=%lu,b=%lu\n", i, num_of_areas, red, green, blue);
			
		// green->in cell 0, red->1, yellow->2, blue->3, orange->4
		if (i == NOTE_GREEN_IDX && is_green(red, green, blue)) 
		{
			sync_item_on[NOTE_GREEN_IDX] = true;
		}
		
		if (i == NOTE_RED_IDX && is_red(red, green, blue)) 
		{
			sync_item_on[NOTE_RED_IDX] = true;
		}
		
		if (i == NOTE_YELLOW_IDX && is_yellow(red, green, blue)) 
		{
			sync_item_on[NOTE_YELLOW_IDX] = true;
		}
			
		if (sync_item_on[NOTE_GREEN_IDX] && sync_item_on[NOTE_RED_IDX] && sync_item_on[NOTE_YELLOW_IDX])
		{
			printf("==================> GAME START DETECTED\n");
			//state = STATE_CALIBRATE;
			state = STATE_PLAY;
			if (pause_detect)
			   paused = true;
		}
   }
} 

void process_video_calibrate(SDL_Surface* surface, int width, int height)
{
   Uint32 c;
   int red, green, blue;
   SDL_Rect temp_rect = { calibrate_rect.x, calibrate_rect.y, calibrate_rect.w, calibrate_rect.h };
   bool line_found = false;
   int i = 0;
   
   while (!line_found && i < 15)
   { 
      c = average_color(surface, &temp_rect);
      if (highlight_found_objects)
         Draw_FillRect(surface, temp_rect.x, temp_rect.y, temp_rect.w, temp_rect.h, c);
      
      red = c & 0x000000ff;
      green = (c & 0x0000ff00) >> 8;
      blue = (c & 0x00ff0000) >> 16;

      if (verbose)
         printf("Avg col calibrate: r=%lu,g=%lu,b=%lu\n", red, green, blue);
         
      if (red > 90 && green > 90 && blue > 90)
      {
         line_found = true;
         
         if (verbose)
            printf("Found line at frame %lu!\n", frame_counter);
         
         // Do not count a line that was found in two consecutive frames 
         if (lines_found == 1 && frame_counter - lines_found_info[0].frame <= 1)
            continue;
            
         if (highlight_found_objects)
            Draw_FillRect(surface, calibrate_rect.x, calibrate_rect.y, calibrate_rect.w, 15, 0x00ffffffL);
         lines_found_info[lines_found].ticks = SDL_GetTicks();
         lines_found_info[lines_found].frame = frame_counter;
         lines_found++;
         
         // We only need two lines to calculate the delta
         if (lines_found == 2)
         {
            int current_delay = lines_found_info[1].ticks - lines_found_info[0].ticks;
            
            if (lines_delay == 0)
               lines_delay = current_delay;
            else
               lines_delay = (lines_delay + current_delay) / 2;
               
            printf("==================> TWO LINES DETECTED\n");
            printf("Calculated delay: %lu ms\n", lines_delay);            
            
            lines_found_info[0] = lines_found_info[1];
            lines_found = 1;
            total_lines_found++;
            
            if (pause_detect)
               paused = true;
               
            if (total_lines_found == 4)
               state = STATE_PLAY;               
         }
      }
      
      // Move to the next line
      temp_rect.y++;
      i++;
   }
} 

void init_state()
{
   int i;
  	for (i = 0; i < NUM_NOTES; i++) 
  	   sync_item_on[i] = false;
  	   
   lines_found = 0;  	   
   total_lines_found = 0;  	   
   frame_counter = 0;
   notes_found_in_previous_frame = false;
   event_counter = 0;
            
   write_parallel_port(0);

#ifndef USE_TIMER   
   heap_init(&q);
#endif
   
   printf("==================> WAITING FOR NEW GAME\n");
}

int main(int argc, char *argv[]) 
{
  AVFormatContext *pFormatCtx;
  int             i, videoStream;
  AVCodecContext  *pCodecCtx;
  AVCodec         *pCodec;
  AVFrame         *pFrame; 
  AVFrame         *pFrameRGB; 
  AVPacket        packet;
  int             frameFinished;
  float           aspect_ratio;

  int 			numBytes;
  uint8_t		*buffer;
  bool         running = 1;
  bool         one_frame = 0;

  SDL_Rect        rect;
  SDL_Event       event;

  // Register all formats and codecs
  av_register_all();
  
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
    fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
    exit(1);
  }

  open_parallel_port();
  
  // Open video file
#ifdef DO_FILE  
  if(av_open_input_file(&pFormatCtx, "x" /* argv[1] */, NULL, 0, NULL)!=0)
    return -1; // Couldn't open file
#else
  AVFormatParameters formatParams;
  AVInputFormat *iformat;
  int rv;
  char filename[255];
  
  formatParams.device = "/dev/video0";  
  formatParams.standard = "pal";
  formatParams.width = 640;
  formatParams.height = 480;
  formatParams.time_base.den = 29;
  formatParams.time_base.num = 1;
  formatParams.pix_fmt = PIX_FMT_YUYV422;
  strcpy(filename, "/dev/video0");
  iformat = av_find_input_format("video4linux2");
  
  if ((rv = av_open_input_file(&pFormatCtx,
                 filename, iformat, 0, &formatParams)) != 0)
  {
      fprintf (stderr, "cannot open file: %s\n", strerror (AVERROR (rv)));
      return -1;
  }
#endif
  
  // Retrieve stream information
  if(av_find_stream_info(pFormatCtx)<0)
    return -1; // Couldn't find stream information
  
  // Dump information about file onto standard error
  dump_format(pFormatCtx, 0, argv[1], 0);
  
  // Find the first video stream
  videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++)
    if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  if(videoStream==-1)
    return -1; // Didn't find a video stream
  
  // Get a pointer to the codec context for the video stream
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;
  
  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  
  // Open codec
  if(avcodec_open(pCodecCtx, pCodec)<0)
    return -1; // Could not open codec
  
  // Allocate video frame
  pFrame=avcodec_alloc_frame();

  // Allocate an AVFrame structure
  pFrameRGB=avcodec_alloc_frame();
  if(pFrameRGB==NULL)
    return -1;

  // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
			      pCodecCtx->height);
  buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
		 pCodecCtx->width, pCodecCtx->height);

  // Make a screen to put our video
  SDL_Surface* screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);
  printf("Opening window: %dx%d\n", pCodecCtx->width, pCodecCtx->height);
  if(!screen) {
    fprintf(stderr, "SDL: could not set video mode - exiting\n");
    exit(1);
  }

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  // Read frames and save first five frames to disk
  while(running) 
  {
    if (state == STATE_INIT)
    {
       init_state();
       state = STATE_SYNC;
    }
    else if ((paused == 0 || one_frame == 1) && av_read_frame(pFormatCtx, &packet)>=0) 
    {
      // Is this a packet from the video stream?
      if (packet.stream_index==videoStream) 
      {
	      // Decode video frame
         avcodec_decode_video(pCodecCtx, pFrame, &frameFinished, 
							   packet.data, packet.size);
				      
			// Did we get a video frame?
			if (frameFinished) 
			{
			   frame_counter++;
			   
				// Convert the image into RGB format that SDL uses
			   img_convert((AVPicture *) pFrameRGB, PIX_FMT_RGB24,
	                      (AVPicture *)pFrame, pCodecCtx->pix_fmt, 
						       pCodecCtx->width, pCodecCtx->height);
				
				
				SDL_Surface* src = SDL_CreateRGBSurfaceFrom(buffer, pCodecCtx->width, pCodecCtx->height, 24, pCodecCtx->width * 3,  0x000000ffL, 0x0000ff00L, 0x00ff0000L, 0);
				          // Yuval's test          
				          // process_video(src, item, 3, pCodecCtx->width, pCodecCtx->height);
				          // identify start
				if (state == STATE_SYNC)
				   process_video_sync(src, sync_item, 3, pCodecCtx->width, pCodecCtx->height);
				else if (state == STATE_CALIBRATE)
				   process_video_calibrate(src, pCodecCtx->width, pCodecCtx->height);				
				else if (state == STATE_PLAY)
				   process_video(src, pCodecCtx->width, pCodecCtx->height);	
				SDL_BlitSurface(src, NULL, screen, NULL);
            SDL_UpdateRect(screen, 0, 0, pCodecCtx->width, pCodecCtx->height);
				SDL_FreeSurface(src);
				
				one_frame = 0;
				
         }
		}
				
		// Free the packet that was allocated by av_read_frame
	   av_free_packet(&packet);
	}

#ifndef USE_TIMER
   if (!heap_isEmpty(&q) && next_event == NULL)
      next_event = heap_extractMin(&q);

   if (next_event != NULL && SDL_GetTicks() >= next_event->ticks)
   {
      timer_callback(0, next_event);
      next_event = NULL;
   }      
#endif
   
   while (SDL_PollEvent(&event))
	{
	   switch(event.type) 
	   {
         case SDL_QUIT:
		      SDL_Quit();
		      exit(0);
		      break;

		   case SDL_MOUSEBUTTONDOWN:
			{
			   int x, y;      
			   SDL_GetMouseState(&x, &y);
			   printf("Mouse down at: %d,%d\n",  x, y);
			   break;
			}
			
			case SDL_KEYDOWN:  
			   switch (event.key.keysym.sym) 
			   {
				   case SDLK_SPACE:             
					   paused = 1 - paused;
					   break;
					   
					case SDLK_RIGHT:
					   if (paused) 
					      one_frame = 1;
					   break;
					   
					case 'n':
					   state = STATE_INIT;
					   break;
					   
					case 'h':
					   highlight_found_objects = 1 - highlight_found_objects;
					   break;
					 
					case 'e':
					   print_pushed_events = 1 - print_pushed_events;
					   break;
					   
					case 'v':
					   verbose = 1 - verbose; 
					   break;
					   
					case 'q':
					   write_parallel_port(0);
					   SDL_Quit();
					   exit(0);
					   
					case '1':
					   write_parallel_port(PIN_GREEN);
					   SDL_Delay(500);
					   write_parallel_port(0);					   
					   break;

					case '2':
					   write_parallel_port(PIN_RED);
					   SDL_Delay(500);
					   write_parallel_port(0);					   
					   break;
					   
					case '3':
					   write_parallel_port(PIN_YELLOW);
					   SDL_Delay(500);
					   write_parallel_port(0);					   
					   break;					   

					case SDLK_DOWN:
					   write_parallel_port(PIN_STRUM);
					   SDL_Delay(500);
					   write_parallel_port(0);					   
					   break;

			   }
			   break;      
			
	    }
    }
  }

  // Free the YUV frame
  av_free(pFrame);
  
  // Close the codec
  avcodec_close(pCodecCtx);
  
  // Close the video file
  av_close_input_file(pFormatCtx);
  
  return;
}
