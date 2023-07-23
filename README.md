# koboldcpp-tokenizers-js

Proof of concept of loading up koboldcpp/llama.cpp tokenizers in the browser using emscripten, and fetching the vocab from the server.

Currently uses `bin/extract_tokenizers.py` to extract the tokenizer implementations out of the koboldcpp codebase. This may not be necessary, as apparently emscripten's dead code elimination is pretty good, so it may be possible to do this in-tree.

Current output sizes end up at:
- 31K kcpp_tokenizers.js
- 23K kcpp_tokenizers.wasm

The js will probably drop to single digits if you can get closure working properly in the build, but I think it's possible to eliminate the js entirely and just use the raw wasm.

Also, the wasm could be further reduced by pulling out the current use of -lembind.

Various TODOs are sprinkled throughout.

## Building

Needs cmake and emscripten: https://emscripten.org/docs/getting_started/downloads.html

Then:

```
mkdir build
cd build
emcmake cmake ..
cmake --build .
```

_(note, `emcmake` is emscripten's wrapper for cmake, it'll configure cmake to build with the emscripten toolchain)_

You should then end up with `kcpp_tokenizers.js` and `kcpp_tokenizers.wasm` in the `build/` dir. See `index.html` on how to use it.
