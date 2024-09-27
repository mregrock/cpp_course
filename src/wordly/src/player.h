#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>
#include <optional>

class Player {
 public:

  Player(const std::vector<std::string>& words);

  void Start(std::size_t wordLength);

  std::optional<std::string> Ask();

  void ConsumeAnswer(const std::string& word, const std::string& result);

 private:
  std::vector<std::string> words;
  std::vector<std::string> possibleWords;
  bool gameStarted;

  void FilterPossibleWords(const std::string& guess, const std::string& result);
};

#endif  // PLAYER_H_