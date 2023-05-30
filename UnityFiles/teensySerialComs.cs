using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.IO.Ports;
using System.Threading;



public class teensySerialComs : MonoBehaviour
{
    SerialPort serialObj = new SerialPort("COM3");   //creating an object from the SerialPort class
    //Teensy uses the COM10 port
    int startByte;
    int startCondition;
                                                        
    void Start()    //as implied by the name, commands inside this loop only print once
    {
        //print ("hello");
        //Console.WriteLine("hello");   Console.ReadLine();
        startByte = 0;
        startCondition = 0;

        serialObj.DataBits = 8;
        serialObj.BaudRate = (int)1e6;
        
        serialObj.Open();
    }

    // Update is called once per frame
    void Update()
    {
        //              WITH ARDUINO
        //size of byte = 1, char = 2, int = 4 in c#


        //need to identify the first byte
        //perhaps use the falling edge detection of a '\n' - THIS DIDN'T WORK
        //perhaps send a dummy byte - PARTIALLY WORKED

        byte numOfValues = 6;   //number of bytes being sent (excluding the dummies and '\n' at the end)
        byte[] value = new byte[numOfValues]; //make this the same size as the amount of data being sent from the teensy/arduino
        int[] directionVal = new int[numOfValues/2];  //stores the directions values x, y and z in an array

        //=============================== START BIT CONDITION CHECK ========================================
        //START BIT CONDITON: 1ST BYTE = 255, 2ND BYTE >= 105

        if (serialObj.ReadByte() == 110)
        {
            serialObj.Read(value, 0, numOfValues);  //minus the number of start bits
            startCondition = 1; startByte = 0;
            print("packet start"); 
        }

        if (startByte == 0 && serialObj.ReadByte() == 255)  //if dummy byte is found, the value is valid/correct
        {
            //the max value of each 16 bits value (8 bits combined to form the 16-bit val) is 360 (max angle)
            //store this byte for now and wait to see if the byte after this is > 105 (360-105)
            //if its true than the start condition is true and unity can store the data in the line
            print("first start byte detected");
            startByte = 1;
        }
        else if(startByte == 1 && serialObj.ReadByte() != 110)
        { startByte = 0; } //indicates that the number after the 1st start bit (255) is not > 105

        //==================================================================================================

        if (startCondition == 1)
        {
            
            for (byte i = 0; i < numOfValues; i++)
            {
                
                print(value[i]);
            }
            //directionVal[0] = value[0] | ((UInt16)value[1] << 8);  print(directionVal[0]);
            //directionVal[0] = value[0] | ((UInt16)value[1] << 8); print(directionVal[0]);

            print("packet end"); print('\n');
            startCondition = 0;
        }
        
    }

    
}

//solutions tested
/*
 * 29/05 before 11:00pm
    1. Changed the microcontroller from the teensy to the arduino
        Problem still occurs
    2. Wait for new data to enter before reading the data into a variable
        Serial events don't work in unity
    Problem found:
    The serial print line command in the arduino problem was causing errors in the uart signals
    
    29/05 after 11:00pm
    1. Using the Read() function to store th uarts bytes into an array
        prints fine except that the first value (1) isn't printed with the rest of the array
        for example it prints: 44, 5, 44, 14, 44 instead of 1, 44, 5, 44, 14, 44
    Used a dummy byte, this partially removed the problem. However after each successfull print, there is
    an array print of just 0s along with the dummy byte.
    For example let the dummy byte be 10, the print would be: 10, 0, 0, 0, 0, 0

    30/05
    -Added an if statement to check if the first byte is the dummy byte. Only then
    will the whole line be read into a byte array.
    This has solved the error on 29/05 completely

    - Tried to add multi-byte transmission
*/
