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

        //need to identify the first byte
        int value = serialObj.ReadByte();
        print('\n');
        print("values below");
        print (value);
        print ("h");
        //
    }

    
}

//solutions tested
/*
    1. Changed the microcontroller from the teensy to the arduino
        Problem still occurs
    2. Wait for new data to enter before reading the data into a variable
        Serial events don't work in unity
    Problem found:
    The serial print line command in the arduino problem was causing errors in the uart signals
    
*/
