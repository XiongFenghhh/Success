#ifndef Ring_h
#define Ring_h

#include <math.h>
template<class ringType,int num=10>
class Ring
{
public:
//���캯��
	Ring():currentNum(0),lastEntry(-1){};

//Ԫ����Ӳ���
	void addItem(ringType item);
//Ԫ����Ͳ���
	ringType sum();
//�����������һ��Ԫ��
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
