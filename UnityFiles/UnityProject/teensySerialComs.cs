using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.IO.Ports;
using System.Threading;



public class teensySerialComs : MonoBehaviour
{
    SerialPort serialObj = new SerialPort("COM10");   //creating an object from the SerialPort class

    //Teensy uses the COM10 port
    int startByte;
    int startCondition;
    int x, y, z;

    void Start()    //as implied by the name, commands inside this loop only print once
    {
        startByte = 0;
        startCondition = 0;

        serialObj.DataBits = 8;
        serialObj.BaudRate = (int)1e6;

        serialObj.Open();
    }

    // Update is called once per frame
    void Update()
    {
        findXYZ();  //finds the x, y and z output from the sensors and changes/transforms the game object
    }

    int findXYZ()
    {
        //              WITH ARDUINO
        //size of byte = 1, char = 2, int = 4 in c#

        byte numOfValues = 6;   //number of bytes being sent (excluding the dummies and '\n' at the end)
        byte[] value = new byte[numOfValues]; //make this the same size as the amount of data being sent from the teensy/arduino
        int[] directionVal = new int[numOfValues / 2];  //stores the directions values x, y and z in an array

        //=============================== START BIT CONDITION CHECK ========================================
        //START BIT CONDITON: 1ST BYTE = 255, 2ND BYTE >= 105

        if (startByte == 1 && serialObj.ReadByte() == 110)
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
        else if (startByte == 1 && serialObj.ReadByte() != 110)
        { startByte = 0; } //indicates that the number after the 1st start bit (255) is not > 105

        //==================================================================================================

        if (startCondition == 1)
        {
            for (byte i = 0; i < directionVal.Length; i++)  //combines the 8 bit usart vals into 16 bit vals
            {
                directionVal[i] = value[i * 2] | ((Int16)value[i * 2 + 1] << 8); print(directionVal[i]);
            }

            print("packet end"); print('\n');
            startCondition = 0;
            transform.localRotation = Quaternion.Euler(directionVal[0], directionVal[1], directionVal[2]);
        }
        return directionVal[0];
    }


}

