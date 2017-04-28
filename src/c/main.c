#include <pebble.h>
#include "watchface_window.h"
#include "wunderlist.h"

Window *my_window;
TextLayer *text_layer;

void handle_init(void)
{
	show_watchface_window();
	init_wunderlist();
	
	set_minute_callback(get_tasks);
	set_task_due_callback(update_task_due);
	set_task_desc_callback(update_task_desc);
	set_task_count_callback(update_task_count);
}

void handle_deinit(void)
{
	hide_watchface_window();
	deinit_wunderlist();
}

int main(void)
{
	handle_init();
	app_event_loop();
	handle_deinit();
}
