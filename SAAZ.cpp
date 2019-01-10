#include <iostream>
#include <assert.h>
#include <typeinfo>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <conio.h>

using namespace std;

int anda(int operand, int mainMemory[]);
int ora(int operand, int mainMemory[]);
int load(int operand, int mainMemory[]);
int load_ind(int operand, int mainMemory[]);
int ldi(int operand, int mainMemory[]);
int store(int operand, int mainMemory[]);
int store_ind(int operand, int mainMemory[]);
int add(int operand, int mainMemory[]);
int subtract(int operand, int mainMemory[]);
int divide(int operand, int mainMemory[]);
int multiply(int operand, int mainMemory[]);
int movatob();
int mavbtoa();
int miscell(int operand,int *x);
string convert_hexcode(string a);
int getaddress(string s,string f);
int convert (const string& str);
int convert1 (const string& str);

int prompt();
void dump();
void out();

int mainMemory[256];

int accumulator,b,ret;
bool zero,carry,choice;

int main()
 {
	cout<< "Welcome to SAAZ!!\n";
	prompt();
	return 0;
}
int prompt()
{
    string file_name,a,inp_fh,hexcode,inp_f;
    vector<pair<string,string> > data;
    Again:
    cout<<"Enter the name of the file : ";
    cin>>file_name;
    file_name+=".z";
    const char *c=file_name.c_str();
    ifstream he;
    he.open(c,ios::in);
    if(he.fail())
	{
	cout << "No File Found!"<<endl;
	goto Again;
	}
    he>>a;
    int pos=0,start;
    cout<<"Do you want Step By Step Execution(1) or Run All at once(0) : ";
    cin>>choice;
    cout<<endl<<"Machine Code in Hexadecimal :"<<endl;
    while(a!=".CODE"  && a!=".code")
    {
        he >> a;
        if(a[a.size()-1]==',')
        {
            he >> inp_fh;
            cout << "[" <<hex<<setfill('0')<<setw(2)<< pos << "]: ";
            cout<<inp_fh<<endl;
            int m=convert1(inp_fh);
            mainMemory[pos]=m;
            a=a.substr(0,a.size()-1);
            ostringstream stream;
            stream <<setfill('0')<<setw(2)<<hex<<pos;
            inp_fh=stream.str();
            pos++;
            if(a!="HEX")
            data.push_back(make_pair(a,inp_fh));
        }
    }
    start=pos;
    while(!he.eof())
    {
        he >> a;
        if(a[a.size()-1]==',')
        {
            a=a.substr(0,a.size()-1);
            ostringstream stream;
            stream <<setfill('0')<<setw(2)<<hex<<pos;
            inp_fh=stream.str();
            data.push_back(make_pair(a,inp_fh));
            continue;
        }
        hexcode=convert_hexcode(a);
        if(a=="LDA" || a=="STA" || a=="JMP" ||  a=="CAL" )
        {
            he >> inp_f;
            inp_fh="";
            for(int i=0;i<data.size();i++)
                if(data[i].first==inp_f)
                    inp_fh=data[i].second;
            hexcode+=inp_fh;
            if(hexcode.size()==1)
                {
                    int x1=getaddress(inp_f,file_name);
                    ostringstream stream;
                    stream <<setfill('0')<<setw(2)<<hex<<x1;
                    inp_fh=stream.str();
                    hexcode+=inp_fh;
                }
        }
        else if(a=="LDI")
        {
            he >> inp_fh;
            hexcode+=inp_fh;
        }
		cout << "[" << hex<<setfill('0')<<setw(2)<<pos << "]: ";
		cout<<hexcode<<endl;
		mainMemory[pos] = convert(hexcode);
		pos++;
    }
    cout<<endl;
    he.close();
	int x=start;
	int operand;
	int operationNumeric;
	while(x<pos)
    {
	operationNumeric = mainMemory[x]/1000;
	operand=mainMemory[x]%1000;
	switch ( operationNumeric )
        {
        case 0:
            miscell(operand,&x);
            break;

		case 1:
		    cout<<"Load Operation :"<<endl;
		load(operand, mainMemory);
            break;

		case 2:
		    cout<<"Store Operation :"<<endl;
		store(operand, mainMemory);
		break;

		case 3:
		    cout<<"Load Immediate Operation :"<<endl;
        ldi(operand,mainMemory);
        break;

        case 4:
            cout<<"Move A to B Operation :"<<endl;
        movatob();
        break;

        case 5:
            cout<<"Move B to A Operation :"<<endl;
        mavbtoa();
        break;

		case 6:
		    cout<<"Addition Operation :"<<endl;
		add(operand, mainMemory);
		break;

		case 7:
		    cout<<"Subtraction Operation :"<<endl;
		subtract(operand, mainMemory);
		break;

		case 9:
		    cout<<"Division Operation :"<<endl;
		divide(operand, mainMemory);
		break;

		case 8:
		    cout<<"Multiplication Operation :"<<endl;
		multiply(operand, mainMemory);
		break;

		case 10:
		if(operand==0)
        {
            cout<<"And Operation :"<<endl;
            anda(operand, mainMemory);
        }
        else if(operand==1)
        {
            cout<<"Or Operation :"<<endl;
            ora(operand, mainMemory);
        }
        break;

        case 11:
        if(operand==0)
        {
            cout<<"Load Indirect Operation :"<<endl;
            load_ind(operand, mainMemory);
        }
        else if(operand==1)
        {
            cout<<"Store Indirect Operation :"<<endl;
            store_ind(operand, mainMemory);
        }
        break;

        case 12:
            cout<<"Jump to "<<hex<<operand<<" Operation :"<<endl;
            x=operand-1;
            break;

        case 13:
            cout<<"Call to "<<hex<<setfill('0')<<setw(2)<<operand<<" Operation :"<<endl;
            ret=x+1;
            x=operand-1;
            break;

        case 14:
            cout<<"Return Operation :"<<endl;
            x=ret-1;
            break;

		case 15:
		cout << "Quitting!";
		return 1;
		break;

		default:
		cout << "Unknown command, error! Quiting!";
		return 1;
		break;
        }
        out();
        if(choice==1)getch();
        x++;
	}
	return 0;
}

void out()
{
    cout<<endl;
    cout<<"|---------------------------------------------------------------|"<<endl;
    cout<<"|    "<<"Accumulator     |     B     |      Carry     |     Zero    |"<<endl;
    cout<<"|---------------------------------------------------------------|"<<endl;
    cout<<"|          "<<hex<<setw(2)<<accumulator<<"        |     "<<setw(2)<<b<<"    |       "<<carry<<"        |     "<<zero<<"       |"<<endl;
    cout<<"|---------------------------------------------------------------|"<<endl<<endl<<endl;
}

int miscell(int operand,int *x)
{
        if(operand==0)
            {
                cout<<"Clear Accumulator :"<<endl;
                accumulator=0;
            }
        else if(operand==1)
            {
                cout<<"Clear Carry Flag :"<<endl;
                carry=0;
            }
        else if(operand==2)
            {
                cout<<"Clear Zero Flag :"<<endl;
                zero=0;
            }
        else if(operand==3)
            {
                cout<<"Compliment Accumulator :"<<endl;
                accumulator=255-accumulator;
            }
        else if(operand==4)
            {
                cout<<"Compliment Carry Flag :"<<endl;
                carry=(!carry);
            }
        else if(operand==5)
            {
                cout<<"Compliment Zero Flag :"<<endl;
                zero=(!zero);
            }
        else if(operand==6)
            {
                cout<<"Increement Accumulator :"<<endl;
                accumulator++;
                if(accumulator==256)
                {
                    accumulator=0;
                    carry=1;
                }
                else
                    carry=0;
                if(accumulator==0)
                    zero=1;
                else
                    zero=0;
            }
        else if(operand==7)
            {
                cout<<"Decreement Accumulator :"<<endl;
                accumulator--;
                if(accumulator<0)
                {
                    accumulator=255+accumulator;
                    carry=1;
                }
                else
                {
                    carry=0;
                }
                if(accumulator==0)
                    zero=1;
                else
                    zero=0;
            }
        else if(operand==8)
            {
                cout<<"Skip if Carry Flag is Set :"<<endl;
                if(carry==1)
                *x+=1;
            }
        else if(operand==9)
            {
                cout<<"Skip if Carry Flag is Clear :"<<endl;
                if(carry==0)
                *x+=1;
            }
        else if(operand==10)
            {
                cout<<"Skip if Zero Flag is Set :"<<endl;
                if(zero==1)
                *x+=1;
            }
        else if(operand==11)
            {
                cout<<"Skip if Zero Flag is Clear :"<<endl;
                if(zero==0)
                *x+=1;
            }
        else if(operand==12)
        {
            bool y=carry;
            cout<<"Circular Right Rotate Accumulator :"<<endl;
            carry=accumulator%2;
            accumulator=accumulator/2;
            if(y)
                accumulator+=128;
            if(accumulator==0)
                zero=1;
            else
                zero=0;
        }
        else if(operand==13)
        {
            bool y=carry;
            cout<<"Circular Left Rotate Accumulator :"<<endl;
            if(accumulator>127)
                carry=1;
            else
                carry=0;
            accumulator*=2;
            if(y)
                accumulator++;
            if(accumulator>255)
                accumulator=accumulator%256;
            if(accumulator==0)
                zero=1;
            else
                zero=0;
        }
        return 0;
}

int load(int operand, int mainMemory[])
{
	int address = operand;
	accumulator = mainMemory[address];
	return 0;
}

int load_ind(int operand, int mainMemory[])
{
	int address = operand;
	accumulator = mainMemory[b];
	return 0;
}

int store(int operand, int mainMemory[])
{
	int address = operand;
	mainMemory[address] = accumulator;
	cout<<endl<<"Change in Memory"<<endl;
	cout<<"["<< hex<<setfill('0')<<setw(2)<<address<<"]"<<":  "<<hex<<mainMemory[address]<<endl<<endl;
	return 0;
}

int store_ind(int operand, int mainMemory[])
{
	int address = operand;
	mainMemory[b] = accumulator;
	cout<<endl<<"Change in Memory"<<endl;
	cout<<"["<< hex<<setfill('0')<<setw(2)<<b<<"]"<<":  "<<hex<<mainMemory[b]<<endl<<endl;
	return 0;
}

int ldi(int operand, int mainMemory[])
{
	int data = operand;
	accumulator = data;
	return 0;
}
int movatob()
{
    b=accumulator;
    return 1;
}

int mavbtoa()
{
    accumulator=b;
    return 1;
}

int add(int operand, int mainMemory[])
{
	int address = operand;
	accumulator += b;
	if(accumulator>255)
    {
        accumulator=accumulator%256;
        carry=1;
    }
    else
        carry=0;
    if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}

int subtract(int operand, int mainMemory[])
{
	int address = operand;
	int value = accumulator - b;
	accumulator = value;
	if(accumulator<0)
        {
            carry=1;
            accumulator=255+accumulator;
        }
    else
        carry=0;
	if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}

int anda(int operand, int mainMemory[])
{
	int address = operand;
	int a=accumulator,ba=b,c=0,d=1;
while(a&&b)
{
    if(a%2==1  && ba%2==1)
        c+=d;
    d*=2;
    a/=2;
    ba/=2;
}
    accumulator=c;
	if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}
int ora(int operand, int mainMemory[])
{
	int address = operand;
	int a=accumulator,ba=b,c=0,d=1;
while(a&&b)
{
    if(a%2==1  || ba%2==1)
        c+=d;
    d*=2;
    a/=2;
    ba/=2;
}
    accumulator=c;
	if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}
int divide(int operand, int mainMemory[])
{
	int address = operand;
	int value = accumulator / b;
	accumulator = value;
	b=accumulator % b;
	if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}

int multiply(int operand, int mainMemory[])
{
	int address = operand;
	int value = accumulator * b;
	accumulator = value;
	if(accumulator>255)
    {
        b=accumulator/256;
        accumulator=accumulator%256;
        carry=1;
    }
    else
    {
        carry=0;
    }
    if(accumulator==0)
        zero=1;
    else
        zero=0;
	return 0;
}

string convert_hexcode(string a)
{
    for(int i=0;i<a.size();i++)
       a[i] = toupper(a[i]);
    if(a=="LDA")
        return "1";
    else if(a=="STA")
        return "2";
    else if(a=="LDI")
        return "3";
    else if(a=="MAB")
        return "400";
    else if(a=="MBA")
        return "500";
    else if(a=="ADD")
        return "600";
    else if(a=="SUB")
        return "700";
    else if(a=="MUL")
        return "800";
    else if(a=="DIV")
        return "900";
    else if(a=="AND")
        return "A00";
    else if(a=="ORA")
        return "A01";
    else if(a=="LAI")
        return "B00";
    else if(a=="SAI")
        return "B01";
    else if(a=="JMP")
        return "C";
    else if(a=="CAL")
        return "D";
    else if(a=="RET")
        return "E00";
    else if(a=="HLT")
        return "fff";
    else if(a=="CLA")
        return "000";
    else if(a=="CLC")
        return "001";
    else if(a=="CLZ")
        return "002";
    else if(a=="CMA")
        return "003";
    else if(a=="CMC")
        return "004";
    else if(a=="CMZ")
        return "005";
    else if(a=="INR")
        return "006";
    else if(a=="DCR")
        return "007";
    else if(a=="SCS")
        return "008";
    else if(a=="SCC")
        return "009";
    else if(a=="SZS")
        return "00A";
    else if(a=="SZC")
        return "00B";
    else if(a=="CIR")
        return "00C";
    else if(a=="CIL")
        return "00D";
    else
        return NULL;
}

int getaddress(string s,string f)
{
    const char *c=f.c_str();
    string a;
    ifstream file;
    file.open(c,ios::in);
    file>>a;
    int x=0;
    while(a!=".CODE")
    {
        file>>a;
        if(a[a.size()-1]==',')
            file>>a;
        x++;
    }
    x--;
    while(!file.eof())
    {
        file>>a;
        if(a=="LDA" || a=="STA" || a=="LDI" || a=="JMP" ||  a=="CAL" )
            file>>a;
        if(a[a.size()-1]==',')
            if(a.substr(0,a.size()-1)==s)
                break;
            else
                continue;
        x++;
    }
    file.close();
    return x;
}

int convert (const string& str)
{
    string s1,s2;
    s1=str.substr(0,1);
    s2=str.substr(1,2);
    istringstream converter1(s1),converter2(s2);
int value1,value2,value;
converter1 >> hex >> value1;
converter2 >> hex >> value2;
value=value1*1000+value2;
return value;
}

int convert1 (const string& str)
{
istringstream converter1(str);
int value;
converter1 >> hex >> value;
return value;
}
