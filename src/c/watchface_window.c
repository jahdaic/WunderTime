#include <pebble.h>
#include "watchface_window.h"

static bool started = false;
static int hours, minutes, seconds, stroke_width = 5;
callback minute_callback;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_steps;
static GFont s_res_gothic_14;
static GFont s_res_bitham_34_medium_numbers;
static GFont s_res_gothic_18;
static GBitmap *s_res_image_battery;
static GBitmap *s_res_image_tasks;
static BitmapLayer *steps_bitmaplayer;
static Layer *canvas_layer;
static TextLayer *date_textlayer;
static TextLayer *time_textlayer;
static TextLayer *seconds_textlayer;
static TextLayer *task_due_textlayer;
static TextLayer *task_desc_textlayer;
static BitmapLayer *battery_bitmaplayer;
static TextLayer *tasks_textlayer;
static TextLayer *steps_textlayer;
static TextLayer *battery_textlayer;
static BitmapLayer *tasks_bitmaplayer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_image_steps = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_STEPS);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_image_battery = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  s_res_image_tasks = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TASKS);
  // steps_bitmaplayer
  steps_bitmaplayer = bitmap_layer_create(GRect(65, 120, 16, 16));
  bitmap_layer_set_bitmap(steps_bitmaplayer, s_res_image_steps);
  layer_add_child(window_get_root_layer(s_window), (Layer *)steps_bitmaplayer);
  
  // canvas_layer
  canvas_layer = layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_get_root_layer(s_window), (Layer *)canvas_layer);
  
  // date_textlayer
  date_textlayer = text_layer_create(GRect(0, 20, 144, 14));
  text_layer_set_background_color(date_textlayer, GColorClear);
  text_layer_set_text_color(date_textlayer, GColorWhite);
  text_layer_set_text(date_textlayer, "SUN 01");
  text_layer_set_text_alignment(date_textlayer, GTextAlignmentCenter);
  text_layer_set_font(date_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_textlayer);
  
  // time_textlayer
  time_textlayer = text_layer_create(GRect(7, 29, 89, 36));
  text_layer_set_background_color(time_textlayer, GColorClear);
  text_layer_set_text_color(time_textlayer, GColorWhite);
  text_layer_set_text(time_textlayer, "10:00");
  text_layer_set_text_alignment(time_textlayer, GTextAlignmentCenter);
  text_layer_set_font(time_textlayer, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_textlayer);
  
  // seconds_textlayer
  seconds_textlayer = text_layer_create(GRect(100, 47, 37, 20));
  text_layer_set_background_color(seconds_textlayer, GColorClear);
  text_layer_set_text_color(seconds_textlayer, GColorWhite);
  text_layer_set_text(seconds_textlayer, "00 PM");
  text_layer_set_font(seconds_textlayer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)seconds_textlayer);
  
  // task_due_textlayer
  task_due_textlayer = text_layer_create(GRect(0, 70, 144, 16));
  text_layer_set_background_color(task_due_textlayer, GColorClear);
  text_layer_set_text_color(task_due_textlayer, GColorWhite);
  text_layer_set_text(task_due_textlayer, "Due Wednesday");
  text_layer_set_text_alignment(task_due_textlayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)task_due_textlayer);
  
  // task_desc_textlayer
  task_desc_textlayer = text_layer_create(GRect(0, 85, 144, 30));
  text_layer_set_background_color(task_desc_textlayer, GColorClear);
  text_layer_set_text_color(task_desc_textlayer, GColorWhite);
  text_layer_set_text(task_desc_textlayer, "This is the text layer that contains the tasky description");
  text_layer_set_text_alignment(task_desc_textlayer, GTextAlignmentCenter);
  text_layer_set_font(task_desc_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)task_desc_textlayer);
  
  // battery_bitmaplayer
  battery_bitmaplayer = bitmap_layer_create(GRect(111, 120, 16, 16));
  bitmap_layer_set_bitmap(battery_bitmaplayer, s_res_image_battery);
  layer_add_child(window_get_root_layer(s_window), (Layer *)battery_bitmaplayer);
  
  // tasks_textlayer
  tasks_textlayer = text_layer_create(GRect(0, 140, 48, 14));
  text_layer_set_background_color(tasks_textlayer, GColorClear);
  text_layer_set_text_color(tasks_textlayer, GColorWhite);
  text_layer_set_text(tasks_textlayer, "00");
  text_layer_set_text_alignment(tasks_textlayer, GTextAlignmentCenter);
  text_layer_set_font(tasks_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)tasks_textlayer);
  
  // steps_textlayer
  steps_textlayer = text_layer_create(GRect(48, 140, 48, 14));
  text_layer_set_background_color(steps_textlayer, GColorClear);
  text_layer_set_text_color(steps_textlayer, GColorWhite);
  text_layer_set_text(steps_textlayer, "0");
  text_layer_set_text_alignment(steps_textlayer, GTextAlignmentCenter);
  text_layer_set_font(steps_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)steps_textlayer);
  
  // battery_textlayer
  battery_textlayer = text_layer_create(GRect(96, 140, 48, 14));
  text_layer_set_background_color(battery_textlayer, GColorClear);
  text_layer_set_text_color(battery_textlayer, GColorWhite);
  text_layer_set_text(battery_textlayer, "100%");
  text_layer_set_text_alignment(battery_textlayer, GTextAlignmentCenter);
  text_layer_set_font(battery_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)battery_textlayer);
  
  // tasks_bitmaplayer
  tasks_bitmaplayer = bitmap_layer_create(GRect(20, 120, 16, 16));
  bitmap_layer_set_bitmap(tasks_bitmaplayer, s_res_image_tasks);
  layer_add_child(window_get_root_layer(s_window), (Layer *)tasks_bitmaplayer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(steps_bitmaplayer);
  layer_destroy(canvas_layer);
  text_layer_destroy(date_textlayer);
  text_layer_destroy(time_textlayer);
  text_layer_destroy(seconds_textlayer);
  text_layer_destroy(task_due_textlayer);
  text_layer_destroy(task_desc_textlayer);
  bitmap_layer_destroy(battery_bitmaplayer);
  text_layer_destroy(tasks_textlayer);
  text_layer_destroy(steps_textlayer);
  text_layer_destroy(battery_textlayer);
  bitmap_layer_destroy(tasks_bitmaplayer);
  gbitmap_destroy(s_res_image_steps);
  gbitmap_destroy(s_res_image_battery);
  gbitmap_destroy(s_res_image_tasks);
}
// END AUTO-GENERATED UI CODE

// UTILITY FUNCTIONS
float get_sqrt(const float num) {

	const uint MAX_STEPS = 40;
	const float MAX_ERROR = 0.001;
	float answer = num;
	float ans_sqr = answer * answer;
	uint step = 0;

	while((ans_sqr - num > MAX_ERROR) && (step++ < MAX_STEPS)) {
		answer = (answer + (num / answer)) / 2;
		ans_sqr = answer * answer;
	}

	return answer;
}

float get_pow(const float num, const float exp) {
	float answer = num;
	
	for(int i = 1; i < exp; i++) {
		answer = answer * num;
	}
	
	return answer;
}

float get_distance(GPoint p1, GPoint p2) {
	float answer = get_sqrt(get_pow(p2.x - p1.x, 2) + get_pow(p2.y - p1.y, 2));
	
	return answer;
}

char lower_to_upper(char c) {
	return (c>='a' && c<='z')?c&0xdf:c;
}

int32_t get_angle_for_minute(int minute) { 
	return (minute * 360) / 60; 
}

GPoint get_start_point_for_hour(int hour) {
	GRect window_bounds = layer_get_bounds(window_get_root_layer(s_window));
	int perimeter = (2 * window_bounds.size.h) + (2 * window_bounds.size.w);
	int hour_position = perimeter / 12 * hour;

	if(hour_position < window_bounds.size.w / 2) {
		return GPoint(window_bounds.size.w / 2 + hour_position, 0);
	}
	else if(hour_position < window_bounds.size.w / 2 + window_bounds.size.h) {
		return GPoint(window_bounds.size.w - 1, hour_position - (window_bounds.size.w / 2));
	}
	else if(hour_position < window_bounds.size.w / 2 + window_bounds.size.h + window_bounds.size.w) {
		return GPoint(hour_position - (window_bounds.size.w / 2) - window_bounds.size.h, window_bounds.size.h - 1);
	}
	else if(hour_position < window_bounds.size.w / 2 + (2 * window_bounds.size.h) + window_bounds.size.w) {
		return GPoint(0, hour_position - (window_bounds.size.w / 2) - window_bounds.size.h - window_bounds.size.w);
	}
	else {
		return GPoint(hour_position - (window_bounds.size.w / 2) - (window_bounds.size.h * 2) - window_bounds.size.w, 0);
	}
}

//UPDATE FUNCTIONS
void update_battery(BatteryChargeState battery) {
	static char battery_text[] = "100%";
	
	if(battery.is_charging) {
		gbitmap_destroy(s_res_image_battery);
		s_res_image_battery = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_CHARGING);
		bitmap_layer_set_bitmap(battery_bitmaplayer, s_res_image_battery);
		layer_mark_dirty(bitmap_layer_get_layer(battery_bitmaplayer));
	}
	else if(battery.charge_percent <= 20) {
		gbitmap_destroy(s_res_image_battery);
		s_res_image_battery = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_LOW);
		bitmap_layer_set_bitmap(battery_bitmaplayer, s_res_image_battery);
		layer_mark_dirty(bitmap_layer_get_layer(battery_bitmaplayer));
	}
	else {
		gbitmap_destroy(s_res_image_battery);
		s_res_image_battery = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
		bitmap_layer_set_bitmap(battery_bitmaplayer, s_res_image_battery);
		layer_mark_dirty(bitmap_layer_get_layer(battery_bitmaplayer));
	}
	
	snprintf(battery_text, sizeof(battery_text), "%d%%", battery.charge_percent);
	
	text_layer_set_text(battery_textlayer, battery_text);
	
	layer_mark_dirty(text_layer_get_layer(battery_textlayer));
}

void update_minutes(struct tm* tick_time, TimeUnits units_changed) {
	static char time_text[] = "00:00";
	static char date_text[] = "SUN 01";
	static char steps_text[] = "99999";
	char * time_format = "%i:%i";
	
	hours = tick_time->tm_hour;
	minutes = tick_time->tm_min;	
	
	if(hours > 12) { hours -= 12; }
	if(minutes < 10) { time_format = "%i:0%u"; }
	snprintf(time_text, sizeof(time_text), time_format, hours, minutes);
	
	int steps = health_service_sum_today(HealthMetricStepCount);

	// APP_LOG(APP_LOG_LEVEL_INFO, "Time: %s", time_text);
	strftime(date_text, sizeof(date_text), "%a %d", tick_time);
	snprintf(steps_text, sizeof(steps_text), "%d", steps);
	
	for (int i = 0; date_text[i]; i++) {
		date_text[i] = lower_to_upper(date_text[i]);
	}
		
	text_layer_set_text(time_textlayer, time_text);
	text_layer_set_text(date_textlayer, date_text);	
	text_layer_set_text(steps_textlayer, steps_text);
	
	layer_mark_dirty(text_layer_get_layer(time_textlayer));
	layer_mark_dirty(text_layer_get_layer(date_textlayer));
	layer_mark_dirty(text_layer_get_layer(steps_textlayer));
	
	(*minute_callback)();
}

void update_seconds(struct tm* tick_time, TimeUnits units_changed) {
	static char seconds_text[] = "00 AM";
	
	strftime(seconds_text, sizeof(seconds_text), "%S %p", tick_time);
	
	seconds = tick_time->tm_sec;
	
	text_layer_set_text(seconds_textlayer, seconds_text);
	
	if(seconds == 0 || !started) {
		update_minutes(tick_time, units_changed);
		update_battery(battery_state_service_peek());
		started = true;
	}
	
	layer_mark_dirty(text_layer_get_layer(seconds_textlayer));
	layer_mark_dirty(canvas_layer);
}

// LAYER UPDATES
void update_task_due(const char *date) {
	text_layer_set_text(task_due_textlayer, date);
	layer_mark_dirty(text_layer_get_layer(task_due_textlayer));
}

void update_task_desc(const char *description) {
	text_layer_set_text(task_desc_textlayer, description);
	layer_mark_dirty(text_layer_get_layer(task_desc_textlayer));
}

void update_task_count(const char *count) {
	text_layer_set_text(tasks_textlayer, count);
	layer_mark_dirty(text_layer_get_layer(tasks_textlayer));
}

void update_round_canvas(Layer *layer, GContext *ctx) {
	GRect window_bounds = layer_get_bounds(window_get_root_layer(s_window));
	GPoint center_point = GPoint(window_bounds.size.w / 2, window_bounds.size.h / 2);
	
	int minute_angle = get_angle_for_minute(seconds);
	
	graphics_context_set_fill_color(ctx, GColorRed);
	graphics_fill_radial(ctx, window_bounds, GOvalScaleModeFillCircle, stroke_width, 0, DEG_TO_TRIGANGLE(minute_angle));
	
	// Draw Ticks
	graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
	graphics_context_set_stroke_width(ctx, 1);
	
	for(int i = 0; i < 12; i++) {
		GPoint start_point = gpoint_from_polar(window_bounds, GOvalScaleModeFillCircle, DEG_TO_TRIGANGLE((360 / 12) * i));
		GPoint end_point;
		
		end_point.x = start_point.x + (((stroke_width + 1) / get_distance(start_point, center_point)) * (center_point.x - start_point.x));
		end_point.y = start_point.y + (((stroke_width + 1) / get_distance(start_point, center_point)) * (center_point.y - start_point.y));
				
		graphics_draw_line(ctx, start_point, end_point);
	}	
}

void update_rectangle_canvas(Layer *layer, GContext *ctx) {
	GRect window_bounds = layer_get_bounds(window_get_root_layer(s_window));
	GPoint center_point = GPoint(window_bounds.size.w / 2, window_bounds.size.h / 2);
	
	int perimeter = (2 * window_bounds.size.h) + (2 * window_bounds.size.w);
	int minute_position = perimeter / 60 * (seconds + 1);
	bool done = false;
	
	// Drawing Settings
	graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
	graphics_context_set_stroke_width(ctx, stroke_width * 2);
		
	// Top right
	if(window_bounds.size.w / 2 < minute_position) {
		graphics_draw_line(ctx, GPoint(window_bounds.size.w / 2, 0), GPoint(window_bounds.size.w - 1, 0));
		minute_position -= window_bounds.size.w / 2;
	}
	else {
		graphics_draw_line(ctx, GPoint(window_bounds.size.w / 2, 0), GPoint((window_bounds.size.w / 2) + minute_position, 0));
		done = true;
	}
		
	// Right
	if(!done) {
		if(window_bounds.size.h < minute_position) {
			graphics_draw_line(ctx, GPoint(window_bounds.size.w - 1, 0), GPoint(window_bounds.size.w - 1, window_bounds.size.h));
			minute_position -= window_bounds.size.h;
		}
		else {
			graphics_draw_line(ctx, GPoint(window_bounds.size.w - 1, 0), GPoint(window_bounds.size.w - 1, minute_position));
			done = true;
		}
	}
	
	// Bottom
	if(!done) {
		if(window_bounds.size.w < minute_position) {
			graphics_draw_line(ctx, GPoint(window_bounds.size.w - 1, window_bounds.size.h), GPoint(0, window_bounds.size.h));
			minute_position -= window_bounds.size.w;
		}
		else {
			graphics_draw_line(ctx, GPoint(window_bounds.size.w - 1, window_bounds.size.h), GPoint(window_bounds.size.w - minute_position, window_bounds.size.h));
			done = true;
		}
	}
	
	// Left
	if(!done) {
		if(window_bounds.size.h < minute_position) {
			graphics_draw_line(ctx, GPoint(0, window_bounds.size.h), GPoint(0, 0));
			minute_position -= window_bounds.size.w;
		}
		else {
			graphics_draw_line(ctx, GPoint(0, window_bounds.size.h), GPoint(0, window_bounds.size.h - minute_position));
			done = true;
		}
	}
	
	// Top left
	if(!done) {
		if(window_bounds.size.w / 2 < minute_position) {
			graphics_draw_line(ctx, GPoint(0, 0), GPoint(window_bounds.size.w / 2, 0));
			minute_position -= window_bounds.size.w / 2;
		}
		else {
			graphics_draw_line(ctx, GPoint(0, 0), GPoint(minute_position, 0));
			done = true;
		}
	}
	
	// Draw Ticks
	graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
	graphics_context_set_stroke_width(ctx, 1);
		
	for(int i = 0; i < 12; i++) {
		GPoint start_point = get_start_point_for_hour(i);
		GPoint end_point;
		
		end_point.x = start_point.x + (((stroke_width + 1) / get_distance(start_point, center_point)) * (center_point.x - start_point.x));
		end_point.y = start_point.y + (((stroke_width + 1) / get_distance(start_point, center_point)) * (center_point.y - start_point.y));
				
		graphics_draw_line(ctx, start_point, end_point);
	}	
}

void update_canvas_layer(Layer *layer, GContext *ctx) {
	if(PBL_IF_ROUND_ELSE(true, false)) {
		update_round_canvas(layer, ctx);
	}
	else {
		update_rectangle_canvas(layer, ctx);
	}
}

void set_minute_callback(callback callback_func) {
	minute_callback = callback_func;
}

// WINDOW HANDLING
void fit_watchface(void) {
	GRect window_bounds = layer_get_bounds(window_get_root_layer(s_window));
	int16_t window_height = window_bounds.size.h;
	int16_t window_width = window_bounds.size.w;
	
	GRect date_bounds = layer_get_bounds(text_layer_get_layer(date_textlayer));
	GRect time_bounds = layer_get_bounds(text_layer_get_layer(time_textlayer));
	GRect seconds_bounds = layer_get_bounds(text_layer_get_layer(seconds_textlayer));
	GRect task_due_bounds = layer_get_bounds(text_layer_get_layer(task_due_textlayer));
	GRect task_desc_bounds = layer_get_bounds(text_layer_get_layer(task_desc_textlayer));
	GRect tasks_bounds = layer_get_bounds(text_layer_get_layer(tasks_textlayer));
	GRect steps_bounds = layer_get_bounds(text_layer_get_layer(steps_textlayer));
	GRect battery_bounds = layer_get_bounds(text_layer_get_layer(battery_textlayer));
	GRect tasks_icon_bounds = layer_get_bounds(bitmap_layer_get_layer(tasks_bitmaplayer));
	GRect steps_icon_bounds = layer_get_bounds(bitmap_layer_get_layer(steps_bitmaplayer));
	GRect battery_icon_bounds = layer_get_bounds(bitmap_layer_get_layer(battery_bitmaplayer));
	
	// Date
	date_bounds.origin.y = window_height / 3 / 2 - date_bounds.size.h;
	date_bounds.size.w = window_width;
	layer_set_frame(text_layer_get_layer(date_textlayer), date_bounds);
	layer_mark_dirty(text_layer_get_layer(date_textlayer));
	
	// Time
	time_bounds.origin.x = (window_width - time_bounds.size.w - seconds_bounds.size.w) / 2;
	time_bounds.origin.y = window_height / 3 / 2;
	layer_set_frame(text_layer_get_layer(time_textlayer), time_bounds);
	layer_mark_dirty(text_layer_get_layer(time_textlayer));
	
	// Seconds
	seconds_bounds.origin.x = (window_width - time_bounds.size.w - seconds_bounds.size.w) / 2 + time_bounds.size.w;
	seconds_bounds.origin.y = time_bounds.origin.y + time_bounds.size.h - seconds_bounds.size.h;
	layer_set_frame(text_layer_get_layer(seconds_textlayer), seconds_bounds);
	layer_mark_dirty(text_layer_get_layer(seconds_textlayer));
	
	// Task Due
	task_due_bounds.origin.y = window_height / 2 - task_due_bounds.size.h;
	task_due_bounds.size.w = window_width;
	layer_set_frame(text_layer_get_layer(task_due_textlayer), task_due_bounds);
	text_layer_set_text_color(task_due_textlayer, PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorWhite));
	text_layer_set_overflow_mode(task_due_textlayer, GTextOverflowModeWordWrap);
	layer_mark_dirty(text_layer_get_layer(task_due_textlayer));
	
	// Task Description
	task_desc_bounds.origin.y = window_height / 2;
	task_desc_bounds.size.w = window_width;
	layer_set_frame(text_layer_get_layer(task_desc_textlayer), task_desc_bounds);
	text_layer_enable_screen_text_flow_and_paging(task_desc_textlayer, 12);
	layer_mark_dirty(text_layer_get_layer(task_desc_textlayer));
	
	// Task Count
	tasks_bounds.origin.x = (window_width / 4) - (tasks_bounds.size.w / 2);
	tasks_bounds.origin.y = window_height / 3 * 2.5;
	layer_set_frame(text_layer_get_layer(tasks_textlayer), tasks_bounds);
	layer_mark_dirty(text_layer_get_layer(tasks_textlayer));
	
	// Steps Count
	steps_bounds.origin.x = (window_width / 4 * 2) - (steps_bounds.size.w / 2);
	steps_bounds.origin.y = window_height / 3 * 2.5;
	layer_set_frame(text_layer_get_layer(steps_textlayer), steps_bounds);
	layer_mark_dirty(text_layer_get_layer(steps_textlayer));
	
	// Battery Level
	battery_bounds.origin.x = (window_width / 4 * 3) - (battery_bounds.size.w / 2);
	battery_bounds.origin.y = window_height / 3 * 2.5;
	layer_set_frame(text_layer_get_layer(battery_textlayer), battery_bounds);
	layer_mark_dirty(text_layer_get_layer(battery_textlayer));
	
	// Task Count
	tasks_icon_bounds.origin.x = (window_width / 4) - (tasks_icon_bounds.size.w / 2);
	tasks_icon_bounds.origin.y = window_height / 3 * 2.5 - tasks_icon_bounds.size.h;
	layer_set_frame(bitmap_layer_get_layer(tasks_bitmaplayer), tasks_icon_bounds);
	layer_mark_dirty(bitmap_layer_get_layer(tasks_bitmaplayer));
	
	// Steps Count
	steps_icon_bounds.origin.x = (window_width / 4 * 2) - (steps_icon_bounds.size.w / 2);
	steps_icon_bounds.origin.y = window_height / 3 * 2.5 - steps_icon_bounds.size.h;
	layer_set_frame(bitmap_layer_get_layer(steps_bitmaplayer), steps_icon_bounds);
	layer_mark_dirty(bitmap_layer_get_layer(steps_bitmaplayer));
	
	// Battery Level
	battery_icon_bounds.origin.x = (window_width / 4 * 3) - (battery_icon_bounds.size.w / 2);
	battery_icon_bounds.origin.y = window_height / 3 * 2.5 - battery_icon_bounds.size.h;
	layer_set_frame(bitmap_layer_get_layer(battery_bitmaplayer), battery_icon_bounds);
	layer_mark_dirty(bitmap_layer_get_layer(battery_bitmaplayer));
	
	// Canvas
	layer_set_frame(canvas_layer, GRect(0, 0, window_width, window_height));
	layer_mark_dirty(canvas_layer);
}

void handle_window_unload(Window* window) {
	destroy_ui();
}

void show_watchface_window(void) {
	initialise_ui();
	
	fit_watchface();
	
	tick_timer_service_subscribe(SECOND_UNIT, update_seconds);
	//tick_timer_service_subscribe(MINUTE_UNIT, update_minutes);
	battery_state_service_subscribe(update_battery);
	
	layer_set_update_proc(canvas_layer, update_canvas_layer);
	
	window_set_window_handlers(s_window, (WindowHandlers) {
		.unload = handle_window_unload,
	});

	window_stack_push(s_window, true);
}

void hide_watchface_window(void) {
  window_stack_remove(s_window, true);
}
