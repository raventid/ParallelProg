enum ALPHABET {X, PLUS, MINUS, DIGIT, DEG};

ALPHABET get_alphabet(char c){
    if (c >= '0' && c <= '9'){
        return DIGIT;
    }
    if (c == 'x')
        return X;
    if (c == '+')
        return PLUS;
    if (c == '-')
        return MINUS;
 
    //^
    return DEG;
}
 
                    //DIGIT //X     //+     //-     //^
STATES tf[9][5] = { {S_N,   S_X1,   S_P,    S_M,    S_E },  //0 S_S
                    {S_N,   S_X1,   S_E,    S_E,    S_E },  //1 S_M
                    {S_N,   S_X1,   S_E,    S_E,    S_E },  //2 S_P
                    {S_N,   S_X,    S_E,    S_E,    S_E },  //3 S_N
                    {S_E,   S_E,    S_E,    S_E,    S_D },  //4 S_X1
                    {S_E,   S_E,    S_E,    S_E,    S_D },  //5 S_X
                    {S_DEG, S_E,    S_E,    S_E,    S_E },  //6 S_D
                    {S_DEG, S_E,    S_E,    S_E,    S_E },  //7 S_DEG
                    {S_E,   S_E,    S_E,    S_E,    S_E }}; //8 S_E

member parse(char** pos){
    member m;
    int sign = 1, n = 1;
    STATES state = S_S;
    stringstream ss_n, ss_deg;
    unsigned int deg = 0;
    char*& cur = *pos;
    while(true){
        if (*cur == '\0'){
            break;
        }else{
            if (*cur == '*')
                cur++;
            state = tf[state][get_alphabet(*cur)];
            switch (state) {
                case S_N:
                    ss_n << *cur;
                    break;
                case S_DEG:
                    ss_deg << *cur;
                    break;
                case S_X1:
                case S_X:
                    deg = 1;
                    break;
                case S_M:
                    sign *= -1;
                    break;
                default:
                    break;
            }
            if (state == S_E)
                break;
            else
                cur++;
        }
    }
    if (ss_n.str().size() > 0){
        ss_n >> n;
    }
    if (ss_deg.str().size() > 0){
        ss_deg >> deg;
    }
    n *= sign;
    m.n  = n;
    m.deg = deg;
    return m;
}
 
 
 
 
string derivative(string polynomial){
    char* buf = (char*) calloc(polynomial.size() + 1, sizeof(char));
    buf = strncpy(buf, polynomial.c_str(), polynomial.size());
    char* ptr = buf;
    set<unsigned int> degrees;
    unordered_map<unsigned int, member> input_polynom;
    unordered_map<unsigned int, member> output_polynom;
    while (*ptr != '\0'){
        member m = parse(&ptr);
        if (degrees.count(m.deg) == 0){
            input_polynom[m.deg] = m;
            degrees.insert(m.deg);
        }else{
            input_polynom[m.deg].n += m.n;
        }
    }
    free(buf);
    set<unsigned int> output_degrees;
    for (auto i = degrees.rbegin(); i != degrees.rend(); i++){
        if (*i > 0){
            output_degrees.insert((*i) - 1);
            output_polynom[(*i)-1].n = input_polynom[*i].n * ((*i));
            output_polynom[(*i)-1].deg = (*i)-1;
        }
    }
    if (output_degrees.size() == 0){
        return (string)"0";
    }else{
        stringstream answer;
        for (auto i = output_degrees.rbegin(); i != output_degrees.rend(); i++){
            int n = output_polynom[*i].n;
            unsigned int deg = output_polynom[*i].deg;
            if (n == 0){
                continue;
            }
            if (n < 0){
                answer << "-";
            }else{
                answer << "+";
            }
            n = abs(n);
            switch (deg) {
                case 0:
                    answer << n;
                    break;
                case 1:
                    if (n > 1)
                        answer << n <<"*x";
                    else
                        answer << "x";
                    break;
                default:
                    if (n > 1)
                        answer << n << "*x^" << deg;
                    else
                        answer << "x^" << deg;
                    break;
            }
        }
        string ret = answer.str();
        if (ret.size() == 0){
            return (string)"0";
        }
        if (ret.c_str()[0] == '+')
            return (string)(ret.c_str()+1);
        else{
            return ret;
        }
    }
    return (string)"";
}

int main(int argc, char **argv){
  derivative("2*x^100+100*x^2");  
}
