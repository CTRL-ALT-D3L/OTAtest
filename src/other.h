
#pragma once

#include <Arduino.h>

namespace charArray
{

    // info about the char array:

    // gets the length of a char array
    int len(const char *arr)
    {
        int out = 0;
        while (true)
        {
            if (arr[out] == '\0')
                return out;
            out++;
        }
    }

    // is the character invalid
    bool isCharInvalid(char c)
    {
        return !(c >= 0 and c < 128);
    }

    // is the array invalid
    bool isInvalid(const char *arr)
    {
        int lenArr = len(arr);
        for (uint8_t i = 0; i < lenArr; i++)
        {
            if (isCharInvalid(arr[i]))
                return true;
        }
        return false;
    }

    // is the array empty
    bool isEmpty(const char *arr)
    {
        return arr[0] == '\0';
    }

    // is the array kosher (not invalid or empty)
    bool isKosher(const char *arr)
    {
        return !isEmpty(arr) and !isInvalid(arr);
    }

    // char array modification:

    // adds arr2 to arr1
    // make sure that arr1 has enough space
    void addArray(char *arr1, char *arr2)
    {
        int lenArr1 = len(arr1);
        int lenArr2 = len(arr2);
        for (int i = 0; i < lenArr2; i++)
        {
            arr1[lenArr1 + i] = arr2[i];
        }
    }

    // adds c to arr
    void addChar(char *arr, char c)
    {
        arr[len(arr)] = c;
    }

    // fills arr with c
    void fill(char *arr, int totalLen, char c)
    {
        for (int i = 0; i < totalLen; i++)
            arr[i] = c;
    }

    // turns arr to an int
    // make sure arr only has numbers in it
    int toInt(const char *arr)
    {
        int lenArr = len(arr);
        int output = 0;
        for (uint8_t i = 0; i < lenArr; i++)
            output += (arr[i] - 48) * pow(10, lenArr - i - 1);
        return output;
    }

    // cuts arr from start to end and puts it into buffer
    // make sure buffer is longer than end - start
    // this function is 2 inputs longer than it has to be because cpp is retarded
    // "you have more freedom" my ass
    void substring(char *arr, int start, int end, char *buffer, int totalBufferLen)
    {
        int lenArr = len(arr);
        if (totalBufferLen <= end - start)
            return;
        for (int i = 0; i < lenArr; i++)
        {
            if (i == start)
            {
                fill(buffer, totalBufferLen, '\0');
                for (int j = 0; j < end - start; j++)
                    buffer[j] = arr[i + j];
                return;
            }
        }
    }

    // gets the index of arr2s first appearance in arr1
    // will return -1 if it didnt find anything
    int indexOf(const char *arr1, const char *arr2)
    {
        // loop through arr1 (i)
        //     if the length of arr2 is greater than the rest of arr1 and we still didnt find a match
        //          break
        //     if the first letter matches
        //          loop through the other letters
        //              if the current letter doesnt match
        //                  break
        //              if we've reached the last letter of arr2
        //                  return i
        //  if no matches
        //     return -1 :D
        int lenArr1 = len(arr1);
        int lenArr2 = len(arr2);
        for (int i = 0; i < lenArr1; i++)
        {
            if (lenArr2 > lenArr1 - i)
                break;
            if (arr1[i] == arr2[0])
            {
                for (int j = 0; j < lenArr2; j++)
                {
                    if (arr1[i + j] != arr2[j])
                        break;
                    else if (lenArr2 == j + 1)
                        return i;
                }
            }
        }
        return -1;
    }

    //
}

namespace floatFuncs
{
    float truncate(float input)
    {
        return int(input * 10) / 10.0f;
    }
}
