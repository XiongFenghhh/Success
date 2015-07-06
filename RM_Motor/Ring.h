#ifndef Ring_h
#define Ring_h

#include <math.h>
template<class ringType,int num=10>
class Ring
{
public:
//构造函数
	Ring():currentNum(0),lastEntry(-1){};

//元素添加操作
	void addItem(ringType item);
//元素求和操作
	ringType sum();
//返回最后插入的一个元素
	ringType getLast();

	int getCurrentNum(){return currentNum;};
	int getLastEntry(){return lastEntry;};
private:
ringType items[num];
int currentNum;
int lastEntry;
};
template<class ringType,int num>
void Ring<ringType,num>::addItem(ringType item){
lastEntry=(lastEntry+1)%num;
items[lastEntry]=item;
if(currentNum<num)currentNum++;
}

template<class ringType,int num>
ringType Ring<ringType,num>::sum(){
ringType sum;
for(int i=0;i<num;i++,sum+=items[i]);
return sum;
}

template<class ringType,int num>
ringType Ring<ringType,num>::getLast(){
return items[lastEntry];
}
#endif
