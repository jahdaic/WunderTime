#pragma once

typedef void (*callback_c)(const char *);

void init_wunderlist(void);
void deinit_wunderlist(void);
void get_tasks(void);

void set_task_due_callback(callback_c);
void set_task_desc_callback(callback_c);
void set_task_count_callback(callback_c);