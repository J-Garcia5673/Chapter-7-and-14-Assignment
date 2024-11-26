#include <iostream>
#include <stack>
#include <cmath>

using namespace std; 
enum ElementType {NUM, OP};
class Element {
    private:
        string v;
        ElementType type;
    public:
    string getString() {return v;}
    ElementType getType() {return type;}
    Element( string s, ElementType t) {
        v = s; 
        type = t;
    }
    Element() {
        v = "";
        type = NUM; 
    }
};

stack<Element> stk1, stk2;

int getPrecedence(string op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+"|| op == "-") return 1;
    return 0; 

}

void printStack(stack<Element> s){
    if (s.empty()) return;

    Element t = s.top();
    s.pop();

    printStack(s);
    cout << t.getString() << " ";
    s.push(t);
}

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

void postifx(string s) {
    char oper;
    string temp;
    Element t("$", OP);
    stk2.push(t);
    
    // the loops and its cases
    for (int i=0; i<s.length(); i++){
        if (s[i] == ' ') continue;
        else if (s[i] == '('){
            stk2.push(Element("(", OP));
            continue;
        }
        else if (s[i] == ')'){
            while (!stk2.empty()){
                Element t = stk2.top();
                if (t.getString()== "("){
                    stk2.pop();
                    break;
                }
                if (t.getString() == "$"){
                    throw string("Error... Mismatch Pratenthesis");
                }
                Element k = stk2.top();
                stk2.pop();
                stk1.push(k);
            }
        }
        else if (s[i]>='0' && s[i]<='9') {
            string t = "";
            while (s[i]>='0' && s[i]<='9'){
                t += s[i];
                i++;
            }
            i--;
            stk1.push(Element(t, NUM));
        }
        else if (isOperator(s[i])) {
            string oper(1, s[i]);
            int k = getPrecedence(oper);
            while (!stk2.empty()) {
                Element t = stk2.top();
                int j = getPrecedence(t.getString());
                if (k<=j){
                    stk1.push(t);
                    stk2.pop();
                }
                else {
                    Element t(oper, OP);
                    stk2.push(t);
                    break;
                }
            }
        }
        else throw s[i] + string(" Invalid character encountered in expression "); 
    }
    while (!stk2.empty()) {
        Element t = stk2.top();
        if (t.getString()== "$") break;
        stk2.pop();
        stk1.push(t);
    }

}

int evaluate() {
    int size = stk1.size();
    Element s[size];
    int index = size-1;
    while (!stk1.empty()) {
        Element t = stk1.top();
        stk1.pop();
        s[index] = t;
        index--;
    }
    for (int i=0; i<size; i++) {
        if (s[i].getType()== OP) {
            if (i<2) throw string( "Invalid Expression ");
            if (s[i-1].getType() != NUM) throw string("Invalid Expression");
            if (s[i-2].getType() != NUM) throw string("Invalid Expression");
            int a= stoi(s[i-1].getString());
            int b= stoi(s[i-2].getString());
            int r;
            string op = s[i].getString();
            if (op=="+") r = a+b;
            else if(op=="-") r = b-a;
            else if(op=="*") r = b*a;
            else if(op=="/") {
                if (a == 0) throw string("Division by zero \n");
                r = b/a;
            }
            else if (op=="^") r = pow(a,b);
            s[i-2] = Element(to_string(r), NUM);
            for (int j=i+1; j<size; j++) {
                s[j-2] = s[j];
            }
            size -=2;
            i -= 2;
        }
    } 
    return stoi(s[0].getString());
}

int main(){
    string s;
    cout << "Enter your expression: ";
    getline(cin, s);

    int v = 0;
    try {
        postifx(s);
        v = evaluate();
        cout << "v= " << v << endl;
    }
    catch (string s) {
        cout << s << endl;
    }
    return 0;
}

