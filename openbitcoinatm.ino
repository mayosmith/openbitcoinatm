/*************************************************************************
 OpenBitcoinATM
  
 OpenBitcoinATM is the first open-source Bitcoin automated teller machine for
 experimentation and education. 
  
 This application, counts pulses from a Pyramid Technologies Apex 5000
 series bill acceptor and interfaces with the Adafruit 597 TTL serial Mini Thermal 
 Receipt Printer.
  
  
 MIT Licence (MIT)
 Copyright (c) 1997 - 2013 John Mayo-Smith for Federal Digital Coin Corporation
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
  
  
  References
  -----------
  
  Thermal printer libraries written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license. Pick up a thermal printer at --> http://www.adafruit.com/products/597
  
  Peter Kropf: https://github.com/pkropf/greenbacks
  
  Thomas Mayo-Smith: http://www.linkedin.com/pub/thomas-mayo-smith/



 *************************************************************************/

#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include "bitcoinlogo.h"
#include "BCAddress.h"
#include "BCPrivate.h"
#include "adaqrcode.h"
#include <avr/pgmspace.h>

 int printer_RX_Pin = 5;  // This is the green wire
 int printer_TX_Pin = 6;  // This is the yellow wire
 
  Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);


 long pulseCount = 0;
 unsigned long pulseTime, lastTime;
 volatile long pulsePerDollar = 4;

void setup(){
  
  Serial.begin(57600); //baud rate for serial monitor
  attachInterrupt(0, onPulse, RISING);
  pinMode(2, INPUT);
  


  Serial.begin(9600);
 
}

void loop(){
 
  if(pulseCount == 0)
 return;
 
  if((millis() - pulseTime) < 2000)
   return;
 
 if(pulseCount == 4)
 printBTC(); //Serial.println ("One Dollar Baby!");  
 //else 
// printError(); //Serial.println ("Error");  
 
 
   pulseCount = 0;
   pulseTime = 0;
}

void onPulse(){
  
int val = digitalRead(2);
pulseTime = millis();

if(val == HIGH)
  pulseCount++;
  
}

void printBTC(){

  pinMode(7, OUTPUT); digitalWrite(7, LOW); // To also work w/IoTP printer
  printer.begin();

  printer.setSize('L');     // Set type size, accepts 'S', 'M', 'L'
  printer.justify('C');
  printer.printBitmap(bitcoinlogo_width, bitcoinlogo_height, bitcoinlogo_data);
  
  printer.setSize('S');     // Set type size, accepts 'S', 'M', 'L'
  printer.justify('C');
  printer.println("Value = 0.002 BTC"); // Print line
  
  printer.justify('C');
   
  printer.setSize('L');     // Set type size, accepts 'S', 'M', 'L'
  printer.println("Open Bitcoin ATM"); // Print line
  printer.justify('L');
  printer.feed(1);
  printer.println("S H A R E"); // Print line
  printer.printBitmap(BCAddress_width, BCAddress_height, BCAddress_data);
  
  printer.feed(1);
  printer.println("S P E N D"); // Print line
  printer.printBitmap(BCPrivate_width, BCPrivate_height, BCPrivate_data);

  printer.setSize('S');     // Set type size, accepts 'S', 'M', 'L'
  printer.justify('C');
  printer.println("This receipt is an official");
  printer.println("Bitcoin paper wallet");
  printer.println("containing digital");
  printer.println("global currency.");
  printer.println("To spend, simply share"); 
  printer.println("the SHARE QR code and");
  printer.println("the SPEND QR code with");
  printer.println("any merchant that");
  printer.println("accepts Bitcoin.");
  printer.println("*** openbitcoinatm.org ***");
  
  printer.feed(1);
  printer.feed(1);

  printer.sleep();      // Tell printer to sleep
  printer.wake();       // MUST call wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
  
}

void printError(){
  pinMode(7, OUTPUT); digitalWrite(7, LOW); // To also work w/IoTP printer
  printer.begin();
 printer.justify('C');
   
  printer.setSize('L');     // Set type size, accepts 'S', 'M', 'L'
  printer.println("Error"); // Print line
 
  printer.sleep();      // Tell printer to sleep
  printer.setDefault(); // Restore printer to defaults
  
}


