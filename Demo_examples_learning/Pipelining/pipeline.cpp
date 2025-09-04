#include "pipeline.hpp"

char foo (char incr , char inval){
#pragma HLS INLINE = on
#pragma HLS FUNCTION_INSTANTIATE variable = incr
	return incr + inval;
}


void top(ch inval1,ch inval2,ch inval3,
		ch* outval1,ch* outval2, ch* outval3){
*outval1 = foo(inval1,0);
*outval2 = foo(inval2,1);
*outval3 = foo(inval3,100);

}
