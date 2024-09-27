#include "src/wordly.h"
#include "src/player.h"
#include <iostream>
#include <vector>
#include <random>

int main() {
  std::vector<std::string> dictionary = {
      "apple", "apply", "ample", "maple", "papal", "pappy", "lapel", "leapt", "pleat", "petal",
      "plate", "platy", "pearl", "leper", "rebel", "bleep", "plebe", "belly", "belle", "label",
      "table", "cable", "babel", "blame", "camel", "calm", "claim", "clamp", "cramp", "tramp",
      "grape", "grasp", "glass", "grass", "brass", "blast", "clasp", "crash", "trash", "flash",
      "flask"
  };

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, dictionary.size() - 1);
  std::size_t initialWordIndex = dis(gen);

  Wordly wordly(dictionary, initialWordIndex);
  Player player(dictionary);

  std::cout << "Guessing word: " << dictionary[initialWordIndex] << "\n";

  player.Start(wordly.WordLength());

  while (true) {
    auto guessOpt = player.Ask();
    if (!guessOpt.has_value()) {
      std::cout << "No possible words to guess.\n";
      break;
    }
    std::string guess = guessOpt.value();
    std::cout << "Player guesses: " << guess << "\n";

    auto resultOpt = wordly.Ask(guess);
    if (!resultOpt.has_value()) {
      std::cout << "Invalid guess.\n";
      break;
    }
    std::string result = resultOpt.value();
    std::cout << "Result: " << result << "\n";

    player.ConsumeAnswer(guess, result);

    if (result.find('+') != std::string::npos && result.find('-') == std::string::npos && result.find('X') == std::string::npos) {
      std::cout << "Word guessed correctly!\n";
      break;
    }
  }

  return 0;
}