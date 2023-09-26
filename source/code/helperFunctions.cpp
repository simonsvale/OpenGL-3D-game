#include <string>
#include <vector>

#include <iostream>

#include "helperFunctions.h"

using namespace std;

// Function for removing spaces in strings
string HelperFunctions::RemoveSpaces(string String)
{
    string ReturnString;

    // Go through each charecter of the string
    for(int CharNum = 0; CharNum < String.length();)
    {   
        // Check if the element is a string.
        if(String[CharNum] != ' ')
        {   
            // If it is not, push it to the the return string.
            ReturnString.push_back(String[CharNum]);
        }

        CharNum++;
    }
    
    return ReturnString;
}

// Splits a string by a given character delimiter in ASCII, and returns the split string in a vector, without the delimiter.
vector<string> HelperFunctions::SplitByDelimiter(string String, char Delimiter)
{
    vector<string> SplitStringVector;

    // Run through all the content of the file, and check for delimiters.
    for(int Char = 0; Char < String.length();)
    {   
        // Check if the character is the delimiter ';'.
        if(String[Char] == Delimiter)
        {
            // Line until the delimiter into a vector.
            SplitStringVector.push_back(String.substr(0, Char));

            // Erase that part of the string
            String.erase(0, Char+1);
            
            // Reset the char counter, since all of the string up till this point have been erased.
            Char = 0;
        }
        else
        {
            Char++;
        }
    }
    
    return SplitStringVector;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint8_t throw exception. */
int HelperFunctions::GetAtrisKeyValue_uint8_t(string Key, vector<string> StringVector)
{   
    // Return variable
    int KeyValue;

    // Iterate through all strings in the vector.
    for(int StrNum = 0; StrNum < StringVector.size();)
    {   
        if(StringVector[StrNum].length() >= Key.length())
        {
            // Check for key in string
            if((StringVector[StrNum].find(Key) != string::npos) == true)
            {   
                try
                {   
                    // Get value from key
                    KeyValue = stoi(StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1));
                }
                catch(invalid_argument)
                {
                    throw invalid_argument(string(Key)+"'s value is not of type uint8_t.");
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 255))
                {
                    throw out_of_range(string(Key)+"'s value is out of range of type uint8_t (0 - 255).");
                }
                
                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("key: "+string(Key)+", does not exist in input string.");
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint16_t throw exception. */
int HelperFunctions::GetAtrisKeyValue_uint16_t(string Key, vector<string> StringVector)
{   
    // Return variable
    int KeyValue;

    // Iterate through all strings in the vector.
    for(int StrNum = 0; StrNum < StringVector.size();)
    {   
        if(StringVector[StrNum].length() >= Key.length())
        {
            // Check for key in string
            if((StringVector[StrNum].find(Key) != string::npos) == true)
            {   
                try
                {   
                    // Get value from key
                    KeyValue = stoi(StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1));
                }
                catch(invalid_argument)
                {
                    throw invalid_argument(string(Key)+"'s value is not of type uint16_t.");
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 65535))
                {
                    throw out_of_range(string(Key)+"'s value is out of range of type uint16_t (0 - 65535).");
                }
                
                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in input string.");
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the boolean of the key. 
If the key is not found, returns -1. */
bool HelperFunctions::GetAtrisKeyValue_bool(string Key, vector<string> StringVector)
{   
    // Return variable
    bool KeyValue;
    string KeyString;

    // Iterate through all strings in the vector.
    for(int StrNum = 0; StrNum < StringVector.size();)
    {   
        if(StringVector[StrNum].length() >= Key.length())
        {
            // Check for key in string
            if((StringVector[StrNum].find(Key) != string::npos) == true)
            {   
                try
                {   
                    // get key value as string
                    KeyString = StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1);

                    // Check for true, True, 1, false, False, 0.
                    if((KeyString == "false") || (KeyString == "False"))
                    {
                        return false;
                    }
                    else if((KeyString == "true") || (KeyString == "True"))
                    {
                        return true;
                    }
                    else
                    {                  
                    // if value is int (0 or 1)
                    KeyValue = stoi(KeyString);
                    }
                }
                catch(...)
                {
                    throw invalid_argument(string(Key)+"'s value is not of type boolean (bool).");
                }
                
                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 1))
                {
                    throw out_of_range(string(Key)+"'s value is out of range of type bool (0 or 1)");
                }
                
                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in input string.");
    return -1;
}