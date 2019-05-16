// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	if (this->words.contains(word)){
		return true;
	}
    return false;
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> suggest;

	std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//swap
	for(unsigned int i = 0; i < word.length(); i++){ // might have to be length - 1 bc i+1
		std::string copy = word;
		std::swap(copy[i], copy[i+1]);
		if(wordExists(copy) && std::find(suggest.begin(),suggest.end(),copy) == suggest.end() ){
			suggest.push_back(copy);
		}
	}

	//insert
	for(unsigned int i = 0; i < word.length(); i++){
		for(unsigned int b = 0; b < letters.length(); b++){
			std::string copy = word;
			copy.insert(i,letters.substr(b,1));
			if(wordExists(copy) && std::find(suggest.begin(),suggest.end(),copy) == suggest.end() ){
				suggest.push_back(copy);
			}
		}
	}

	//delete
	for(unsigned int i = 0; i < word.length(); i++){
		std::string copy = word;
		copy.erase(i,1);
		if(wordExists(copy) && std::find(suggest.begin(),suggest.end(),copy) == suggest.end() ){
			suggest.push_back(copy);
		}
	}

	//replace
	for(unsigned int i = 0; i < word.length(); i++){
		std::string copy = word;
		for(unsigned int b = 0; b < letters.length(); b++){
			copy[i] = letters[b];
			if( wordExists(copy) && std::find(suggest.begin(),suggest.end(),copy) == suggest.end() ){
				suggest.push_back(copy);
			}
		}
	}

	//split
	for(unsigned int i = 0; i < word.length(); i++){
		std::string copy = word;
		std::string first = copy.substr(i,word.length()-i);
		std::string second = copy.substr(0,i);
		if(wordExists(first) && wordExists(second)){
			copy.insert(i," ");
			if(std::find(suggest.begin(),suggest.end(),copy) == suggest.end()){
				suggest.push_back(copy);
			}
		}
	}

	return suggest;

    // return std::vector<std::string>{};
}

