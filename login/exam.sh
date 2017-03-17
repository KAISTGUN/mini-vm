ocaml str.cma  ../compiler/compiler.ml login.mini bytecode
cd ../interpreter/
make
cd ../login/
../interpreter/build/interpreter bytecode
