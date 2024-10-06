//
//  5V       5/A5
//  GND      6
//  3V3      7
//  4/A4     8/SDA
//  3/A3     9/SCL
//  2/A2     10
//  1/A1     20
//  0/A0     21
// define led according to pin diagram

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Adafruit_TinyUSB.h>
//#include "USB.h"
//#include "USBHID.h"
//USBHID HID;

//#define ADC_ONESHOT_FORCE_USE_ADC2_ON_C3 Enable


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1

#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define LED 8

#define J1V A5 //Z 
#define J1H A4 //ZR
#define J2V A2 // up joy y
#define J2H A3 // up joy x
#define J3V A1 // down joy y
#define J3H A0 // down joy x

#define SW0 6
#define SW1 7
#define SW2 10
#define SW3 20
#define SW4 21

int JOY[6];

static const uint8_t report_descriptor[] = {
//static const uint8_t desc_hid_report[] PROGMEM = {
  0x05, 0x01,           //  Usage Page (Generic Desktop)
  0x09, 0x08,           //  0x08: Usage (Multi-Axis)
  0xa1, 0x01,           //  Collection (Application)
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x01,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x30,           //    Usage (X)
  0x09, 0x31,           //    Usage (Y)
  0x09, 0x32,           //    Usage (Z)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x02,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x33,           //    Usage (RX)
  0x09, 0x34,           //    Usage (RY)
  0x09, 0x35,           //    Usage (RZ)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
/* 
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x03,           //  Report ID
  0x15, 0x00,           //   Logical Minimum (0)
  0x25, 0x01,           //    Logical Maximum (1)
  0x75, 0x01,           //    Report Size (1)
  0x95, 32,             //    Report Count (24)
  0x05, 0x09,           //    Usage Page (Button)
  0x19, 1,              //    Usage Minimum (Button #1)
  0x29, 32,             //    Usage Maximum (Button #24)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,
*/
  0xC0,
  //TUD_HID_REPORT_DESC_KEYBOARD(0x04)
0x05, 0x01,                         // Usage Page (Generic Desktop)
0x09, 0x06,                         // Usage (Keyboard)
0xA1, 0x01,                         // Collection (Application)
0x85, 0x03,           //  Report ID
//0x85, 0x04,           //  Report ID
0x05, 0x07,                         //     Usage Page (Key Codes)
0x19, 0xe0,                         //     Usage Minimum (224)
0x29, 0xe7,                         //     Usage Maximum (231)
0x15, 0x00,                         //     Logical Minimum (0)
0x25, 0x01,                         //     Logical Maximum (1)
0x75, 0x01,                         //     Report Size (1)
0x95, 0x08,                         //     Report Count (8)
0x81, 0x02,                         //     Input (Data, Variable, Absolute)

0x95, 0x01,                         //     Report Count (1)
0x75, 0x08,                         //     Report Size (8)
0x81, 0x01,                         //     Input (Constant) reserved byte(1)

0x95, 0x05,                         //     Report Count (5)
0x75, 0x01,                         //     Report Size (1)
0x05, 0x08,                         //     Usage Page (Page# for LEDs)
0x19, 0x01,                         //     Usage Minimum (1)
0x29, 0x05,                         //     Usage Maximum (5)
0x91, 0x02,                         //     Output (Data, Variable, Absolute), Led report
0x95, 0x01,                         //     Report Count (1)
0x75, 0x03,                         //     Report Size (3)
0x91, 0x01,                         //     Output (Data, Variable, Absolute), Led report padding

0x95, 0x06,                         //     Report Count (6)
0x75, 0x08,                         //     Report Size (8)
0x15, 0x00,                         //     Logical Minimum (0)
0x25, 0x65,                         //     Logical Maximum (101)
0x05, 0x07,                         //     Usage Page (Key codes)
0x19, 0x00,                         //     Usage Minimum (0)
0x29, 0x65,                         //     Usage Maximum (101)
0x81, 0x00,                         //     Input (Data, Array) Key array(6 bytes)


0x09, 0x05,                         //     Usage (Vendor Defined)
0x15, 0x00,                         //     Logical Minimum (0)
0x26, 0xFF, 0x00,                   //     Logical Maximum (255)
0x75, 0x08,                         //     Report Count (2)
0x95, 0x02,                         //     Report Size (8 bit)
0xB1, 0x02,                         //     Feature (Data, Variable, Absolute)

0xC0,                                // End Collection (Application)


    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x04,           //  Report ID
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};

//Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 4, false);

#if 0
class CustomHIDDevice: public USBHIDDevice {
public:
  CustomHIDDevice(void){
    static bool initialized = false;
    if(!initialized){
      initialized = true;
      HID.addDevice(this, sizeof(report_descriptor));
    }
  }
  
  void begin(void){
    HID.begin();
  }
    
  uint16_t _onGetDescriptor(uint8_t* buffer){
    memcpy(buffer, report_descriptor, sizeof(report_descriptor));
    return sizeof(report_descriptor);
  }

  bool send(uint8_t * value){
    return HID.SendReport(0, value, 8);
  }
};

CustomHIDDevice Device;
#endif

const int buttonPin = 0;
int previousButtonState = HIGH;
uint8_t axis[8];

void Joysread(){

  char buf[100];

  JOY[0] = analogRead(J3H);
  JOY[1] = analogRead(J3V);
  JOY[2] = analogRead(J2H);
  JOY[3] = analogRead(J2V);
  JOY[4] = analogRead(J1H);
  JOY[5] = analogRead(J1V);

  sprintf(buf, "V1=%d, H1=%d ,", JOY[5], JOY[4]);
  Serial.print(buf);
  sprintf(buf, "V2=%d, H2=%d ,", JOY[3], JOY[2]);
  Serial.print(buf);
  sprintf(buf, "V3=%d, H3=%d ,", JOY[1], JOY[0]);
  Serial.println(buf);
  
}

void setup() {

  //usb_hid.begin();
  //while( !TinyUSBDevice.mounted() ) delay(1);

  //Device.begin();
  //USB.begin();


  // initialize digital pin led as an output
  pinMode(LED, OUTPUT);

  pinMode(J1V, INPUT);
  pinMode(J1H, INPUT);
  pinMode(J2V, INPUT);
  pinMode(J2H, INPUT);
  pinMode(J3V, INPUT);
  pinMode(J3H, INPUT);

  Serial.begin(9600);

#if 1
  //if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
  if(!oled.begin()){
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }else{
    Serial.println("SSD1306 succeed");
    //oled.begin();
    oled.clearDisplay();
    //oled.drawFastHLine(32, 5, 48, SSD1306_WHITE); // 绘制水平线
    oled.setRotation(2);
    oled.setTextSize(2);          // 設定文字大小
    oled.setTextColor(1);         // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
    oled.setCursor(20,0);          // 設定起始座標
    oled.print("Joys 6Dof");     // 要顯示的字串
    oled.display();               // 要有這行才會把文字顯示出來
  }
#endif

}



void loop() {
  digitalWrite(LED, HIGH);   // turn the LED off
  delay(1000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED on
  delay(1000);               // wait for a second

#if 0
  //Serial.println("SSD1306 succe");
  JOY[0] = analogRead(J3H);
  JOY[1] = analogRead(J3V);
  JOY[2] = analogRead(J2H);
  JOY[3] = analogRead(J2V);
  JOY[4] = analogRead(J1H);
  JOY[5] = analogRead(J1V);
#endif
  Joysread();

}
