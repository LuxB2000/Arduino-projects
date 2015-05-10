/*
  Simple project that
  - blink D diodes, each one has it own period (in milli-second) 
  - the period is determined by potentiometers
 */

const int D = 1; // nbr of diodes, and potentiometers

int startDiodePin = 8; // access to each diod with startDiodPin + d, d \in [0,D]
int startPotPin = 0; // access each potentiometer with startPotPin + d, d \in [0,D]

unsigned long last_blink_time[D];
int M = 10; // duration of diodes blinking time, in milli-seco
float Q = 1.0; // quantification step of the periode

/* -----
 * SETUP
 * -----
 */
void setup() {
  int d = 0;
  Serial.begin(9600);      // open the serial port at 9600 bps:
  
  for( d=0; d<D; d++ ){
    // declare output diodes
    pinMode( startDiodePin+d, OUTPUT );
    // declare input potentiomenter
    pinMode( startPotPin, INPUT);
    // init last blinking time
    last_blink_time[d] = millis();
  }
}

/*
  long get_period(int d)
  returns, in milli-second, the period measured on the dth potentiometer.
*/
unsigned long get_period(int d){
  unsigned long p=(unsigned long) Q;
  
  // analogRead takes about 100 microseconds (0.0001 s) to read an analog input
  // it returns int (0 to 1023) 
  int meas = analogRead( startPotPin+d );
  
  p = (unsigned long) ( ( meas )*Q ); // TODO fix Q
  
  return p;
}

/*
  bool blink_diode(d)
  blink the dth diode for M milli-seconds, returns true if no error
*/
bool blink_diode(int d){
  bool ok = true;

  digitalWrite( startDiodePin+d, HIGH );
  delay( M );
  digitalWrite( startDiodePin+d, LOW );
  
  return ok;
}

/* ----
 * LOOP
 * ----
 */
void loop() {
  unsigned long time = 0, period_d = 0;
  int d = 0;
  
  for (d=0; d<D; d++ ){
    // check period_d value
    time = millis();
    period_d = get_period(d); // > 0.1ms to exec
    //Serial.print("Period - ");
    //Serial.print(d);
    //Serial.print(": ");
    //Serial.println(period_d);
    if( (time - last_blink_time[d]) > period_d ){
      blink_diode(d);
      last_blink_time[d] = time;
    }// end if`
  }// end for d
  
} // end loop

