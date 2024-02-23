#ifndef RANDOM_H_
#define RANDOM_H_

uint state;

uint murmur_hash11(uint src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M;
  src ^= src >> 24u;
  src *= M;
  h *= M;
  h ^= src;
  h ^= h >> 13u;
  h *= M;
  h ^= h >> 15u;
  return h;
}

uint murmur_hash13(uvec3 src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M;
  src ^= src >> 24u;
  src *= M;
  h *= M;
  h ^= src.x;
  h *= M;
  h ^= src.y;
  h *= M;
  h ^= src.z;
  h ^= h >> 13u;
  h *= M;
  h ^= h >> 15u;
  return h;
}

float hash11(uint src) {
  uint h = murmur_hash11(src);
  return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

void init_random(uvec3 seed) {
  state = murmur_hash13(seed);
}

float random() {
  float value = hash11(state);
  state = murmur_hash11(state);
  return value;
}

#endif
