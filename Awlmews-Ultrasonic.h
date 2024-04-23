#ifndef AWLMEWS_ULTRASONIC
#define AWLMEWS_ULTRASONIC


using ul = unsigned long;
using ull = unsigned long long;


class Ultrasonic{
private:
  bool initialized = false;
  char trigger_pin, echo_pin;
  ul timeout;

  void ping(){
    digitalWrite(trigger_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin,LOW);
  }
public:
  Ultrasonic() {}
  Ultrasonic(char trigger_pin_, char echo_pin_, ul timeout_ = 40000){
    initialize(trigger_pin_,echo_pin_,timeout_);
  }

  void initialize(char trigger_pin_, char echo_pin_, ul timeout_ = 40000){
    timeout = timeout_;

    trigger_pin = trigger_pin_;
    echo_pin = echo_pin_;

    pinMode(trigger_pin,OUTPUT);
    pinMode(echo_pin,INPUT);

    digitalWrite(trigger_pin,LOW);
  }

  float read(){
    ping();

    ul duration = pulseIn(echo_pin,HIGH);
    if(duration > timeout){
      return 0.0;
    }
    return ((float)duration/2.0)/29.1;
  }

  float read_averaged(int count){
    float average = 0;
    for(int i = 0; i < count; ++i){
      float reading = read();
      if(reading == 0){
        count --;
      }
      average += reading;
      delay(100);
    }
    return average / max(count,1);
  }
};

#endif