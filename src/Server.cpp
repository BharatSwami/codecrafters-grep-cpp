#include <iostream>
#include <string>
#include <stack>

bool matchdigit(const std::string& input_line){
    int length = input_line.size();
    for(int i = 0;i<length;i++){
        if(std::isdigit(input_line[i])) return true;
    }
    return false;
}
bool matchalphanumeric(const std::string& input_line){
    int length = input_line.size();
    for(int i = 0;i<length;i++){
        if(std::isalnum(input_line[i])) return true;
    }
    return false;
}
bool positiveMatchGroup(const std::string& input_line, const std::string& pattern){
    std::stack<char> s;
    std::stack<std::pair<char,char>> s_pair;
    int idx = 1, patternSize = pattern.length();
    while(idx<patternSize){
        if(idx != patternSize-1 && pattern[idx] == '-'){
            idx++;
            char temp = s.top();
            s.pop();
            s_pair.push({temp,pattern[idx]});
        }
        else{
            s.push(pattern[idx]);
        }
        idx++;
    }
    while (!s.empty())
    {
        char temp = s.top();
        s.pop();
        if(input_line.find(temp) != std::string::npos) return true;
    }
    while (!s_pair.empty())
    {
        std::pair<char,char> temp = s_pair.top();
        s_pair.pop();
        char temp_1 = temp.first, temp_2 = temp.second;
        for(char ch = temp_1; ch<=temp_2;ch++){
            if(input_line.find(ch) != std::string::npos) return true;
        }
        
    }
    
    return false;

}
bool negitiveMatchGroup(const std::string& input_line, const std::string& pattern){
    std::stack<char> s;
    std::stack<std::pair<char,char>> s_pair;
    int idx = 2, patternSize = pattern.length();
    while(idx<patternSize-1){
        if(idx != patternSize-1 && pattern[idx] == '-'){
            idx++;
            char temp = s.top();
            s.pop();
            s_pair.push({temp,pattern[idx]});
        }
        else{
            s.push(pattern[idx]);
        }
        idx++;
    }
    while (!s.empty())
    {
        char temp = s.top();
        s.pop();
        if(input_line.find(temp) != std::string::npos) return false;
    }
    while (!s_pair.empty())
    {
        std::pair<char,char> temp = s_pair.top();
        s_pair.pop();
        char temp_1 = temp.first, temp_2 = temp.second;
        for(char ch = temp_1; ch<=temp_2;ch++){
            if(input_line.find(ch) != std::string::npos) return false;
        }
        
    }
    
    return true;

}
bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if(pattern[0] == '\\' && pattern.length() == 2){
        switch (pattern[1])
        {
        case 'd':
            return matchdigit(input_line);
            break;
        case 'w':
            return matchalphanumeric(input_line);
            break;
        default:
            throw std::runtime_error("Unhandled pattern " + pattern);
            break;
        }
    }
    else if(pattern[0] == '[' && pattern[pattern.length()-1] == ']'){
        if(pattern[1] == '^'){
            return negitiveMatchGroup(input_line,pattern);
        }
        else{
            return positiveMatchGroup(input_line,pattern);
        }

    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char* argv[]) {
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    //Uncomment this block to pass the first stage
    
    std::string input_line;
    std::getline(std::cin, input_line);
    
    try {
        if (match_pattern(input_line, pattern)) {
            return 0;
        } else {
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
