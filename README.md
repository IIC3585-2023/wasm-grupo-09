# wasm-grupo-09

`npm install`

`npm start`

Open http://localhost:3000

To compile C code with emscripten use:

`emcc lib/main.c -s WASM=1 -o public/main.js -s INITIAL_MEMORY=64MB -s EXPORTED_FUNCTIONS="['_solution', '_malloc', '_free']" -s EXPORTED_RUNTIME_METHODS=ccall`
