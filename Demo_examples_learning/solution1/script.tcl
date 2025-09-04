############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project Demo_examples_learning
set_top loop_imperfect
add_files Demo_examples_learning/Pipelining/loop_imperfect.cpp
add_files Demo_examples_learning/Pipelining/loop_imperfect.hpp
add_files loop_perfect.cpp
add_files loop_pipeline.cpp
add_files loop_pipeline.hpp
add_files Demo_examples_learning/Pipelining/pipeline.cpp
add_files Demo_examples_learning/Pipelining/pipeline.hpp
add_files -tb loop_imperfect_test.cpp -cflags "-Wno-unknown-pragmas"
add_files -tb loop_pipeline_test.cpp -cflags "-Wno-unknown-pragmas"
add_files -tb Demo_examples_learning/Pipelining/pipeline.tb -cflags "-Wno-unknown-pragmas"
add_files -tb Demo_examples_learning/Pipelining/pipeline_tb.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}
create_clock -period 10 -name default
#source "./Demo_examples_learning/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
