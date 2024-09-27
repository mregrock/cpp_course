#include "player.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

Player::Player(const std::vector<std::string>& words)
    : words(words), gameStarted(false) {}

void Player::Start(std::size_t wordLength) {
  possibleWords.clear();
  for (const auto& word : words) {
    if (word.size() == wordLength) {
      possibleWords.emplace_back(word);
    }
  }
  gameStarted = true;
}

std::optional<std::string> Player::Ask() {
  if (!gameStarted || possibleWords.empty()) {
    return std::nullopt;
  }

  return possibleWords.front();
}

void Player::ConsumeAnswer(const std::string& guess, const std::string& result) {
  if (!gameStarted) {
    return;
  }
  FilterPossibleWords(guess, result);
}

void Player::FilterPossibleWords(const std::string& guess, const std::string& result) {
  std::vector<std::string> filtered;
  for (const auto& word : possibleWords) {
    bool match = true;
    std::unordered_map<char, int> letterCounts;

    for (char c : word) {
      letterCounts[std::tolower(c)]++;
    }

    for (std::size_t i = 0; i < guess.size(); ++i) {
      char gChar = std::tolower(guess[i]);
      char wChar = std::tolower(word[i]);
      if (result[i] == '+') {
        if (gChar != wChar) {
          match = false;
          break;
        }
        letterCounts[gChar]--;
      }
    }

    if (!match) {
      continue;
    }

    for (std::size_t i = 0; i < guess.size(); ++i) {
      if (result[i] == '-') {
        if (word.find(std::tolower(guess[i])) != std::string::npos) {
          match = false;
          break;
        }
      } else if (result[i] == 'X') {
        char gChar = std::tolower(guess[i]);
        if (word.find(gChar) == std::string::npos) {
          match = false;
          break;
        }
        if (letterCounts.find(gChar) == letterCounts.end() || letterCounts[gChar] <= 0) {
          match = false;
          break;
        }
        letterCounts[gChar]--;
      }
    }

    if (match) {
      filtered.emplace_back(word);
    }
  }
  possibleWords = std::move(filtered);
}