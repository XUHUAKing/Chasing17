/*
 * debug.h
 *
 *  Created on: Jun 28, 2017
 *      Author: dipsy
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include "libsc/st7735r.h"
#include "libsc/lcd_typewriter.h"
#include "libsc/joystick.h"
#include "libbase/k60/flash.h"
#include "debug_console.h"

#include "car_manager.h"

using libsc::St7735r;
using libsc::System;
using libsc::LcdTypewriter;
using libsc::Joystick;
using libbase::k60::Flash;

uint16_t car = 0;
bool confirm = false;

namespace debug_flag{
	  bool lcd_debug = false;
}

void confirmCar() {
  confirm = true;
}

void loadItems(DebugConsole* console) {
  if (car == 1) {
    using namespace algorithm::optimal::car1::TuningVar;

    console->PushItem("LCD debug", &debug_flag::lcd_debug);
    console->PushItem("Speed", &targetSpeed, 5);
    console->PushItem("Normal Speed", &normal_speed, 50);
    console->PushItem("Normal Kp", &servo_normal_kp, 0.1);
    console->PushItem("Normal Kd", &servo_normal_kd, 0.01);
    console->PushItem("Straight Speed", &straight_speed, 50);
    console->PushItem("Straight Kp", &servo_straight_kp, 0.1);
    console->PushItem("Cross Speed", &cross_speed, 50);
    console->PushItem("Cross Kp", &servo_cross_kp, 0.1);
    console->PushItem("Rabt Speed", &roundabout_speed, 50);
    console->PushItem("Rabt Kp", &servo_roundabout_kp, 0.1);
    console->PushItem("Exit Rabt Speed", &roundabout_exit_speed, 50);
    console->PushItem("Exit Rabt Kp", &servo_roundabout_exit_kp, 0.1);
    console->PushItem("Exit Rabt Kp", &servo_exit_kp, 0.1);
    console->PushItem("Rabt Sel", &algorithm::optimal::car1::TuningVar::roundabout_shortest_flag);
    console->PushItem("Rabt Raw", &algorithm::optimal::car1::TuningVar::roundabout_shortest_flag, 1);

  } else if (car != 0) {
    using namespace algorithm::optimal::car2::TuningVar;

    console->PushItem("LCD debug", &debug_flag::lcd_debug);
    console->PushItem("Speed", &targetSpeed, 5);
    console->PushItem("Normal Speed", &normal_speed, 50);
    console->PushItem("Normal Kp", &servo_normal_kp, 0.1);
    console->PushItem("Normal Kd", &servo_normal_kd, 0.01);
    console->PushItem("Straight Speed", &straight_speed, 50);
    console->PushItem("Straight Kp", &servo_straight_kp, 0.1);
    console->PushItem("Cross Speed", &cross_speed, 50);
    console->PushItem("Cross Kp", &servo_cross_kp, 0.1);
    console->PushItem("Rabt Speed", &roundabout_speed, 50);
    console->PushItem("Rabt Kp", &servo_roundabout_kp, 0.1);
    console->PushItem("Exit Rabt Speed", &roundabout_exit_speed, 50);
    console->PushItem("Exit Rabt Kp", &servo_roundabout_exit_kp, 0.1);
    console->PushItem("Exit Rabt Kp", &servo_exit_kp, 0.1);
    console->PushItem("Rabt Sel", &algorithm::optimal::car1::TuningVar::roundabout_shortest_flag);
    console->PushItem("Rabt Raw", &algorithm::optimal::car1::TuningVar::roundabout_shortest_flag, 1);

  }
}

uint16_t debug(bool call_reset) {

  St7735r::Config lcd_config;
  lcd_config.is_revert = true;
  St7735r lcd(lcd_config);
  lcd.Clear();

  LcdTypewriter::Config writerconfig;
  writerconfig.lcd = &lcd;
  LcdTypewriter writer(writerconfig);

  Joystick::Config joystick_config;
  joystick_config.id = 0;
  joystick_config.is_active_low = true;
  Joystick joystick(joystick_config);

  Flash::Config flash_config;
  Flash flash(flash_config);

  DebugConsole console(&joystick, &lcd, &writer);

  console.SetFlash(&flash);
  console.PushItem("confirm: car", &car, 1);
  Item item = console.GetItem(0);
  item.listener = &confirmCar;
  console.SetItem(0, item);
  console.Load();

  if (call_reset) {
    car = 0;
  }
  while (car != 1 && car != 2) {
    confirm = false;
    call_reset = true;
    console.ListItems();
    while (!confirm) {
      console.Listen();
      console.Save();
    }
  }
  confirm = false;
  item.type = VarType::kNan;
  item.text = (char*) (car == 1 ? ">>run car1<<" : ">>run car2<<");
  console.SetItem(0, item);

  loadItems(&console);

  if (!call_reset) console.Load();

  console.ListItems();
  while (!confirm) {
    console.Listen();
    console.Save();
  }

  return car;
}

#endif /* INC_DEBUG_H_ */
