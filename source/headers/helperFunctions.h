#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <string>
#include <vector>

#include "mapHandler.h"

using namespace std;

class HelperFunctions
{
    public:
        /** 
          *  Removes all of a given ASCII character from the given `string`.
          * 
          *  @param std::string `String`, any string with `String.length()` >= 0.
          *  @param char `Char`, any ASCII character. 
          *
          *  @return `std::string`, returns the input `String` without the given Char.
        */
        string RemoveChar(string String, char Char);

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
        void SplitByDelimiter(string String, vector<string> *SplitStrVecPtr, char Delimiter, int DelimiterAmount);
		void SplitByBraces(string String, vector<string> *SplitStrVecPtr, char StartBrace, char EndBrace);

		// Pass array pointer by reference
        void GetFloatArrayFromStr(string String, float *ArrayPtr, int *ArraySize);
        void GetKeyValue_floatarray(string Key, vector<string> StringVector, float *ArrayPtr, int *ArraySize, string FilePath);

        void GetKeyValue_strvector(string Key, vector<string> StringVector, vector<string> *NestedStringVector);
        void GetKeyValue_floatvector(string Key, vector<string> StringVector, vector<float> *VectorPtr, string FilePath);
 

        // A function for getting a value from a given key string from an .atris file for uint8_t and uint16_t.
        int GetKeyValue_uint8_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath);
        int GetKeyValue_uint16_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath);

        vector<int> GetKeyValue_intvector(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath);

        // A function for getting a value from a given key string from an .atris file for bool.
        bool GetKeyValue_bool(string Key, vector<string> StringVector, vector<string> *StringPtr, string FilePath);
        

};

#endif