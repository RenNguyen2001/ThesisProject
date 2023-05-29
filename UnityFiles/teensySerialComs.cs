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
                                                        
    void Start()    //as implied by the name, commands inside this loop only print once
    {
        //print ("hello");
        //Console.WriteLine("hello");   Console.ReadLine();

        serialObj.DataBits = 8;
        serialObj.BaudRate = 9600;
        
        serialObj.Open();
    }

    // Update is called once per frame
    void Update()
    {
        //              WITH ARDUINO
        //size of byte = 1, char = 2, int = 4 in c#

        print('\n');
        print("packet start");
        //need to identify the first byte
        //perhaps use the falling edge detection of a '\n'

        byte[] value = new byte[7];
        string line = serialObj.ReadLine(); //THIS CAUSES UNITY TO CRASH
        serialObj.Read(value, 0, 7);
        //print('\n');
        //print("values below");
        for (byte i = 0; i < value.Length; i++)
        {
            print(value[i]);
        }
        print("packet end");
        print('\n');    print(line);
        //print ("h");
        //
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
*/
