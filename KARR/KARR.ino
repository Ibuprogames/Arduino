///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// \brief   KARR, el hermano maligno de KITT
//          Ciclo de LEDS simulando fades usando PWM
// \date    06.12.2011
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011 Martin B.R. martin@martinbr.com
//
// Permission is hereby granted, free of charge, to any
// person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the
// Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions of
// the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// Constantes
////////////////////////////////////////////////////////////////////

// Numero de LEDS
const unsigned int LED_COUNT = 10;

// Primer pin usado
const unsigned int START_PIN = 4;

// Tiempo para bajar el brillo de un LED
const unsigned long LED_DELAY_TIME = 1;

// Tiempo para que el puntero se mueva
const unsigned long POINTER_DELAY_TIME = 75;

////////////////////////////////////////////////////////////////////
// Se fija su brillo maximo y se va reduciendo hasta llegar a 0
////////////////////////////////////////////////////////////////////
class Led
{
public:
  Led()
  {
    pin = brightness = 0;
    delayTime = 0;
  }

  void Setup(unsigned int thePin)
  {
    pin = thePin;

    // Ininializa el pin como salida
    pinMode(pin, OUTPUT);
  }

  void Update()
  {
    if (brightness > 0 && (millis() - delayTime >= LED_DELAY_TIME))
    {
      delayTime = millis();
      analogWrite(pin, brightness--);
    }
  }
  
  void SetHigh()
  {
    brightness = 255;
    delayTime = millis();
    analogWrite(pin, brightness);
  }

private:
  unsigned int pin;
  unsigned int brightness;
  unsigned long delayTime;
};


////////////////////////////////////////////////////////////////////
// Se mueve entre el primer LED y el ultimo, fijando a su paso el
// maximo brillo
////////////////////////////////////////////////////////////////////
class Pointer
{
public:
  Pointer()
  {
    currentPosition = 0;
    delaySpeed = time = 0;
    dir = 0;
    
    leds = NULL;
    ledsCount = 0;
  }

  void Setup(unsigned long theDelay, Led *theLeds, unsigned int theLedsCount)
  {
    currentPosition = 0;
    dir = 1;
    delaySpeed = theDelay;
    leds = theLeds;
    ledsCount = theLedsCount;
    time = 0;
  }
  
  void Update()
  {
    if (millis() - time >= delaySpeed)
    {
      leds[currentPosition].SetHigh();

      currentPosition += dir;
      
      if (currentPosition <= 0 || currentPosition >= (ledsCount - 1))
        dir *= -1;

      time = millis();
    }
  }

private:
  int currentPosition;
  unsigned long delaySpeed;
  unsigned long time;
  int dir;

  Led *leds;
  unsigned int ledsCount;
};


Led led[LED_COUNT];
Pointer pointer;

void setup()
{
  pointer.Setup(POINTER_DELAY_TIME, led, LED_COUNT);

  for (int i = 0; i < LED_COUNT; ++i)
    led[i].Setup(i + START_PIN);
}


void loop()
{
  pointer.Update();

  for (int i = 0; i < LED_COUNT; ++i)
    led[i].Update();
}

