#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

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
bool positiveMatchGroup(const std::string& input_line, const std::string& pattern,int start,int end){
    std::stack<char> s;
    std::stack<std::pair<char,char>> s_pair;
    if(end>=pattern.size()) end = pattern.size();
    int idx = start, patternSize = end;
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
bool negitiveMatchGroup(const std::string& input_line, const std::string& pattern,int start,int end){
    std::stack<char> s;
    std::stack<std::pair<char,char>> s_pair;
    if(end>=pattern.length()) end=pattern.length();
    int idx = start+1, patternSize = end;
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

bool match(const std::string& input_line, const std::string& pattern){
    int i = 0;
    bool startWildcard = false;
    bool endWildcard = false;
    if(pattern[0] == '^'){
            startWildcard = true;
        } 
    if(pattern[pattern.size()-1] == '$'){
            //std::string pattern = std::string(pattern.rbegin(),pattern.rend());
            //std::string input_line = std::string(input_line.rbegin(),input_line.rend());
            endWildcard = true;
        }
    while(i<input_line.size()){
        int j = 0;
        if(startWildcard){
            j++;
        } 
          
        if(endWildcard){
            if(input_line.size() < pattern.size()-1) return false;
            else{
                i = input_line.size() - (pattern.size() -1);
            }
        }
        int temp = i;

        while(j<pattern.size() && temp < input_line.size()){
            if(pattern[j] == '\\'){
                j++;
                if(j<pattern.size()){
                    if(pattern[j] == 'd'){
                        if(!isdigit(input_line[temp])){
                            break;
                        }
                        else temp++;
                    }
                    else if(pattern[j] == 'w'){
                        if(!isalnum(input_line[temp])){
                            break;
                        }
                        else temp++;
                    }
                    else if(pattern[j] == '[' ){
                        int start=j;
                        while(j<pattern.size()){
                            if(pattern[j] != ']') j++;
                        }
                        if(pattern[j] == '^'){
                            return negitiveMatchGroup(input_line,pattern,start,j+1);
                        }
                        else{
                            return positiveMatchGroup(input_line,pattern,start,j+1);
                        }

                    }
                }
                else{
                    break;
                }
            }
            else{
                if(input_line[temp] != pattern[j]){
                    break;
                }
                else temp++;
            }
            j++;
        }
        if(j == pattern.size()) return true;
        if(startWildcard && j!= pattern.size()) return false;
        if(endWildcard){
            if(j == pattern.size()-1) return true;
            return false;
        }
        i++;
    }
    
    return false;
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
            return negitiveMatchGroup(input_line,pattern,0,pattern.size());
        }
        else{
            return positiveMatchGroup(input_line,pattern,0,pattern.size());
        }

    }
    else if(pattern.length() > 1){
        return match(input_line,pattern);
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
