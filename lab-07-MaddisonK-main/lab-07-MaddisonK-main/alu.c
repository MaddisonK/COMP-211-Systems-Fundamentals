// PID: 730322550
// I pledge the COMP211 honor code.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "alu.h"

// ----------------------------------------
// Simulate add and subtract arithmetic digital logic circuit
// 
// ----------------------------------------

void arithmetic_fun_exe( arithmetic* circuit ) {

    if (circuit->Sub == '0') {
        circuit->Y = circuit->A + circuit->B;
    }
    if (circuit->Sub == '1') {
        circuit->Y = circuit->A - circuit->B;
    }

}

int arithmetic_fun_cfg( arithmetic* circuit ) {

    if ( circuit->Sub == 'x' ) {
        circuit->Sub = toupper( circuit->Sub );
    }

    if ( circuit->Sub != '0' && circuit->Sub != '1' && circuit->Sub != 'X' ) {
        return CONFIG_ERROR;
    } else {
        return OK;
    }

}

// ----------------------------------------
// Simulate shifting digital logic circuit
// 
// ----------------------------------------

void shift_fun_exe( shift* circuit ) {

    if (strcmp(circuit->Bool, "00") == 0) {
        circuit->Y = circuit->B << circuit->A;
    }
    if (strcmp(circuit->Bool, "10") == 0) {
        circuit->Y = circuit->B >> circuit->A;
    }
    if (strcmp(circuit->Bool, "11") == 0) {
        circuit->Y = (unsigned int) circuit->B >> circuit->A;
    }

}

int shift_fun_cfg( shift* circuit ) {

    int i=0;
    char temp[3];
    memcpy( temp, circuit->Bool, 3 );

    while ( circuit->Bool[i] != '\0' && circuit->Bool[i] == 'x' ) {
        temp[i] = toupper( circuit->Bool[i] );
        i++;
    }

    memcpy( circuit->Bool, temp, 3);

    if ( strcmp( circuit->Bool, "00") != 0 && strcmp(  circuit->Bool, "01") != 0 && strcmp( circuit->Bool, "10") != 0  && strcmp( circuit->Bool, "11") != 0 && strcmp( circuit->Bool, "XX") != 0 ) {
        return CONFIG_ERROR;
    } else {
        return OK;
    }

}

// ----------------------------------------
// Simulate boolean logic digital logic circuit
// 
// ----------------------------------------

void logic_fun_exe( logic* circuit ) {

    if (strcmp( circuit->Bool, "00") == 0) {
        circuit->Y = circuit->A & circuit->B;
    } 
    if (strcmp( circuit->Bool, "01") == 0) {
        circuit->Y = circuit->A | circuit->B;
    }
    if (strcmp( circuit->Bool, "10") == 0) {
        circuit->Y = (circuit->A ^ circuit->B);
    }
    if (strcmp( circuit->Bool, "11") == 0) {
        circuit->Y = ~(circuit->A | circuit->B);
    }

}

int logic_fun_cfg( logic* circuit ) {

    int i = 0;
    char temp[3];
    memcpy( temp, circuit->Bool, 3 );

    while ( circuit->Bool[i] != '\0' && circuit->Bool[i] == 'x' ) {
        temp[i] = toupper( circuit->Bool[i] );
        i++;
    }

    memcpy( circuit->Bool, temp, 3 );

    if ( strcmp( circuit->Bool, "00") != 0 && strcmp(  circuit->Bool, "01") != 0 && strcmp( circuit->Bool, "10") != 0  && strcmp( circuit->Bool, "11") != 0 && strcmp( circuit->Bool, "XX") != 0 ) {
        return CONFIG_ERROR;
    } else {
        return OK;
    }

}

// ----------------------------------------
// Simulate multiplexor (MUX) digital logic circuit
// 
// ----------------------------------------

void mux_fun_exe( mux* circuit ) {

    if (circuit->Selector == '0') {
        circuit->Y = circuit->A;
    }
    if (circuit->Selector == '1') {
        circuit->Y = circuit->B;
    }

}

int mux_fun_cfg( mux* circuit ) {

    if ( circuit->Selector == 'x' ) {
        circuit->Selector = toupper( circuit->Selector );
    }

    if ( circuit->Selector != '0' && circuit->Selector != '1' && circuit->Selector != 'X' ) {
        return CONFIG_ERROR;
    } else {
        return OK;
    }

}

// ----------------------------------------
// Initialize the simulated MUX, shift, logic, and arithmetic
// hardware components.
//
// args:
//			None
//
// return:
//			None
// ----------------------------------------

void initialize_alu() {

    math_mux = (mux*)malloc( sizeof( mux ) );
    shft_mux = (mux*)malloc( sizeof( mux ) );
    shift_circuit = (shift*)malloc( sizeof( shift ) );
    logic_circuit = (logic*)malloc( sizeof( logic ) );
    arithmetic_circuit = (arithmetic*)malloc( sizeof( arithmetic ) );

    math_mux->A = ZERO;
    math_mux->B = ZERO;
    math_mux->Y = ZERO;
    math_mux->Selector = BIT_DEFAULT;
    math_mux->exe = mux_fun_exe;
    math_mux->cfg = mux_fun_cfg;

    shft_mux->A = ZERO;
    shft_mux->B = ZERO;
    shft_mux->Y = ZERO;
    shft_mux->Selector = BIT_DEFAULT;
    shft_mux->exe = mux_fun_exe;
    shft_mux->cfg = mux_fun_cfg;

    shift_circuit->A = ZERO;
    shift_circuit->B = ZERO;
    shift_circuit->Y = ZERO;
    shift_circuit->Bool = (char*)malloc( sizeof(char)*3);
    strcpy( shift_circuit->Bool, "00" );
    shift_circuit->exe = shift_fun_exe;
    shift_circuit->cfg = shift_fun_cfg;

    logic_circuit->A = ZERO;
    logic_circuit->B = ZERO;
    logic_circuit->Y = ZERO;
    logic_circuit->Bool = (char*)malloc( sizeof(char)*3);
    strcpy( logic_circuit->Bool, "00" );
    logic_circuit->exe = logic_fun_exe;
    logic_circuit->cfg = logic_fun_cfg;

    arithmetic_circuit->A = ZERO;
    arithmetic_circuit->B = ZERO;
    arithmetic_circuit->Y = ZERO;
    arithmetic_circuit->Sub = BIT_DEFAULT;
    arithmetic_circuit->exe = arithmetic_fun_exe;
    arithmetic_circuit->cfg = arithmetic_fun_cfg;

} // end initialize_alu() function

// ----------------------------------------
// Configure ALU using ALUFN bits
// In our simulator a configuration bit is represented by 
// an 8-bit ASCII character value (per the 5-bit ALUFN Table)
//
// args:
//			Sub = add or subtract circuit configuration bits
//			Bool = shifter and boolean circult configuration bits
//			Shft = MUX selector bit
//			Math = MUX selector bit
//
// return:
//			CONFIG_ERROR for an unsupported configuration value
//        	PASS when all configuration values are correct
// ----------------------------------------

int configure_alu( char Sub, char* Bool, char Shft, char Math ) {

    arithmetic_circuit->Sub = Sub;
    strcpy( logic_circuit->Bool, Bool );
    strcpy(shift_circuit->Bool, Bool);

    math_mux->Selector = Math;
    shft_mux->Selector = Shft;

    int ret = 0;
    ret += arithmetic_circuit->cfg(arithmetic_circuit);
    ret += shift_circuit->cfg(shift_circuit);
    ret += logic_circuit->cfg(logic_circuit);
    ret += shft_mux->cfg(shft_mux);
    ret += math_mux->cfg(math_mux);
    if (ret != 0) {
        return CONFIG_ERROR;
    }
    return 0;

} // end configure_alu() function

// ----------------------------------------
// Run ALU
//
// args:
//			A = signed 2's complement 32 bit integer value
//			B = signed 2's complement 32 bit integer value
//
// return:
//			Result = signed 2's complement 32 bit integer value
// ----------------------------------------

int run_alu( int A, int B ) {
    arithmetic_circuit->A = A;
    arithmetic_circuit->B = B;
    shift_circuit->A = A;
    shift_circuit->B = B;
    logic_circuit->A = A;
    logic_circuit->B = B;

    logic_circuit->exe(logic_circuit);
    shift_circuit->exe(shift_circuit);
    shft_mux->A = logic_circuit->Y;
    shft_mux->B = shift_circuit->Y;
    
    shft_mux->exe(shft_mux);
    math_mux->A = shft_mux->Y;
    arithmetic_circuit->exe(arithmetic_circuit);
    math_mux->B = arithmetic_circuit->Y;

    math_mux->exe(math_mux);

    return math_mux->Y;

} // end run_alu() function
