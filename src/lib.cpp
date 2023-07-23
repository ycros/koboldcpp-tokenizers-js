#include "extracted.h"

#include <emscripten/bind.h>
#include <emscripten/val.h>

// TODO: I don't think we actually need to be using enbind here, I tried
// wrapping the structs with it but it ended up easier to just use simpler types
// at the interface so all it's adding is code bloat

using namespace emscripten;

static llama_vocab vocab;

int load_vocab(const std::string &text) {
  const size_t size = text.size();
  const char *buffer = text.c_str();
  const char *ptr = buffer;

  // printf("Loading vocab of size %zu\n", size);

  // read the number of tokens
  uint32_t num_tokens = *(uint32_t *)ptr;
  ptr += sizeof(uint32_t);

  // reset the vocab
  vocab.id_to_token.clear();
  vocab.token_to_id.clear();

  // decode the buffer
  for (uint32_t i = 0; i < num_tokens; ++i) {
    // read the score
    float score = *(float *)ptr;
    ptr += sizeof(float);

    // read the length of the token string
    uint32_t tok_len = *(uint32_t *)ptr;
    ptr += sizeof(uint32_t);

    // ensure we won't exceed the buffer size
    if (ptr - buffer + tok_len > size) {
      return -1;
    }

    // read the token string
    std::string tok(ptr, tok_len);
    ptr += tok_len;

    // repopulate the vocab
    vocab.id_to_token.push_back({tok, score});
    vocab.token_to_id[tok] = i;
  }
  return 0;
}

val tokenize_wrapper(const std::string &text, bool bos) {
  auto result = llama_tokenize(vocab, text, bos);
  return val::array(result);
}

EMSCRIPTEN_BINDINGS(llama) {
  function("load_vocab", &load_vocab);
  function("llama_tokenize", &tokenize_wrapper);
}
