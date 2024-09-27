#include "wordly.h"
#include <cctype>
#include <unordered_map>

Wordly::Wordly(const std::vector<std::string>& words, std::size_t initialWord)
    : words(words), currentWordIndex(initialWord) {}

void Wordly::ResetWord(std::size_t word) {
  if (word < words.size()) {
    currentWordIndex = word;
  }
}

std::size_t Wordly::WordLength() const {
  if (currentWordIndex < words.size()) {
    return words[currentWordIndex].size();
  }
  return 0;
}

std::optional<std::string> Wordly::Ask(const std::string& guess) const {
  if (currentWordIndex >= words.size()) {
    return std::nullopt;
  }

  const std::string& target = words[currentWordIndex];
  if (guess.size() != target.size()) {
    return std::nullopt;
  }

  std::string result(guess.size(), '-');
  std::unordered_map<char, int> targetCounts;

  for (char c : target) {
    targetCounts[std::tolower(c)]++;
  }

  for (std::size_t i = 0; i < guess.size(); ++i) {
    if (std::tolower(guess[i]) == std::tolower(target[i])) {
      result[i] = '+';
      targetCounts[std::tolower(target[i])]--;
    }
  }

  for (std::size_t i = 0; i < guess.size(); ++i) {
    if (result[i] == '-') {
      char lowerChar = std::tolower(guess[i]);
      if (targetCounts.find(lowerChar) != targetCounts.end() && targetCounts[lowerChar] > 0) {
        result[i] = 'X';
        targetCounts[lowerChar]--;
      }
    }
  }

  return result;
}