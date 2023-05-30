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

        byte numOfValues = 5;
        byte numOfDirectionVectors = 3; //number of direction parameters (e.g. x, y and z)
        byte[] value = new byte[numOfValues]; //make this the same size as the amount of data being sent from the teensy/arduino
        byte[] directionVal = new byte[numOfDirectionVectors];  //stores the directions values x, y and z in an array

        if (serialObj.ReadByte() == 0)  //if dummy byte is found, in otherwords if the value valid/correct
        {
            print("packet start");
            serialObj.Read(value, 0, numOfValues);    //print("valid value found");
        }
        

        int j = 0;
        for (byte i = 0; i < value.Length; i++)
        {
            switch(value[i])    //to sort through the array to find the useful values
            {
                case 44: break;     // ','
                case 0x0A: break;   //'\n'
                case 0:    break;   //dummy byte    
                default: directionVal[j] = value[i]; print(directionVal[j]); j++;  break;  //valid results/ values that we want to extract
            }
            //print(value[i]);
        }

        if (j == numOfDirectionVectors) { print("packet end"); print('\n'); }   //put outside loop so it only checks once and doesn't slow down code
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
*/
