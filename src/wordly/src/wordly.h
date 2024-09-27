#ifndef WORDLY_H_
#define WORDLY_H_

#include <string>
#include <vector>
#include <optional>

class Wordly {
 public:
  // words -- allowed words  
  // initialWord -- index of words in ‘words‘, which used for first game  
  Wordly(const std::vector<std::string>& words, std::size_t initialWord);

  // Start new game with new word  
  // word is index in original ‘words vector‘  
  void ResetWord(std::size_t word);

  // Length of word for current game  
  std::size_t WordLength() const;

  // return nullopt if word has wrong length or vocabulary does not contain this word  
  // return string of -/+/X characters with same length as ‘word‘ else  
  std::optional<std::string> Ask(const std::string& word) const;

 private:
  std::vector<std::string> words;
  std::size_t currentWordIndex;
};

#endif  // WORDLY_H_