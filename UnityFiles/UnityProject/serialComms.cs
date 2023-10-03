using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.IO.Ports;
using System.Threading;

public class serialComms : MonoBehaviour
{
    SerialPort serialObj = new SerialPort("COM10");   //creating an object from the SerialPort class

    // Start is called before the first frame update
    void Start()
    {
        serialObj.DataBits = 8;
        serialObj.BaudRate = (int)120000;

        serialObj.Open();
    }

    // Update is called once per frame
    void Update()
    {
        string serialData = serialObj.ReadLine(); //print(serialData);

        //divide the string into several parts depending on the space between each number
        string[] serialValue = serialData.Split(' '); //print(serialValue[0]); print(" "); print(serialValue[1]); print(" "); print(serialValue[2]); print(" "); print(serialValue[3]);

        //convert each string to a float
        float[] number = new float[4];
        for(int i = 0; i < 4; i++)
        {
            number[i] = Convert.ToSingle(serialValue[i]);
            //print(number[i]); print(" ");
        }
        print(number[2]); print(" ");

        //rotating the object
        transform.localRotation = Quaternion.Euler(number[2], 0, 0);
       
    }
}
