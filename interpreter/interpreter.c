// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

char HEAP[MAX_HEAP_SPACE];
char CODE[2048];

// Global variable that indicates if the process is running.
static bool is_running = true;


void halt(void){
    is_running = false;
}

void load(struct VMContext * ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);    
    ctx->r[a].value = HEAP[ctx->r[b].value];
}

void store(struct VMContext * ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    HEAP[ctx->r[a].value] = ctx ->r[b].value;
    
}

void move(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    ctx -> r[a].value = ctx -> r[b].value;
}

void puti(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint8_t imm = EXTRACT_B2(instr);
    ctx -> r[a].value = imm;    
}

void add(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value + ctx->r[c].value;
}

void sub(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value - ctx->r[c].value;
}

void gt(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value > ctx->r[c].value ? 1 : 0;
}

void ge (struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value >= ctx->r[c].value ? 1 : 0;
}

void eq(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value == ctx->r[c].value ? 1 : 0;
}

void ite(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint8_t imm1 = EXTRACT_B2(instr);
    const uint8_t imm2 = EXTRACT_B3(instr);
    ctx->pc = (&CODE);
    ctx->pc = ctx->r[a].value > 0 ? ctx->pc + (imm1) : ctx->pc + (imm2);    
}

void jump(struct VMContext * ctx, const uint8_t instr){
    // imm1 is an offset of the base address to jump.
    const uint8_t imm1 = EXTRACT_B1(instr);
    ctx->pc = (&CODE);
    ctx->pc = ctx->pc + (imm1);    
}


void put(struct VMContext* ctx, const uint32_t instr) {

    uint32_t a = EXTRACT_B1(instr);        
    uint32_t i = 0;
    char character;
      // Iterate until meets a NULL character.

    do{

        character = HEAP[ (ctx->r[a].value) + i ];
        putchar(character);
        i++;

    }while( character != 0X00);
      

}

 

void get(struct VMContext* ctx, const uint32_t instr) {
    uint32_t a = EXTRACT_B1(instr);            
    uint32_t i = 0; 
    char inputchar;

    while (  (inputchar=fgetc(stdin)) != 0x0A) {        
        HEAP[ (ctx->r[a].value) + i ] = inputchar;
        i++;             
    }
}



void usageExit() {
    // TODO: show usage
    printf("Usage : ./interpreter FILE\n");
    exit(1);
}




void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
    f[0x00] = halt;
    f[0x10] = load;
    f[0x20] = store;
    f[0x30] = move;
    f[0x40] = puti;
    f[0x50] = add;
    f[0x60] = sub;
    f[0x70] = gt;
    f[0x80] = ge;
    f[0x90] = eq;
    f[0xa0] = ite;
    f[0xb0] = jump;

    // puts and gets functions are already in the C library,so I changed the name of functions below.
    f[0xc0] = put;
    f[0xd0] = get;

}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}





int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];    
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;    
    uint32_t* pc;    
    


    // There should be at least one argument.
    if (argc < 2) usageExit();
    
    // Initialize registers.
    initRegs(r, NUM_REGS);
    
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);


    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }    
    
    // Initialize VM context.
    fread((void *)&CODE, 1, 1024, bytecode);   
    pc = (uint32_t*) &CODE;

    // Add pc value to track the instruction pointer.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f,pc);   


    while (is_running) {    
        // TODO: Read 4-byte bytecode, and set the pc accordingly                
        stepVMContext(&vm, &vm.pc);       
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}

