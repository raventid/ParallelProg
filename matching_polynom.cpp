#include <iterator>
#include <regex>
#include <string>
#include <iostream>
#include <map>

int main() { 

  std::map<int, int> components;
  int power = 0;
  int coeff = 0;
  int positive = -1;

  std::string s ("-x^2-x^3");
  std::smatch m;
  std::regex e ("([-+]?)(\\d*)\\*?(x|\\d)(\\^([0-9]+))*"); // matches words beginning

  while (std::regex_search (s,m,e)) {
   
    positive = -1;  
    coeff = 1;
    power = 0; 

    if("-" == m[1]) {
        positive = -1;
    }
    else
    {
        positive = 1; 
    }

    if("" != m.str(2)) {
        coeff = std::stoi(m.str(2));
    }
    
    if("" != m.str(5)){
        power = std::stoi(m.str(5)) - 1;
    }

    if("x" == m.str(3)){
      if(power > -1){
          coeff = coeff * (power + 1);
      }
      components[power] += positive * coeff;
    }

    s = m.suffix().str();
  }


  std::string result;
  for(std::map<int, int>::reverse_iterator iterator = components.rbegin(); iterator != components.rend(); iterator++) {

     if(iterator->first == 0)
     {
        if(iterator->second > 0 && components.size() != 1){
         result.append("+"); 
        }
        result.append(std::to_string(iterator->second));
     }
     else
     {
        if(iterator->second > 0 && iterator->first != components.rbegin()->first){
         result.append("+"); 
        }
        result.append(std::to_string(iterator->second));
        result.append("*x");
        if(iterator->first != 1){
          result.append("^");
          result.append(std::to_string(iterator->first));
        }
     }
  } 

  //std::cout << result << std::endl;
  return result;
}
