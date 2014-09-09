#include <switchFab.h>
#include <stdio.h>
#include <stdbool.h>

int main(){
	if(sF_Switch()){
	printf("Switch is on. Turning off.\n");
	sF_turnOff();
	}
	return 0;
}
