//--------------------------
// Test Time App
// Written By : Blake
// Written on: 20150126
//--------------------------
#include <pebble.h>
  
//--------------------------
// Setup
//--------------------------
static Window *s_main_window;
static TextLayer *time_layer;
static TextLayer *date_layer;
static TextLayer *batt_layer;

static char datetext[] ="AAA ##";
static char timetext[] ="00:00";
static char batttext[] ="100% charged"; 

//--------------------------
// Update the time
//--------------------------
static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
   
  if(clock_is_24h_style() == true){
    strftime(timetext, sizeof("00:00"),  "%H:%M", tick_time);
  }  else{
    strftime(timetext, sizeof("00:00"),  "%I:%M", tick_time);
  }
    text_layer_set_text(time_layer,timetext);
}

//--------------------------
// Update the Date
//--------------------------
//TODO: Expand the date option to run across the header.
static void update_date(){
  time_t temp = time(NULL);
  struct tm *tick_date = localtime(&temp);
  
  strftime(datetext, sizeof("Day ##"),"%a %d" , tick_date);
  text_layer_set_text(date_layer, datetext);
}

//--------------------------
// Update the Battery Charge
//--------------------------
static void update_batt(){
  BatteryChargeState charge_state = battery_state_service_peek();
  if (charge_state.is_charging) {
    snprintf(batttext, sizeof(batttext), "charging");
  } else {
  snprintf(batttext, sizeof(batttext), "%d%% charged", charge_state.charge_percent);
  }
  text_layer_set_text(batt_layer, batttext);
}

//--------------------------
// Load Window
//--------------------------

//TODO: make this a function later
static void main_window_load(Window *window){
//build layers
  time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorClear);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  date_layer = text_layer_create(GRect(5, 0, 50, 50));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentLeft);
  
  batt_layer = text_layer_create(GRect(67, 150, 90, 50));
  text_layer_set_background_color(batt_layer, GColorClear);
  text_layer_set_text_color(batt_layer, GColorClear);
  text_layer_set_font(batt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(batt_layer, GTextAlignmentLeft);
  
//add layers and update them
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(batt_layer));  
  update_time();
  update_date();
  update_batt();
}

//--------------------------
// Unload window
//--------------------------
static void main_window_unload(Window *window){
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(batt_layer);
}

//minute ticker
static void tick_handler(struct tm *tick_time ,TimeUnits units_changed){
  update_time();
  update_batt();
  update_date();
}

//--------------------------
// Intitalize
//--------------------------
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_set_fullscreen(s_main_window,true);
  window_stack_push(s_main_window, true);
  
 // subscribe ticker
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}
  
static void deinit(){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}