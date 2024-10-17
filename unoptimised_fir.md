# HLS-pynq
this repo gives you a progressive view upon making optimisations on HLS and implementation on the PYNQ-Z2 board

## C simulation result

![image](https://github.com/user-attachments/assets/69ba6f18-bc25-4b99-a807-5e56fce6d6ef)

Execution time (sec) = clock cycles * clocl periood
                     = clock cycles / clock frequency
                     500Mhz PS clock and 250Mhz PL clock in the Pynq-Z2 board

![image](https://github.com/user-attachments/assets/2b8a8eb9-c0c3-49ac-ad87-a455185c966e)

1 function call of fir requires 29 clock cycles and 108ns (unoptimised)
1 whole clock cycle to call a function so interval is Latency+1 that is 30
fir_test.cpp has 600 samples that means 600 calls of that function

600 * 30 = 18000 cycles to complete the function 

C similation for  testbench and RTL simulation for the kernel file (cycle accurate simulation)

### Co-simulation report

![image](https://github.com/user-attachments/assets/b2bae0b7-9a6d-4060-a680-75e8286ebdfb)

its different than that of the synthesis report 
Max and Avg latncy being 27 and Min II being 28 

600 * 28 = 16,800

### START (ap_start=1)

![image](https://github.com/user-attachments/assets/654516d1-ebdf-484a-adbf-76446e6abda4)


### END (ap_done=0)

![image](https://github.com/user-attachments/assets/559e77f0-c569-498c-9db2-ccf978413b74)


600 calls of th fir function ap_done is 600

see timing of starting and ending 67314-114 ns = 67.2us (which is 16,800 cycles)

