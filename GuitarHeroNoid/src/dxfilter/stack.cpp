#include <stdio.h>
#include <conio.h>
//#include <iostream.h>
#include <stdlib.h>

#include "stack.h"

bool stack::push(int ele,element* temp)
{
	// its a line!!! exit
	if (size == 3)
		return false;

	temp=new element;
	if(current==NULL)
	{
		temp->next=NULL;
	}
	else
	{
		temp->next=current;
	}
	temp->value=ele;
	current=temp;
	//printf("%d inserted\n\n",ele);
	size++;
    return false;
}

int stack::pop()
{
	int val = PLATE_NONE;

	if(isEmpty())
	{
		//cout<<"\nStack is Empty\n";      
		return PLATE_NONE;
	}
	else
	{
		//cout<<"\n Element To POP :"<<current->value;
		val = current->value;
		//cout<<"\n Before POP";
		//printStackElements(current);
		current=current->next;
		//cout<<"\n After POP";
		//printStackElements(current);
		size=size--;
	}
	return val;
}

bool stack::isEmpty()
{
	if(getStackSize()==0)
	return true;
	
	return false;
}

int stack::getStackSize()
{
return size;
}//returns size of the stack

void stack::printStackSize()
{
	//cout<<"\nThe Size of the Stack:"<<size<<"\n";
}//print the stack size

void stack::printStackElements(element* base)
{
     	element* curr2;
		curr2= base;
		//cout<<"\n-----\n";
		//cout<<"STACK\n";
		//cout<<"-----\n";
		while(curr2!=NULL)
		{
			//cout<<" |"<<curr2->value<<"|\n";
			curr2=curr2->next;
		}
}// print the stack
