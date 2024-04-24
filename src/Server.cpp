#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_map> 




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

bool match(const std::string& input_line, const std::string& pattern,std::unordered_map<int,std::pair<std::string,int>> mpp, int sub_pattern = 0){
    int i = 0;
    bool startAnchor = false;
    bool endAnchor = false;
    if(pattern[0] == '^'){
            startAnchor = true;
        } 
    if(pattern[pattern.size()-1] == '$'){
            //std::string pattern = std::string(pattern.rbegin(),pattern.rend());
            //std::string input_line = std::string(input_line.rbegin(),input_line.rend());
            endAnchor = true;
        }
    while(i<input_line.size()){
        int j = 0;
        if(startAnchor){
            j++;
        } 
          
        if(endAnchor){
            if(input_line.size() < pattern.size()-1) return false;
            else{
                i = input_line.size() - (pattern.size() -1);
            }
        }
        int temp = i;

        while(j<pattern.size() && temp < input_line.size()){
            if(j+1<pattern.size() && pattern[j+1] == '?'){
                if(input_line[temp] == pattern[j]){
                    temp++;
                    j+=1;
                }
                else{
                    j+=1;
                }
            }
            else{
                if(pattern[j] == '\\'){
                    j++;
                    if(j<pattern.size()){
                        if(pattern[j] == 'd'){
                            std::cout<<input_line[temp]<<" " << 'd'<<std::endl;
                            if(!isdigit(input_line[temp])){
                                break;
                            }
                            else temp++;
                        }
                        else if(pattern[j] == 'w'){
                            std::cout<<input_line[temp]<<" " << 'w'<<std::endl;
                            if(!isalnum(input_line[temp])){
                                break;
                            }
                            else temp++;
                        }
                        else if(std::isdigit(pattern[j]) && pattern[j] >='1'){
                            std::string temp2;
                            temp2 += pattern[j];
                            // comment
                            int temp1= std::stoi(temp2);
                            std::string subpattern = mpp[temp1].first;
                            int size = mpp[temp1].second;
                            std::string in_line = input_line.substr(temp,input_line.size()-temp);
                            std::cout << in_line<<std::endl;

                            std::cout << input_line[temp]<<std::endl;
                            if(!match(in_line,subpattern,mpp)) return false;
                            //size = subpattern.size();
                            //auto count = std::count(subpattern.begin(), subpattern.end(),'\\');
                            //size-=count;
                            temp+=size;
                            std::cout << input_line[temp]<<std::endl;
                        
                    }
                    }
                }
                else if(pattern[j] == '[' ){
                        int start=j;
                        std::cout<<"hello " << std::endl;
                        std::cout<<pattern.size() << std::endl;
                        std::cout<<j << std::endl;
                        while(j<pattern.size()){
                            std::cout<<j << std::endl;
                            if(pattern[j] == ']') break;
                            j++;
                        }
                        std::cout<<j << std::endl;
                        if(pattern[pattern.size()-1] == '+'){
                            j++;
                            while(temp<input_line.size() && std::find(pattern.begin(),pattern.end(),input_line[temp])!=pattern.end()){
                                temp++;
                            }
                            if(temp == input_line.size()) return true;
                            else{
                                if(sub_pattern) return false;
                            }
                        }
                        if(pattern[start+1] == '^'){
                            return negitiveMatchGroup(input_line,pattern,start,j+1);
                        }
                        else{
                            return positiveMatchGroup(input_line,pattern,start,j+1);
                        }

                    }
                else if(pattern[j] == '(' ){
                        int start=j;
                        bool ans = false;
                        std::string in_line = input_line.substr(temp,input_line.size()-temp);
                        while(j<pattern.size() ){
                            
                            
                            int size = 0;
                            if(pattern[j] == '|' || pattern[j] == ')'){
                                std::string pat = pattern.substr(start+1,j-start-1);
                                int newSize = pat.size() - std::count(pat.begin(), pat.end(),'\\')-std::count(pat.begin(), pat.end(),'[')-std::count(pat.begin(), pat.end(),']')-std::count(pat.begin(), pat.end(),'+')-std::count(pat.begin(), pat.end(),'*')-std::count(pat.begin(), pat.end(),'?');
                                in_line = in_line.substr(0,newSize);
                                std::cout << pat<<std::endl;
                                std::cout<<in_line<<" " << std::endl;
                                std::cout<<match(in_line,pat,mpp,1)<<std::endl;
                                if(match(in_line,pat,mpp,1)){
                                    ans = true;
                                    size = in_line.size();
                                    while(j<pattern.size() && pattern[j] != ')') j++;
                                }
                                start=j;
                                
                                if(ans){
                                    temp+=size;
                                    int mpp_size = mpp.size();
                                    mpp[mpp_size+1] = {in_line,in_line.size()};
                                    break;
                                }
                                if(pattern[j] == ')') break;
                            }  
                            j++;      
                        }
                        if(!ans) return false;
                    } 
                else if(pattern[j] == '+'){
                    j++;
                    while(j<pattern.size() && temp<input_line.size() && pattern[j] != input_line[temp]){
                        temp++;
                }
            }
                else if(pattern[j] == '.'){
                    temp++;
                }
                else{
                    if(input_line[temp] != pattern[j]){
                        break;
                    }
                    else temp++;
                }
            
            }
            j++;
        }
        if(j == pattern.size()) return true;
        if(startAnchor && j!= pattern.size()) return false;
        if(endAnchor){
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
        std::unordered_map<int,std::pair<std::string,int>> mpp;
        
        return match(input_line,pattern,mpp);
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}


// abcd is abcd, not efg
//([abcd]+) is \1, not [^xyz]+
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
