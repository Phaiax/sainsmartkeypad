#include "Arduino.h"
#include "sainsmartkeypad.h"

/* The source code in this file sainsmartkeypad.cpp
 * is licensed under the GNU GPLv3 (http://www.gnu.org/licenses/gpl.html)
 * Ask me if you want to have extended rights. */
 
/* Authors: Phaiax (Daniel Seemer). 
 * Mail patches to: phaiax <aet> invisibletower [doat] de  
 * (confusing spam bots...)
 * 
 * LIBRARY-VERSION: 1.0
 */


static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 15;
static int DEFAULT_REFRESH_RATE = 10;

static int DEFAULT_MS_TO_ACTIVATE_FAST_SCROLL = 400;
static int DEFAULT_FAST_SCROLL_TRIGGER_EVERY_MS = 100;
static int DEFAULT_MAX_FAST_SCROLL_TRIGGER_EVERY_MS = 20;

// ADJUST THIS REGARDING YOUR OWN HARDWARE
static int UPKEY_ARV = 142; //that's read "analogue read value"
static int DOWNKEY_ARV = 326;
static int LEFTKEY_ARV = 503;
static int RIGHTKEY_ARV = 0;
static int SELKEY_ARV = 741;
static int NOKEY_ARV = 1023;


SainsmartKeypad::SainsmartKeypad(int rate, int pin)
{
  _init();
  _keyPin = pin;
  _refreshRate = rate;
}

SainsmartKeypad::SainsmartKeypad(int pin){
  _init();
  _keyPin = pin;
}


SainsmartKeypad::SainsmartKeypad() 
{
  _init();
} 

void SainsmartKeypad::_init()
{
  _refreshRate = DEFAULT_REFRESH_RATE,
  _msToActivateFastScroll = DEFAULT_MS_TO_ACTIVATE_FAST_SCROLL;
  _maxFastScrollTriggerRate = DEFAULT_MAX_FAST_SCROLL_TRIGGER_EVERY_MS;
  _fastScrollTriggerRate = DEFAULT_FAST_SCROLL_TRIGGER_EVERY_MS;
  _keyPin = DEFAULT_KEY_PIN;
  _prevKey = NO_KEY;
  _curKey = NO_KEY;
}


int SainsmartKeypad::getKey_fastscroll()
{
  _curKey = getKey_periodic();  
  if(_curKey == SAMPLE_WAIT)
    return SAMPLE_WAIT;  // Ignore SAMPLE_WAITs
  if(_prevKey != _curKey)
  {
    _prevKey = _curKey;
    _lastFastScrollTime = 0;
    _lastKeyChangeTime = millis();
    return _curKey; // Forward once on first press
  }
  if(_curKey == NO_KEY){
    return SAMPLE_WAIT;
  }

  // _curKey is not SAMPLE_WAIT and NOT NO_KEY here.

  if(_prevKey == _curKey)
  {
    unsigned long curmillis = millis();
    unsigned long lastKeyChangeDiff = curmillis - _lastKeyChangeTime;
    if(lastKeyChangeDiff > _msToActivateFastScroll)
    {
      // Fast Scrolling activated

      unsigned long triggerevery = _fastScrollTriggerRate / (lastKeyChangeDiff / _msToActivateFastScroll);
      if(triggerevery < _maxFastScrollTriggerRate)
        triggerevery = _maxFastScrollTriggerRate;

      if(curmillis > _lastFastScrollTime + triggerevery)
      {
        // FastTrigger NOW
        _lastFastScrollTime = curmillis; 
        //Serial.print(triggerevery);
        //E
        //Serial.println("Scroll");    
        return _curKey;      
      }
      else
        // Key is pressed but not forwarded. Wait for next
        // fast scroll trigger.
        return SAMPLE_WAIT; 
    }
    else
      // Fast scroll not yet activated
      return SAMPLE_WAIT; 
  }

  // Code should not reach this.
  return 99;
  ;
}


int SainsmartKeypad::getKey_waitrelease(){
  _curKey = getKey_periodic();
  if(_curKey != SAMPLE_WAIT && _curKey != _prevKey)
  {
    _prevKey = _curKey;
    return _curKey;
  }
  return SAMPLE_WAIT;
}

int SainsmartKeypad::getKey_periodic()
{
  if (millis() > _oldTime + _refreshRate)
  {
    _oldTime = millis();
    return getKey_instant();
  }
  return SAMPLE_WAIT;
}

int SainsmartKeypad::getKey_instant()
{
  _curInput = analogRead(_keyPin);

  if (_curInput > UPKEY_ARV - DEFAULT_THRESHOLD && _curInput < UPKEY_ARV + DEFAULT_THRESHOLD ) return UP_KEY;
  else if (_curInput > DOWNKEY_ARV - DEFAULT_THRESHOLD && _curInput < DOWNKEY_ARV + DEFAULT_THRESHOLD ) return  DOWN_KEY;
  else if (_curInput > RIGHTKEY_ARV - DEFAULT_THRESHOLD && _curInput < RIGHTKEY_ARV + DEFAULT_THRESHOLD ) return RIGHT_KEY;
  else if (_curInput > LEFTKEY_ARV - DEFAULT_THRESHOLD && _curInput < LEFTKEY_ARV + DEFAULT_THRESHOLD ) return LEFT_KEY;
  else if (_curInput > SELKEY_ARV - DEFAULT_THRESHOLD && _curInput < SELKEY_ARV + DEFAULT_THRESHOLD ) return SELECT_KEY;
  else return NO_KEY;
}

void SainsmartKeypad::setRefreshRate(int rate)
{
  _refreshRate = rate; 
}
void SainsmartKeypad::setMsToActivateFastScroll(int ms)
{
  _msToActivateFastScroll = ms;
}
void SainsmartKeypad::setFastScrollTriggerRate(int rate, int maxrate)
{
  _fastScrollTriggerRate = rate;
  _maxFastScrollTriggerRate = maxrate;
  if(_fastScrollTriggerRate < _maxFastScrollTriggerRate)
    _fastScrollTriggerRate = _maxFastScrollTriggerRate;
  if(_refreshRate > _fastScrollTriggerRate)
    _refreshRate = _fastScrollTriggerRate;
  if(_refreshRate > _maxFastScrollTriggerRate)
    _refreshRate = _maxFastScrollTriggerRate;

}









