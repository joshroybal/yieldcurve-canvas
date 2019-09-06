#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include "csv.hpp"

float stringToFloat(const std::string&);
csvRecord binarySearch(const std::string&);

int main()
{
   const int RECSIZ = 256;
   const std::string headers[] = { "date", "1mo", "2mo", "3mo", "6mo", "1yr", 
      "2yr", "3yr", "5yr", "7yr", "10yr", "20yr", "30yr" };
   char buf[RECSIZ];
   
   std::cout << "Content-Type:text/html;charset=utf-8\n\n";
   std::cin.read(buf, RECSIZ);
   std::string target(buf+5);
   csvRecord record = binarySearch(target);
   if ( record.getRecord() != "" ) {
      const std::string headers[] = { "date", "1mo", "2mo", "3mo", "6mo", 
      "1yr", "2yr", "3yr", "5yr", "7yr", "10yr", "20yr", "30yr" };
      std::vector<std::string> fields = record.getFields();
      std::ostringstream osstr;
      for (int i = 1; i < fields.size(); i++) {
         if ( fields[i] == "" ) continue;
         float yield = stringToFloat(fields[i]);
         osstr << headers[i] << ' ' << yield << '\n';
      }
      std:: string execstr = "echo '" + osstr.str() + "' | ";
      execstr += "gnuplot -p -e  'set terminal canvas; plot \"/dev/stdin\" ";
      execstr += "using 2:xticlabels(1) with linespoint title\"" + target + "\" '";
      // std::cout << execstr << std::endl;
      system(execstr.c_str());
   }
   else {
      std::cout << " search target not found\n";
   }
   return 0;
}

float stringToFloat(const std::string& str)
{
   std::istringstream isstr(str);
   float r;
   isstr >> r;
   return r;
}

csvRecord binarySearch(const std::string& target)
{
   const int RECSIZ = 256;
   char buf[RECSIZ];
   std::ifstream ifstr("yields.dat", std::ifstream::binary | std::ifstream::in);
   ifstr.seekg(ifstr.beg, ifstr.end);
   int filesize = ifstr.tellg();
   ifstr.seekg(ifstr.beg);
   int norecs = filesize / RECSIZ;
   int hi = norecs - 1;
   int lo = 0;
   int mid = hi / 2;
   while ( lo <= hi ) {
      ifstr.seekg(mid * RECSIZ);
      ifstr.read(buf, RECSIZ);
      std::string recstr(buf);
      csvRecord record(recstr);
      std::string date_field( record.getField(1) );
      if ( date_field < target ) {
         lo = mid + 1;
      }
      else if ( date_field > target ) {
         hi = mid - 1;
      }
      else {   // target found - return record to main
         ifstr.close();
         return record;
      }
      mid = (lo + hi) / 2;
   }
   ifstr.close();
   return csvRecord("");
}
