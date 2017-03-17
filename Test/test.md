# File Description
test.mini file computes a GCD value of two input digits.
To have a complicate operation, I modified compiler a bit to add my own function.

# gcd opcode description
	gcd (reg) (reg) (reg)

"gcd" opcode computes a gcd value of memory values located at R2 and R3.
a gcd value will be stored in the memory value located at R1.

 - Input  : Two digits. Enter a digit one by one due to type-check limits.
 - Output : A GCD value of two digits.

# Pseudocode

#include <stdio.h>
#include <stdlib.h>
void main(void){

	unsigned int R1,R2,R3;
	
	printf("Enter two positive integers (1-9)");
	
	printf("First : ");
	scanf("%d",&R1);

	printf("Second : ");
	scanf("%d",&R2);

	R3 = gcd(R1,R2);
	printf("Answer is %d\n",R3); 
}


// This function is called by opcode
//////////////////////////////////////////////////////////
void gcd(struct VMContext* ctx, const uint32_t instr) {
    const uint32_t a = EXTRACT_B1(instr);                
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);          
    unsigned int R2 = HEAP[(ctx->r[b].value)] - 48;
    unsigned int R3 = HEAP[(ctx->r[c].value)] - 48;
    unsigned int gcd = 1;
    // If any of them is a zero value, do not compute gcd.
    if( (R2 | R3) == 0){
        printf("Invalid value for gcd\n");
        exit(1);
    }
    //Compute GCD
    for(unsigned int i=2; (i <= R2 ) && (i<= R3) ; i++)
    {        
        if(R2%i==0 && R3%i==0)
            gcd = i;
    }      
    // Store a gcd value to the memory in the R1.
    HEAP[(ctx->r[a].value)] = gcd + 48;
}
//////////////////////////////////////////////////////////////

