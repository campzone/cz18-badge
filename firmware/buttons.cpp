#include "buttons.h"

const int Buttons::BUTTON_PINS[BUTTON_COUNT] { A2, A3, A6, A7 };
const int Buttons::SWITCH_READ_COUNT = 50;


Buttons::Buttons(int pin_mode, int debounce_ms, bool open_high) :
        pin_mode(pin_mode),
        debounce_ms(debounce_ms),
        open_high(open_high)
{
  for (int i = 0; i < Buttons::BUTTON_COUNT; ++i) 
  {
    this->button_state[i] = false;
    this->last_update[i] = (int)millis();
  }
}

void Buttons::setup()
{
  for(int32_t i = 0; i < Buttons::BUTTON_COUNT; ++i)
  {
    pinMode(Buttons::BUTTON_PINS[i], this->pin_mode);
  }
}

Buttons::button_type Buttons::currently_pressed()
{
  int output = (int)Buttons::button_type::NONE;
  bool buttonstates[] = { false, false, false, false };
  
  for (int i = 0; i < Buttons::BUTTON_COUNT; ++i)
  {
    buttonstates[i] = this->read_update_button(i);
  }

  for (int i = 0; i < Buttons::BUTTON_COUNT; ++i)
  {
    if(buttonstates[i])
    {
      if (output != (int)Buttons::button_type::NONE)
      {
        // Seen another pressed button alread, so that's undefined behaviour
        return Buttons::button_type::NONE;
      }

      output = i;
    }
  }

  return (Buttons::button_type)output;
}

bool Buttons::read_update_button(int bindex)
{
  bool button_pushed = this->read_button_state(bindex);
  bool updated = false;
  int cur = (int)millis();

  if (((cur - this->last_update[bindex]) >= this->debounce_ms))
  {
    updated = (button_pushed != this->button_state[bindex]);
    this->button_state[bindex] = button_pushed;
    this->last_update[bindex] = cur;
  }

  return updated && button_pushed;
}

bool Buttons::read_button_state(int bindex)
{
  for(int32_t i = 0; i < Buttons::SWITCH_READ_COUNT; i++)
  {
    if(analogRead(Buttons::BUTTON_PINS[bindex]))
    {
      return false;
    }
  }
  
  return true;
}

