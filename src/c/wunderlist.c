#include <pebble.h>
#include "wunderlist.h"

static AppSync s_sync;
static uint8_t s_sync_buffer[128];
callback_c task_due_callback, task_desc_callback, task_count_callback;

enum TaskKey {
	TASK_DUE_KEY = 0x0,   // TUPLE_CSTRING
	TASK_DESC_KEY = 0x1,  // TUPLE_CSTRING
	TASK_COUNT_KEY = 0x2, // TUPLE_INT
	TASK_COLOR_KEY = 0x3, // TUPLE_CSTRING
};

void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void appmsg_in_dropped(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %d", reason);
}

void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	switch (key) {
		case TASK_DUE_KEY:
			APP_LOG(APP_LOG_LEVEL_INFO, "Task Due: %s", new_tuple->value->cstring);
		
			if(new_tuple->value->cstring != old_tuple->value->cstring) {
				(*task_due_callback)(new_tuple->value->cstring);
			}
		
			break;
		case TASK_DESC_KEY:
			APP_LOG(APP_LOG_LEVEL_INFO, "Task Desc: %s", new_tuple->value->cstring);
		
			if(new_tuple->value->cstring != old_tuple->value->cstring) {
				(*task_desc_callback)(new_tuple->value->cstring);
			}
		
			break;
		case TASK_COUNT_KEY:
			APP_LOG(APP_LOG_LEVEL_INFO, "Task Count: %d", new_tuple->value->uint8);
				
			if(new_tuple->value->cstring != old_tuple->value->cstring) {
				static char count[] = "000";
				snprintf(count, 3, "%d", new_tuple->value->uint8);

				(*task_count_callback)(count);
			}
		
			break;
		case TASK_COLOR_KEY:
			APP_LOG(APP_LOG_LEVEL_INFO, "Task Color: %s", new_tuple->value->cstring);
				
// 			if(new_tuple->value->cstring != old_tuple->value->cstring) {
// 				static char count[] = "000";
// 				snprintf(count, 3, "%d", new_tuple->value->uint8);

// 				(*task_count_callback)(count);
// 			}
		
			break;
	}
}

void get_tasks(void) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Getting Tasks");
	
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (!iter) {
		// Error creating outbound message
		return;
	}

	int value = 1;
	dict_write_int(iter, 1, &value, sizeof(int), true);
	dict_write_end(iter);

	app_message_outbox_send();
}

void set_task_due_callback(callback_c callback_func) {
	task_due_callback = callback_func;
}

void set_task_desc_callback(callback_c callback_func) {
	task_desc_callback = callback_func;
}

void set_task_count_callback(callback_c callback_func) {
	task_count_callback = callback_func;
}

void init_wunderlist(void) {
	Tuplet initial_values[] = {
		TupletCString(TASK_DUE_KEY, ""),
		TupletCString(TASK_DESC_KEY, "Loading..."),
		TupletInteger(TASK_COUNT_KEY, 0),
		TupletCString(TASK_COLOR_KEY, "blue"),
	};

	app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_tuple_changed_callback, sync_error_callback, NULL);
	
	app_message_open(128, 128);

	get_tasks();
}

void deinit_wunderlist(void) {
	app_sync_deinit(&s_sync);
}