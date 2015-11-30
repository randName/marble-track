template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

const byte LED_PIN[] = { 3, 5, 6 };
#define LED_GND 4

#define LDR_PIN A0
#define LDR_GND A1

#define LDR_THRESH 50
#define DEBOUNCE 100

#define PASS_RESET 550

void setup()
{
  pinMode( LDR_GND, OUTPUT ); digitalWrite( LDR_GND, LOW );
  pinMode( LED_GND, OUTPUT ); digitalWrite( LED_GND, LOW );
  for ( byte i=0; i<3; i++ ) pinMode( LED_PIN[i], OUTPUT );
}

void loop()
{
  static bool triggered, last_triggered;
  static byte i, passed = 0;
  static byte c = 0, rgb[] = {0,0,0}, dir[] = {0,2,1};
  static int ldr, last_ldr = analogRead( LDR_PIN ), k;
  static unsigned long last_debounce = millis();

  ldr = analogRead( LDR_PIN );
  triggered = ( abs( ldr - last_ldr ) > LDR_THRESH );
  
  if ( ( millis() - last_debounce ) > DEBOUNCE )
  {
    if ( triggered != last_triggered )
    {
      last_debounce = millis();
      
      if ( triggered && ldr > last_ldr )
      {
        passed++;
        k = 0; // reset passing timer
      }
    }
  }

  last_ldr = ldr;
  last_triggered = triggered;
  
  if ( passed ) k++; // passing timer

  c++; // LED frame counter
  
  for ( i=0; i<3; i++ )
  {
    if ( passed )
    {
      // Magic.
      rgb[i] = ( i == 2 - ( passed - 1 ) % 3 ) ? 255 : 0 ;

      // Basically,
      //    2 - ( passed - 1 ) % 3
      // splits the "passed" counter into 3 cases (0, 1, 2)
      // and it lights up the corresponding LED
      
      if ( k > PASS_RESET ) rgb[i] = 0;
    }
    else
    {
      if ( c ) // update LED values
      {
        rgb[i] += ( dir[i] - 1 );
      }
      else // frame counter back to 0 (overflow)
      {
        // cycle directions
        // 0->1, 1->2, 2->0
        dir[i] = ( dir[i] + 1 ) % 3;
      }
    }
    analogWrite( LED_PIN[i], rgb[i] );
  }

  if ( k > PASS_RESET ){ passed = 0; c = 0; k = 0; }

  delay(10);
}
