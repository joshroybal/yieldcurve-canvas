#include <string>
#include <vector>
#include "csv.hpp"

// csv parsing function definitions
int csvRecord::countFields()
{
   int i = 0, j = 0;

   while (i < record.length()) {
      if ( record[i] == '\r' || record[i] == '\n') return j;
      while (record[i] != ',' && i < record.length()) {
         if ( record[i] == '\r' || record[i] == '\n') return j;
         if (record[i] != '\"') {
            i++;
         } else {   // we're escaped right?
            i++;  // step through the quote
            while (record[i] != '\"') 
               i++;
            i++;  // step through the quote
         }
      }
      j++;
      i++;
   }
   return j;
}

std::string csvRecord::getField(int n)
{
   unsigned i = 0, j = 0;
   std::string field;

   while (i < record.length()) {
      while (record[i] != ',' && i < record.length())
         if (record[i] != '\"')
            if (j + 1 == n)
               field += record[i++];
            else
               i++;
         else {
            i++;
            while (record[i] != '\"') {
               if (j + 1 == n)
                  field += record[i++];
               else
                  i++;
            }
            i++;
         }
      if (j + 1 == n) return field;
      i++;
      j++;
   }
   field.clear();
   return field;
}

std::vector<std::string> csvRecord::getFields()
{
   unsigned int i = 0, j = 0;
   std::vector<std::string> fields;

   while (i < record.length()) {
      if ( record[i] == '\r' || record[i] == '\n') return fields;
      fields.push_back("\0");
      while (record[i] != ',' && i < record.length()) {
         if ( record[i] == '\r' || record[i] == '\n') return fields;
         if (record[i] != '\"') {
            fields[j] += record[i++];
         } else {   // we're escaped right?
            i++;  // step through the quote
            while (record[i] != '\"') 
               fields[j] += record[i++];
            i++;  // step through the quote
         }
      }
      j++;
      i++;
   }
   return fields;
}
