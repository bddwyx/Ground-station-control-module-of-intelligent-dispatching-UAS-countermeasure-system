//IN1--3, IN2 -- 5, RADAR_OUT -- A0, dtheta -- 200;
//转到给定角度后返回角度-速度数组

#include <Wire.h>
#include <Servo.h>

#define ARRAY_SIZE 5 //雷达得到的速度平均值用的
#define PITCH1 9
#define PITCH2 10
#define IN1 3
#define IN2 5
#define RADAR_OUT A0
#define dt 999

/*Positive or negative, according to position. NOTICE THAT 4095/0-LEAP-VELOCITY SHOULD ALSO BE MODIFIED*/
#define IG 0.3
#define PG 0.5

bool is_instruction_changed = false;

long int result = 0, new_result = 0, printed_result = 0; //position (yaw)
long int t = 0;
long int total = 0; 
int aim = 100;
int count = 0;
int pitch = 75; // pitch > 24 and pitch < 180  !!!!! 建议把 24-102 <=> 0-90 (102是真的90但24不是0)
int boundary = 0;
int radar_value = 0;
int new_radar_val = 0;  //change = 1
int period = 0;
int space_count = 0;  //times of the maintanance of radar value (0 of course) detected
long int dt1 = 0, dt2 = 0;
int result_array[2*ARRAY_SIZE] = {0};
float data = 0;
int yaw = 0;
int mode = 1;
int previous_mode = 1;
 
float velocity_array[ARRAY_SIZE];
float average_velocity_array[20] = {0};
float average_velocity = 0.0;
float velocity = 0.0;
float sum = 0;

double correction = 30;

Servo pitch1, pitch2;

struct spid{ 
  double dState; // Last position input 
  double iState; // Integrator state 
  double iMax, iMin; // Maximum and minimum of integrator state
  double iGain, // integral gain 
         pGain, // proportion gain 
         dGain; // derivation gain 
} ; 
  
spid pidc;


double updatepid(spid pid, double error, long int dtt) {
   double pTerm = 0, iTerm = 0; 
  
  // calculate the integral state
  pid.iState += error * (dtt / 1000); 
  //limit intergral value
  if (pid.iState > pid.iMax) 
    pid.iState = pid.iMax; 
  else if (pid.iState < pid.iMin)
    pid.iState = pid.iMin; 
    iTerm = (pid.iGain) * (pid.iState); // calculate the integral term
    //Serial.print("iTerm:");
    //Serial.println(iTerm);

  // = pid.dGain * (c_position - pid.dState); 
  //pid.dState = c_position;
  
  pTerm = pid.pGain * error; 
  //Serial.print("pTerm:");
  //Serial.println(pTerm);
  return pTerm + iTerm; 
}

// motor_velocity(IN1, IN2,velocity);  ---(IN1 positive IN2 negative!!!!)
// only abs(v) > 70 will do 
void motor_velocity(int pin_positive, int pin_negative, int v){
  
    if(v > 255)v = 255;
    else if(v < -255)v = -255;
    else if(0 < v and v < 50) v = 50;
    else if( -50 < v and v < 0) v = -50;

    if (v >= 0)
    {analogWrite(pin_positive, v); analogWrite(pin_negative, 0);} 
    else
    {analogWrite(pin_negative, -v); analogWrite(pin_positive, 0);} 
}

int read_position(){
  
  byte i = 0;
  byte rx_Num = 0;  // the bytes of received by I2C
  byte rx_buf[12] = {0}; // received buffer by I2C

  Wire.beginTransmission(0x36); // Begin a transmission with the address
  Wire.write(12); // Data Length
  Wire.endTransmission(0);  // Send a START Sign

  // Wire.requestFrom（AA,BB）;receive the data form slave.
  // AA: Slave Address ; BB: Data Bytes 
  rx_Num = Wire.requestFrom(0x36, 12); 

  // Wire.available: Returns the number of bytes available for retrieval with read().
  while(Wire.available())
  {
      rx_buf[i] = Wire.read(); // received one byte
      i++;
  }
  new_result = rx_buf[0]*256+rx_buf[1];

  return new_result;
  }



void setup() {
  Serial.begin(38400);
  //Initiate the Wire library.
  Wire.begin(); 
  //Serial.print("Yaw \t average_velocity \r\n");

  //initialize struct pid
  pidc.dState = 0;
  pidc.iState = 0;
  pidc.iMax = 700;  //integrational limit
  pidc.iMin = -700; //integrational limit
  pidc.iGain = IG;
  pidc.pGain = PG;
  pidc.dGain = 0;

  pitch1.attach(PITCH1);
  pitch2.attach(PITCH2);


}


void loop() {
  
    if(Serial.available()){mode = Serial.read();}
    if(mode < 1 or mode > 80){mode = 1;};
 
    pitch1.write(pitch);
    pitch2.write(204-pitch);
       
    t = millis();
    dt1 = micros();
    boundary = t % dt;
       
      if(boundary <= dt*2/3 and boundary >= 6){
        radar_value = 0;
        new_radar_val = 0; //change = 1
        period = 0;
        space_count = 0; //times of the maintanance of radar value (1023 of course) detected

        for(int i = 0; i < ARRAY_SIZE; i++){velocity_array[i] = 0;};
        for(int i = 0; i < 19; i++){average_velocity_array[i] = 0;};
      
        velocity_array[ARRAY_SIZE] = {0};
        average_velocity = 0.0;
        velocity = 0.0;
        sum = 0;
        
        new_result = read_position();
        if(new_result < 4096 and new_result >= 0){
        result = 0.85 * result + 0.15 * new_result;
        /*for(int i = 0; i < 2*ARRAY_SIZE-1; i++){
          result_array[i] = result_array[i+1];
          };
        result_array[2*ARRAY_SIZE-1] = result;
        for(int i = 0; i < 2*ARRAY_SIZE; i++){
          printed_result += result_array[i];
          };
        printed_result /= (2*ARRAY_SIZE);
        printed_result = 0;
        
        //Serial.print(printed_result);
        //Serial.print('\t');
        //Serial.println(aim);*/
      
        if(result - aim < -2200)motor_velocity(IN1,IN2,168);//or 168, according to actual position
        else if (result - aim > 2200)motor_velocity(IN1,IN2,-168);//or -168, according to actual position
        else{
          correction = updatepid(pidc, double(result - aim), dt1-dt2);
          //Serial.println(correction);
          result = new_result;
          motor_velocity(IN1,IN2,int(correction));}
   
        dt2 = dt1; delay(4);}}

        
     else if (boundary > dt*2/3){
        motor_velocity(IN1,IN2,0);
        new_radar_val = analogRead(RADAR_OUT); //only 0 or 1023: LED off -- 1023, LED on -- 0;
       
        /*探测不到物体时led off， 当拍为加强时LED on, 但LED on的时间相对于 LED off 的时间很短，可以看做一个脉冲*/
        if(radar_value - new_radar_val < 600)period += 1; //'rise'
        else{
          velocity = 100/period;
          period = 0;
          velocity_array[ARRAY_SIZE-1] = velocity;
          for(int i = 0; i < ARRAY_SIZE-1; i++){velocity_array[i] = velocity_array[i+1];}
          space_count = 0;
          }
          
        //wait too long without radar value change means that no object detected
        if(period > 16){
          period = 0;
          velocity = 0.0;
          velocity_array[ARRAY_SIZE-1] = velocity;
          for(int j = 0; j < ARRAY_SIZE-1; j++){velocity_array[j] = velocity_array[j+1];}
          space_count += 1;
          }
          
        for(int k = 0; k < ARRAY_SIZE; k++){
          sum += velocity_array[k];
        }
        average_velocity = sum / ARRAY_SIZE;
        if(space_count == 2){average_velocity = 0;} //空了两次就认为没有物体了
        total += average_velocity;
        count += 1;
        //Serial.println(average_velocity);
        
        radar_value = new_radar_val;
        sum = 0.0;
        average_velocity = 0;
        delay(1);
     }
     
    //一轮结束计算这个周期里雷达探测到物体平均速度 
    if(boundary % dt < 6 and count != 0){
     sum = 0;
     is_instruction_changed = (previous_mode == mode ? false : true);
     
     //Serial.print(aim);
     //Serial.print('\t');
     data = 33.0 * float(total)/float(count);
     if(data > 40){data = 40;};
     //data = 40;
     
     Serial.print(int((50+pow(1.94,data/5))*cos((double(aim)/4100*360)* 3.142/180)));
     Serial.print('\t');
     Serial.print(int((50+pow(1.94,data/5))*sin((double(aim)/4100*360)* 3.142/180)));
     Serial.print('\t');
     Serial.println(pitch);
     
     //Serial.println(mode);
     //Serial.println(pitch);
     
     if(mode >= 1 and mode <= 20){pitch = 75; aim = 200 * mode - 100;}
     else if(mode >= 21 and mode <= 40){pitch = 75; aim += 200; if(is_instruction_changed){aim = 200 * (mode-20) - 100;};}
     else if(mode >= 41 and mode <= 60){pitch = 85; aim = 200 * (mode-40) - 100;}
     else if(mode >= 61 and mode <= 80){pitch = 85; aim += 200; if(is_instruction_changed){aim = 200 * (mode-80) - 100;};}
        
     if(aim > 4095) aim = 100;
     count = 0; total = 0;

     previous_mode = mode;
     
    }
} 
