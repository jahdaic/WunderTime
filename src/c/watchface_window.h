typedef void (*callback)(void);

void show_watchface_window(void);
void hide_watchface_window(void);

void set_minute_callback(callback);

void update_task_due(const char *);
void update_task_desc(const char *);
void update_task_count(const char *);