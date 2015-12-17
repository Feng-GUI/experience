
#define PLATE_NONE			0  // 0
#define PLATE_1				1  // 2^0, bit 0
#define PLATE_2				2  // 2^1, bit 1
#define PLATE_3				4  // 2^2, bit 2
#define PLATE_4				8  // 2^3, bit 3
#define PLATE_5				16 // 2^4, bit 4

class element   
{
public:
int value;
element* next;
};//class element

class stack 
{
public:
int size;
element* current;

stack()
{
  size=0;
  current=NULL;
}//default constructor

bool push(int,element*);
int pop(); //pop could return element*, but all we need is an int of value.
bool isEmpty();
int getStackSize();
void printStackSize();
void printStackElements(element*);
//void printStackMenu();
};