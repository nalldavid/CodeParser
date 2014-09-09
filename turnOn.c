#include <switchFab.h>
#include <stdio.h>
#include <stdbool.h>

int main(){
	if(!sF_Switch()){
	printf("Switch is off. Turning on.\n");
	sF_turnOn();
	}
	/*char c[14]={'t','h','i','s',' ','i','s',' ','a',' ','l','i','n','e'};
        int i;
	for(i=0;i<10;i++){
		sF_write(c,14);
	}*/
	return 0;
}
