#include <pebble.h>
 
Window* window;
TextLayer *time_layer;
TextLayer *date_layer;
GBitmap *piffle_bitmap;
BitmapLayer *piffle_layer;

static char time_buffer[] = "00:00";
static char day_text[] = "Day 00";
char *time_format;

//UPDATE TIME
void time_handler(struct tm *tick_time, TimeUnits units_changed)
{
if(clock_is_24h_style()) {
time_format = "%R";
} else {
time_format = "%I:%M";
}
//Format the buffer string using tick_time as the time source
strftime(time_buffer, sizeof(time_buffer), time_format, tick_time);

//Change the Time Layer text to show the new time!
text_layer_set_text(time_layer, time_buffer);

//Date layer
strftime(day_text, sizeof(day_text), "%a %d", tick_time);
text_layer_set_text(date_layer, day_text);
}

void window_load(Window *window)
{
//Load fonts
ResHandle time_font_handle = resource_get_handle(RESOURCE_ID_GOUDY_BOOKLETTER_65);	
ResHandle date_font_handle = resource_get_handle(RESOURCE_ID_QUICKSAND_REGULAR_15);	
	
//Piffle Logo
piffle_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PIFFLE_LOGO);
//Create BitmapLayers to show GBitmaps and add to Window
piffle_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
bitmap_layer_set_bitmap(piffle_layer, piffle_bitmap);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(piffle_layer));	
	
//Time layer
time_layer = text_layer_create(GRect(0, 45, 144, 168));
text_layer_set_background_color(time_layer, GColorClear);
text_layer_set_text_color(time_layer, GColorBlack);
text_layer_set_text_alignment(time_layer, GTextAlignmentRight);
text_layer_set_font(time_layer, fonts_load_custom_font(time_font_handle));

layer_add_child(window_get_root_layer(window), (Layer*) time_layer);
	
//Date layer
date_layer = text_layer_create(GRect(0, 150, 144, 33));
text_layer_set_background_color(date_layer, GColorClear);
text_layer_set_text_color(date_layer, GColorDarkGray);
text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
text_layer_set_font(date_layer, fonts_load_custom_font(date_font_handle));

layer_add_child(window_get_root_layer(window), (Layer*) date_layer);
	
//Get a time structure so that the face doesn't start blank
struct tm *t;
time_t temp;	
temp = time(NULL);	
t = localtime(&temp);	

//Manually call the tick handler when the window is loading
time_handler(t, MINUTE_UNIT);
}
 
void window_unload(Window *window)
{
//Destroy TextLayer
text_layer_destroy(time_layer);
text_layer_destroy(date_layer);

//Destroy bitmaps
gbitmap_destroy(piffle_bitmap);
bitmap_layer_destroy(piffle_layer);
	
}

void init()
{
//Initialize the app elements here!
window = window_create();
window_set_background_color(window, GColorBlack);
window_set_window_handlers(window, (WindowHandlers) {
.load = window_load,
.unload = window_unload,
});

tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) time_handler);

window_stack_push(window, true);
}
 
void deinit()
{
//De-initialize elements here to save memory!
tick_timer_service_unsubscribe();

window_destroy(window);
}
 
int main(void)
{
init();
app_event_loop();
deinit();
}