#include <bitset>
#include <cstring>
#include <iostream>
#include <string>
#include <cstdint>

class FloatBits {
 public:
  explicit FloatBits(float value) : value_(value) {
    std::memcpy(&raw_bits_, &value_, sizeof(float));
  }

  float GetValue() const { return value_; }

  uint32_t GetRawBits() const { return raw_bits_; }

  std::bitset<32> GetBitset() const { return std::bitset<32>(raw_bits_); }

  uint32_t GetSign() const { return (raw_bits_ >> 31) & 0x1; }

  uint32_t GetExponent() const { return (raw_bits_ >> 23) & 0xFF; }

  uint32_t GetFraction() const { return raw_bits_ & 0x7FFFFF; }

  int GetUnbiasedExponent() const { return static_cast<int>(GetExponent()) - 127; }

  bool IsExactPowerOfTwo() const {
    return GetExponent() != 0 && GetFraction() == 0;
  }

  std::string FormatBits() const {
    std::bitset<32> bits(raw_bits_);
    std::string bit_string = bits.to_string();

    return bit_string.substr(0, 1) + " " +
           bit_string.substr(1, 8) + " " +
           bit_string.substr(9, 23);
  }

 private:
  float value_;
  uint32_t raw_bits_;
};

void PrintUsage(const std::string& program_name) {
  std::cout << "usage:\n";
  std::cout << "\t" << program_name << " loop_bound loop_counter\n\n";
  std::cout << "\tloop_bound is a positive floating-point value\n";
  std::cout << "\tloop_counter is a positive floating-point value\n";
}

float BuildFloatFromBits(uint32_t bits) {
  float result;
  std::memcpy(&result, &bits, sizeof(float));
  return result;
}

float ComputeOverflowThreshold(float loop_counter) {
  FloatBits counter_bits(loop_counter);

  uint32_t exponent = counter_bits.GetExponent();
  uint32_t fraction = counter_bits.GetFraction();

  // For normalized positive values:
  // - if loop_counter is an exact power of two, threshold exponent = exponent + 24
  // - otherwise threshold exponent = exponent + 25
  //
  // Threshold is itself a power of two, so fraction = 0 and sign = 0.
  uint32_t threshold_exponent;
  if (fraction == 0) {
    threshold_exponent = exponent + 24;
  } else {
    threshold_exponent = exponent + 25;
  }

  uint32_t threshold_bits = (threshold_exponent << 23);
  return BuildFloatFromBits(threshold_bits);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    PrintUsage(argv[0]);
    return 1;
  }

  float loop_bound = std::stof(argv[1]);
  float loop_counter = std::stof(argv[2]);

  FloatBits bound_bits(loop_bound);
  FloatBits counter_bits(loop_counter);

  std::cout << "Loop bound:   " << bound_bits.FormatBits() << "\n";
  std::cout << "Loop counter: " << counter_bits.FormatBits() << "\n\n";

  float threshold = ComputeOverflowThreshold(loop_counter);
  FloatBits threshold_bits(threshold);

  if (loop_bound >= threshold) {
    std::cout << "Warning: Possible overflow!\n";
    std::cout << "Overflow threshold:\n";
    std::cout << threshold << "\n";
    std::cout << threshold_bits.FormatBits() << "\n";
  } else {
    std::cout << "There is no overflow!\n";
  }

  return 0;
}