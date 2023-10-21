#include <string>
#include <vector>

#include <iostream>

#include "helperFunctions.h"

using namespace std;

/** 
  *  Removes all of a given ASCII character from the given `string`.
  * 
  *  @param std::string `String`, any string with `String.length()` >= 0.
  *  @param char `Char`, any ASCII character. 
  *
  *  @return `std::string`, returns the input `String` without the given Char.
*/
string HelperFunctions::RemoveChar(string String, char Char)
{
    string ReturnString;

    // Go through each charecter of the string
    for(int CharNum = 0; CharNum < String.length();)
    {   
        // Check if the element is a string.
        if(String[CharNum] != Char)
        {   
            // If it is not, push it to the the return string.
            ReturnString.push_back(String[CharNum]);
        }

        CharNum++;
    }
    
    // Return string without the Char(s).
    return ReturnString;
}

/** 
  *  Splits a `string` by a given delimiter of type `char` in ASCII, and takes the substrings created by the split and pushes them into a `std::vector<std::string>`, without the delimiter, using a pointer.
  * 
  *  @param std::string `String`, any string with `String.length()` >= 0.
  *  @param std::vector<std::string> *SplitStrVecPtr, a pointer pointing to the reference of a vector of same type, i.e. `std::vector<std::string> *SplitStrVecPtr = &Vector`.
  *  @param char Delimiter, any ASCII character.
  *  @param int DelimiterAmount, the amount of delimiters to be found before termination, if DelimiterAmount =< 0 it finds all delimiters in the string.
  *
  *  @return void
*/
void HelperFunctions::SplitByDelimiter(string String, vector<string> *SplitStrVecPtr, char Delimiter, int DelimiterAmount)
{
    // Run through the string, and check for the given delimiter.
    for(int Char = 0; Char < String.length();)
    {   
        // Check if the character is the delimiter.
        if(String[Char] == Delimiter)
        {
            // Check if the first character is a delimiter
            if(String[0] != Delimiter)
            {
                // Push the substring until the delimiter, excluding the delimiter, into a vector using a pointer.
                SplitStrVecPtr->push_back(String.substr(0, Char));
            }

            // Create a substring to the right of the delimiter, excluding the delimiter.
            String = String.substr(Char+1, String.size());

            // Found delimiter decrease amount to look for by 1.
            DelimiterAmount -= 1;

            if(DelimiterAmount == 0)
            {
                // Reached last delimiter, push rest of string and return.
                SplitStrVecPtr->push_back(String);
                return;
            }

            /* 
                Run the function with this new substring, since this is recursive function, 
                at some point, the case where no delimiters are found is reached. And the function terminates.
            */
            SplitByDelimiter(String, SplitStrVecPtr, Delimiter, DelimiterAmount);
            
            // Terminate this recursive step of the function.
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

void HelperFunctions::SplitByBraces(string String, vector<string> *SplitStrVecPtr, char StartBrace, char EndBrace)
{
    int BraceStack = 0;
    int StartBracePos;

    // Run through the string, and check for the given delimiter.
    for(int CharNum = 0; CharNum < String.length();)
    {   
        if(String[CharNum] == StartBrace)
        {
            // If bracestack is zero, we know that a new brace has started.
            if(BraceStack == 0)
            {
                // Set the Start position of the brace in the string to the current character index of the string.
                StartBracePos = CharNum;
            }
            // If a startbrace is found add one to the brace stack.
            BraceStack++;
        }

        if((String[CharNum] == EndBrace) && (BraceStack != 0))
        {
            // If a endbrace is found retract one from the brace stack.
            BraceStack--;

            if(BraceStack == 0)
            {
                // If the stack have been cleared, we know that we have gone through all the braces in this "section", and this can be pushed to a vector.
                SplitStrVecPtr->push_back(String.substr(StartBracePos, CharNum-StartBracePos));
            }
        }

        CharNum++;
    }
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint8_t throw exception. */
int HelperFunctions::GetKeyValue_uint8_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath)
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
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type uint8_t, at: "+FilePath);
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 255))
                {
                    throw out_of_range("Key: "+string(Key)+"'s value is out of range of type uint8_t (0 - 255), at: "+FilePath);
                }

                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);

                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string, at: "+FilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key. 
If the key is not found, returns -1, if out of range of uint16_t throw exception. */
int HelperFunctions::GetKeyValue_uint16_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath)
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
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type uint16_t, at: "+FilePath);
                }

                // Throw exception if out of range.
                if((KeyValue < 0) || (KeyValue > 65535))
                {
                    throw out_of_range("Key: "+string(Key)+"'s value is out of range of type uint16_t (0 - 65535), at: "+FilePath);
                }

                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);
                
                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+FilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the boolean of the key. 
If the key is not found, returns -1. */
bool HelperFunctions::GetKeyValue_bool(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath)
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
                    // Erase string in ptr to InfoVector.
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
                    throw invalid_argument("Key: "+string(Key)+"'s value is not of type boolean (bool), at: "+FilePath);
                }
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+FilePath);
    return -1;
}

/* Given a key of type string and a vector containing the string, the function will return the value of the key as a vector. 
If the key is not found, returns {0, 0}, if out of range of uint16_t throws exception. */
vector<int> HelperFunctions::GetKeyValue_vector(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath)
{   
    // Return variable
    vector<int> KeyValue;

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

                SplitByDelimiter(KeyString, KeyValueStrPtr, ',', -1);

                try
                {   
                    for(int Number = 0; Number < KeyValueString.size();)
                    {
                        KeyValue.push_back(stoi(KeyValueString[Number]));
                        Number++;
                    }

                }
                catch(...)
                {
                    throw invalid_argument("Key: "+string(Key)+"'s values are not of type int, at: "+FilePath);
                }
   
                // Erase the string element that the key value pair was a part of, since it only have to be found once, this is to make the searching more efficient.
                StringPtr->erase(StringPtr->begin()+StrNum);

                return KeyValue;
            }
        }
        StrNum++;
    }

    // Because a key was not found throw exception.
    throw invalid_argument("Key: "+string(Key)+", does not exist in the input string vector, at: "+FilePath);
    return {-1};
}


void HelperFunctions::GetFloatArrayFromStr(string String, float *ArrayPtr, int *ArraySize)
{
    HelperFunctions HelperObject;
    vector<string> StringVector;

    String = String.substr(1, String.size()-2);

    // Split the string and pass by reference to the vector.
    HelperObject.SplitByDelimiter(String, &StringVector, ',', -1);

    // Pass the StringVector to the ArraySize by reference.
    *ArraySize = StringVector.size();
    
    for(int Index = 0; Index < StringVector.size();)
    {
        ArrayPtr[Index] = stof(StringVector[Index]);
        Index++;
    }
}

// Gets an array from a key
void HelperFunctions::GetKeyValue_floatarray(string Key, vector<string> StringVector, float *ArrayPtr, int *ArraySize, string FilePath)
{
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

                GetFloatArrayFromStr(KeyString, ArrayPtr, ArraySize);
            }
        }
        StrNum++;
    }
}

void HelperFunctions::GetKeyValue_strvector(string Key, vector<string> StringVector, vector<string> *NestedStringVector)
{
    // Iterate through all strings in the vector.
    for(int StrNum = 0; StrNum < StringVector.size();)
    {   
        if(StringVector[StrNum].length() >= Key.length())
        {
            // Check for key in string
            if((StringVector[StrNum].find(Key) != string::npos) == true)
            {   
                NestedStringVector->push_back(StringVector[StrNum].substr(Key.length()+1, StringVector[StrNum].length()-1));
            }
        }
        StrNum++;
    }
}