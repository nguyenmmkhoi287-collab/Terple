#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
using namespace std;

const string VERSION = "0.3";
string CURRENT_USER = "guest";
bool LEARN_MODE = false;

// ===== COLOR =====
void color(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

// ===== FILE EXISTS =====
bool existsFile(const string &p){
    struct stat buffer;
    return (stat(p.c_str(), &buffer)==0);
}

// ===== BANNER =====
void banner(){
    color(11);
    cout<<"Terple v"<<VERSION<<" (Terminal + Simple)\n";
    cout<<"PC Link Mode Enabled\n";
    cout<<"-------------------------------------\n";
    color(7);
}

// ===== INFO =====
void info(){
    color(10);
    cout<<"==== Terple Info ====\n";
    cout<<"User: "<<CURRENT_USER<<"\n";
    cout<<"Shell: Terple CLI\n";
    cout<<"Mode: PC Link\n";
    cout<<"Version: "<<VERSION<<"\n";
    cout<<"System: Windows\n";
    cout<<"=====================\n";
    color(7);
}

// ===== LEARN HELPER =====
void learnMsg(string cmd,string msg){
    if(LEARN_MODE){
        color(14);
        cout<<"[learn] "<<cmd<<" "<<msg<<"\n";
        color(7);
    }
}

// ====== SIMPLE CALCULATOR (multi expression) ======
double eval(string s){
    stringstream ss(s);
    double result; ss>>result;
    char op;
    double x;
    while(ss>>op>>x){
        if(op=='+') result+=x;
        else if(op=='-') result-=x;
        else if(op=='*') result*=x;
        else if(op=='/') result = (x!=0)? result/x:0;
    }
    return result;
}

// ===== HELP =====
void help(){
    color(14);
    cout<<"Terple Commands:\n";
    cout<<" run help\n";
    cout<<" run help <command>\n";
    cout<<" run print <text>\n";
    cout<<" run calculate <expr>\n";
    cout<<" run info\n";
    cout<<" run notepad <file>\n";
    cout<<" run readfile <file>\n";
    cout<<" run script <file.tp>\n";
    cout<<" run learn on/off\n";
    cout<<" run makefolder <name>\n";
    cout<<" run enter <folder>\n";
    cout<<" run showfiles\n";
    cout<<" run whereami\n";
    cout<<" run open <path>\n";
    cout<<" run pc_ls <folder>\n";
    cout<<" run pc_read <file>\n";
    cout<<" run pc_exists <path>\n";
    cout<<" run pc_exec <exe>\n";
    cout<<" run exit\n";
    color(7);
}

// ======= HELP DETAILS =======
void miniHelp(string c){
    if(c=="print") cout<<"print -> show text on screen\n";
    else if(c=="calculate") cout<<"calculate -> do math like 2 + 3 * 5\n";
    else if(c=="info") cout<<"info -> system info\n";
    else if(c=="notepad") cout<<"notepad <file> -> type text and save\n";
    else if(c=="script") cout<<"script <file.tp> -> execute commands in file\n";
    else if(c=="pc_ls") cout<<"pc_ls <folder> -> list pc files\n";
    else cout<<"No help data for this command\n";
}

// ======== NOTEPAD =========
void notepad(string file){
    learnMsg("notepad","lets you write");
    cout<<"Type text. Type SAVE on a line to finish.\n";
    ofstream f(file);
    string line;
    while(true){
        getline(cin,line);
        if(line=="SAVE") break;
        f<<line<<"\n";
    }
    cout<<"Saved.\n";
}

// ====== SCRIPT RUNNER =====
void script(string f){
    if(!existsFile(f)){
        cout<<"Script not found\n";
        return;
    }
    ifstream file(f);
    string x;
    while(getline(file,x)){
        system(x.c_str());
    }
}

// ======= PC CONTROLS =======
void openFile(string p){
    if(!existsFile(p)){
        cout<<"Failed to open file\n"; return;
    }
    ShellExecuteA(NULL,"open",p.c_str(),NULL,NULL,SW_SHOW);
    cout<<"Opened.\n";
}
void pc_ls(string p){
    string cmd="dir \""+p+"\"";
    system(cmd.c_str());
}
void pc_read(string f){
    if(!existsFile(f)){
        cout<<"Not found\n";return;
    }
    ifstream t(f);
    string l;
    while(getline(t,l)) cout<<l<<"\n";
}
void pc_exec(string e){
    if(!existsFile(e)){ cout<<"Exe not found\n"; return;}
    ShellExecuteA(NULL,"open",e.c_str(),NULL,NULL,SW_SHOW);
}

// ======= VIRTUAL FS =======
string currentFolder="/home";
map<string, vector<string>> folders;

void makefolder(string name){
    folders[currentFolder].push_back(name);
    cout<<"Folder created\n";
}
void enter(string name){
    currentFolder+="/"+name;
    cout<<"Entered "<<currentFolder<<"\n";
}
void showfiles(){
    cout<<"Folders here:\n";
    for(auto &x: folders[currentFolder]) cout<<x<<"\n";
}
void whereami(){
    cout<<"You are in: "<<currentFolder<<"\n";
}

// ================= MAIN =================
int main(){
    system("title Terple Shell");
    banner();

    string cmd;
    while(true){
        color(11);
        cout<<CURRENT_USER<<"@terple$ ";
        color(7);

        getline(cin,cmd);
        if(cmd=="") continue;

        if(cmd.rfind("run ",0)!=0){
            cout<<"Commands must start with 'run'\n";
            continue;
        }

        stringstream ss(cmd.substr(4));
        string c;
        ss>>c;

        if(c=="help"){
            string more; ss>>more;
            if(more=="") help();
            else miniHelp(more);
        }
        else if(c=="print"){
            string x; getline(ss,x);
            if(x.size()&&x[0]==' ') x.erase(0,1);
            learnMsg("print","shows text on screen");
            cout<<x<<"\n";
        }
        else if(c=="calculate"){
            string x; getline(ss,x);
            if(x.size()&&x[0]==' ') x.erase(0,1);
            learnMsg("calculate","does math");
            cout<<eval(x)<<"\n";
        }
        else if(c=="info") info();
        else if(c=="learn"){
            string s; ss>>s;
            LEARN_MODE = (s=="on");
            cout<<"Learn mode "<<(LEARN_MODE?"enabled\n":"disabled\n");
        }
        else if(c=="notepad"){
            string f; ss>>f;
            notepad(f);
        }
        else if(c=="readfile"){
            string f; ss>>f;
            pc_read(f);
        }
        else if(c=="script"){
            string f; ss>>f;
            script(f);
        }
        else if(c=="makefolder"){
            string n; ss>>n;
            makefolder(n);
        }
        else if(c=="enter"){
            string n; ss>>n;
            enter(n);
        }
        else if(c=="showfiles") showfiles();
        else if(c=="whereami") whereami();
        else if(c=="open"){
            string p; getline(ss,p);
            if(p.size()&&p[0]==' ') p.erase(0,1);
            openFile(p);
        }
        else if(c=="pc_ls"){
            string p; getline(ss,p);
            if(p.size()&&p[0]==' ') p.erase(0,1);
            pc_ls(p);
        }
        else if(c=="pc_read"){
            string p; getline(ss,p);
            if(p.size()&&p[0]==' ') p.erase(0,1);
            pc_read(p);
        }
        else if(c=="pc_exists"){
            string p; getline(ss,p);
            if(p.size()&&p[0]==' ') p.erase(0,1);
            cout<<(existsFile(p)?"Exists\n":"Not Found\n");
        }
        else if(c=="pc_exec"){
            string p; getline(ss,p);
            if(p.size()&&p[0]==' ') p.erase(0,1);
            pc_exec(p);
        }
        else if(c=="exit"){
            cout<<"Goodbye!\n";
            break;
        }
        else cout<<"Unknown command\n";
    }
}
