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

    //std::copy(
    //    std::sregex_token_iterator(poly.begin(), poly.end(), term),
    //    std::sregex_token_iterator(), 
    //    std::ostream_iterator<std::string>(std::cout, "\n")
    //    );

  std::string s ("4*x^2+3*x^1+2*x+9-x-10000*x^12-123");
  std::smatch m;
  std::regex e ("([-+]?)(\\d*)\\*?(x|\\d)(\\^([0-9]+))*"); // matches words beginning by "sub"

  while (std::regex_search (s,m,e)) {
   
    positive = -1;  
    coeff = 1;
    power = 0; 

    std::cout << "m[0] = " << m[0] << std::endl;


    std::cout << "m[1] = " << m[1] << std::endl;
    if("-" == m[1]) {
        positive = -1;
    }
    else
    {
        positive = 1; 
    }


    std::cout << "m[2] = " << m[2] << std::endl;
    if("" != m.str(2)) {
        coeff = std::stoi(m.str(2));
    }


    std::cout << "m[5] = " << m.str(5) << std::endl;
    if("" != m.str(5)){
        power = std::stoi(m.str(5)) - 1;
    }

      std::cout << "power" << power << std::endl;
      std::cout << "coeff" << coeff << std::endl;
      std::cout << "positive" << positive << std::endl;

    std::cout << "m[3] = " << m[3] << std::endl;
    if("x" == m.str(3)){
      std::cout << "X exists in this block" << std::endl;
      if(power > -1){
          coeff = coeff * (power + 1);
      }
      components[power] += positive * coeff;

      std::cout << "power" << power << std::endl;
      std::cout << "coeff" << coeff << std::endl;
      std::cout << "positive" << positive << std::endl;

      for(std::map<int, int>::reverse_iterator iterator = components.rbegin(); iterator != components.rend(); iterator++) {
          std::cout << iterator->first << " : " << iterator->second << std::endl; 
      }
    }

    std::cout << std::endl;
    s = m.suffix().str();
  }


  std::string result;
  for(std::map<int, int>::reverse_iterator iterator = components.rbegin(); iterator != components.rend(); iterator++) {

      std::cout << iterator->first << " : " << iterator->second << std::endl; 

     if(iterator->first == 0)
     {
        if(iterator->second > 0){
         result.append("+"); 
        }
        result.append(std::to_string(iterator->second));
     }
     else
     {
        if(iterator->second > 0){
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

  std::cout << "4*x^2+3*x^1+2*x+9-x-10000*x^12-123" << std::endl;
  std::cout << result << std::endl;
    return 0;
}
