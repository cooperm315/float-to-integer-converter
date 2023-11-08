// main driver function to convert floats to integers
// returns the integer value of the float input 
int32_t f2i(uint32_t in);
// helper function to get the stored exonent value
// returns stored exponent
static uint32_t get_exp_s(uint32_t input);
// helper function to get the actual exponent value
// returns actual exponent
static int32_t get_exp_a(uint32_t input);
// helper function to get the the significand isolated
// returns isolated significand
static uint32_t get_sig(uint32_t input);
// helper function to shift the significand
// returns the shifted significand 
static int32_t shift_sig(uint32_t input);
// helper function to check errors in the bit code 
// returns the input if valid otherwise returns proper error value
static int32_t check_val(uint32_t input);
// checks if the value is negative 
// returns true if it is negative
static bool check_neg(uint32_t input);