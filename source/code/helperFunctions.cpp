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
void HelperFunctions::SplitByDelimiter(string String, vector<string> *SplitStrVecPtr, char Delimiter)
{
    // Run through all the content of the file, and check for delimiters.
    for(int Char = 0; Char < String.length();)
    {   
        // Check if the character is the delimiter.
        if(String[Char] == Delimiter)
        {
            if(String[0] != Delimiter)
            {
                // Push the line until the delimiter into a vector.
                SplitStrVecPtr->push_back(String.substr(0, Char));
            }

            // Set the new string
            String = String.substr(Char+1, String.size());

            // Run the function recursivly until the divide step have been completed.
            SplitByDelimiter(String, SplitStrVecPtr, Delimiter);
            
            // break the for loop.
            return;
        }
        else
        {
            Char++;
        }
    }
    
    // Will only reach when base case
    SplitStrVecPtr->push_back(String.substr(0, String.length()));
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint8_t throw exception. */
int HelperFunctions::GetAtrisKeyValue_uint8_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath)
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
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type uint8_t, at: "+AtrisFilePath);
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 255))
                {
                    throw out_of_range("Key: "+string(Key)+"'s value is out of range of type uint8_t (0 - 255), at: "+AtrisFilePath);
                }

                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);

                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string, at: "+AtrisFilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint16_t throw exception. */
int HelperFunctions::GetAtrisKeyValue_uint16_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath)
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
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type uint16_t, at: "+AtrisFilePath);
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 65535))
                {
                    throw out_of_range("Key: "+string(Key)+"'s value is out of range of type uint16_t (0 - 65535), at: "+AtrisFilePath);
                }

                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);
                
                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+AtrisFilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the boolean of the key. 
If the key is not found, returns -1. */
bool HelperFunctions::GetAtrisKeyValue_bool(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath)
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
                // get key value as string
                KeyString = StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1);

                // Check for true, True, 1, false, False, 0.
                if((KeyString == "false") || (KeyString == "False"))
                {
                    // Erase string in ptr to AtrisInfoVector.
                    StringPtr->erase(StringPtr->begin()+StrNum);
                    return false;
                }

                if((KeyString == "true") || (KeyString == "True"))
                {
                    StringPtr->erase(StringPtr->begin()+StrNum);
                    return true;
                }
                
                if(KeyString == "1")
                {
                    StringPtr->erase(StringPtr->begin()+StrNum);
                    return true;
                }

                if(KeyString == "0")
                {
                    StringPtr->erase(StringPtr->begin()+StrNum);
                    return false;
                }
                else
                {
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type boolean (bool), at: "+AtrisFilePath);
                }
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+AtrisFilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key as a vector. 
If the key is not found, returns {0, 0}, if out of range of uint16_t throws exception. */
vector<short> HelperFunctions::GetAtrisKeyValue_vector(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath)
{   
    // Return variable
    vector<short> KeyValue;

    vector<string> KeyValueString;
    vector<string> *KeyValueStrPtr = &KeyValueString;

    string KeyString;

    // Iterate through all strings in the vector.
    for(int StrNum = 0; StrNum < StringVector.size();)
    {   
        if(StringVector[StrNum].length() >= Key.length())
        {
            // Check for key in string
            if((StringVector[StrNum].find(Key) != string::npos) == true)
            {   
                KeyString = StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1);

                SplitByDelimiter(KeyString, KeyValueStrPtr, ',');

                cout << KeyValueString[0] << endl;
                cout << KeyValueString[1] << endl;

                if(KeyValueString.size() != 2)
                {
                    throw invalid_argument("Key: "+string(Key)+"'s values must be contained in a vector of size 2, at: "+AtrisFilePath);
                }
                
                try
                {   
                    // Get values from key
                    KeyValue.push_back(stoi(KeyValueString[0]));
                    KeyValue.push_back(stoi(KeyValueString[1]));

                }
                catch(...)
                {
                    throw invalid_argument("Key: "+string(Key)+"'s values are not of type uint16_t, at: "+AtrisFilePath);
                }
   
                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);

                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+AtrisFilePath);
    return {0, 0};
}