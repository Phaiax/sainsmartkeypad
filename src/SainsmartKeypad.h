#ifndef SAINSMART_KEYPAD_H
#define SAINSMART_KEYPAD_H

#include <Arduino.h>

#define SAMPLE_WAIT 8
#define NO_KEY 6
#define UP_KEY 3
#define DOWN_KEY 4
#define LEFT_KEY 2
#define RIGHT_KEY 5
#define SELECT_KEY 1

/* Hardware-Class to read the keypad of the
 *  SainSmart 1602 LCD Keypad Shield for Arduino Duemilanove,
 *  UNO, MEGA2560 and MEGA1280
 *
 * The LCD Keypad uses a resistor network with one analog
 *  input to read the 5 keys.
 *
 *              |-- Arduino AD0 Pin
 *              |
 * VCC -- |2K| -- |330| -- |620| -- |1K| -- |3.3K| --
 *              |        |        |       |         |
 *              R-       U-       D-      L-        S-Key-Button
 *              |--------|--------|-------|---------|-------------------GND
 *
 * Pay attention: In comparisation with the DFR_Key library
 *  on sainsmart.com the value of SAMPLE_WAIT has changed.
 *
 * This Class reads the keys only. For display communication,
 *  use the LiquidCrystal library.
 * Shielded, it has the following pin assignments:
 * LiquidCrystal lcd(8, 9, 4, 5, 6, 7)
 */

class SainsmartKeypad
{
  public:
    SainsmartKeypad(int rate, int pin);
    SainsmartKeypad(int pin);
    SainsmartKeypad();

    /*
     *  ## This is the method you are probably looking for.
     *
     *  This method behaves like getKey_waitrelease() exept for
     *  keys pressed longer than setMsToActivateFastScroll(><).
     *  Then it starts to trigger the key at given rate
     *  (setFastScrollTriggerRate), going faster with time until
     *  the given maximum is reached. Returns SAMPLE_WAIT if
     *  nothing to do. It does not "fastscroll" the NO_KEY-Key.
     *
     *  USAGE: in loop():
     *    int key=keypad.getKey_fastscroll();
     *    if(key != SAMPLE_WAIT && key != NO_KEY)
     *      { handle_key(key); }
     */
    int getKey_fastscroll();

    /*
     *  ## This is the method you are probably looking for.
     *
     *  This method returns a pressed key only once. It returns
     *  SAMPLE_WAIT until the key is released. Then it returns
     *  NO_KEY once, and SAMPLE_WAIT until next key is pressed.
     *
     *  USAGE: in loop():
     *    int key=keypad.getKey_periodic();
     *    if(key != SAMPLE_WAIT && key != NO_KEY)
     *      { handle_key(key); }
     */
    int getKey_waitrelease();

    /*
     *  Probably you don't want to use this method.
     *
     *  Get current key (or NO_KEY) every <RefreshRate> ms.
     *  Returns SAMPLE_WAIT inbetween.
     *  USAGE: in loop():
     *    int key=keypad.getKey_periodic();
     *    if(key != SAMPLE_WAIT && key != NO_KEY)
     *      { handle_key_FAST(key); }
     */
    int getKey_periodic();

    // Calculates and returns the pressed Key instantly.
    // This is the method you want to use least or only in special cases.
    int getKey_instant();

    // method to customize the analog read values
    void setAnalogReadValues(int arv_select, int arv_left, int arv_up,
      int arv_down, int arv_right, int arv_nokey);

    // set the rate to calculate and refresh the analog key input.
    void setRefreshRate(int rate);

    // time after keypress to enable fast_scroll
    void setMsToActivateFastScroll(int ms);

    // rate in ms. A rate of 40 means to trigger a key event every 40ms.
    // automatically set's the updateRate if maxrate is lower.
    void setFastScrollTriggerRate(int rate, int maxrate);


  private:
    void _init();
    int _curInput;

    int _refreshRate;
    int _keyPin;

    int arv_select_;
    int arv_left_;
    int arv_up_;
    int arv_down_;
    int arv_right_;
    int arv_nokey_;

    int _msToActivateFastScroll;
    int _fastScrollTriggerRate;
    int _maxFastScrollTriggerRate;

    int _curKey;
    int _prevKey;
    unsigned long _lastKeyChangeTime;
    unsigned long _lastFastScrollTime;
    unsigned long _oldTime;
};

#endif // SAINSMART_KEYPAD_H
