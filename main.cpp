#include "windows.h"
#include "winbase.h"	
#include <iostream>
#include <iomanip>

void stack_search(int v,int newv){
std::cout << *((int*)0x28ff18) << std::endl;
	int ebp,esp;
	 __asm__ volatile (
		  "mov %%fs:0x8, %0 \n"
		  "mov %%fs:0x4, %1 \n"
          : "=r" (esp), "=r" (ebp));

	std::cout <<sizeof(int)<<std::endl;
	std::cout << std::hex << esp << " " << ebp << std::endl;;
//	for(int i = esp; i <=ebp; i+=1){
	//	if(i % 8 == 0) std::cout << std::endl;
	//	std::cout << std::hex << (short)*((unsigned char*)i) <<"\t";
	//}	  
	for(int i = esp; i <=ebp; i+=sizeof(int)){
		//if(i % 8 == 0) std::cout << std::endl;
		//std::cout << std::dec << *((int*)i) <<"\t";
		//if( i == 0x28ff18) std::cout <<std::dec <<*((int*)i) <<" " <<v<< " LOL\n";
		if(v == *((int*)i) && &v != ((int*)i)){
			*((int*)i) = newv;
			std::cout << "STACK FOUND!" << *((int*)i) << std::hex << ((int*)i)<< std::endl;
		//	break;
		}
	}	  
}
void heap_search(int v,int newv){
	PROCESS_HEAP_ENTRY phe;
	PHANDLE aHeaps;
	int NumberOfHeaps = GetProcessHeaps(0, NULL);
	aHeaps = new HANDLE[NumberOfHeaps];
	GetProcessHeaps(NumberOfHeaps, aHeaps);
	std::cout <<"\n"<< NumberOfHeaps << std::endl; 	
	for(int i = 1; i < NumberOfHeaps; ++i){
		phe.lpData = NULL;
		std::cout << "HEAP #" << i << std::endl;
		while(HeapWalk(aHeaps[i],&phe)){
			if((PROCESS_HEAP_ENTRY_BUSY & phe.wFlags)  ) {
				for(int j = (int)phe.lpData; 
				j < (int)((int*)phe.lpData + phe.cbData);
				j += sizeof(int)){
				//	std::cout << std::hex << j << std::endl;
				//	std::cout << std::hex << phe.wFlags << std::endl;
					if(v == *((int*)j)){
						*((int*)j) = newv;
						std::cout << "Heap FOUND!" << std::endl;
						goto _exit;
						break;
					}
				}
			}
		}
	}
	_exit:
	delete[] aHeaps;
}

int main(){
	int a;
	int stack = 5234586;
	int *heap = new int(2347567);

	stack_search(5234586,1);
	heap_search(2347567,2);
	//std::cout << hHeap << std::endl;
	///std::cout << std::hex << esp <<" " << ebp << " " << ds << std::endl;
	std::cout << std::hex << &stack << " " << heap << " |"<< sizeof(stack) << std::endl;
	std::cout << std::dec;	
	std::cout << stack << " " << *heap;
	//std::cin >> a;
	//std::cout << stack << " " << *heap;
	return 0;
}