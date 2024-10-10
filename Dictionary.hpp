#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <vector>
#include <string>
#include <set>

class Question {
protected:
    int number;  
    std::string questionText;  

public:
    Question(int num, const std::string& text);
    virtual ~Question() = default;

    virtual void viewQuestion() const = 0; 
    virtual bool checkAnswer(const std::string& userAnswer) const = 0;  

    int getNumber() const;
    std::string getQuestionText() const;
};

class SingleAnswerQuestion : public Question {
    std::string optionA, optionB, optionC;
    char correctAnswer;

public:
    SingleAnswerQuestion(int num, const std::string& text, const std::string& a, const std::string& b, const std::string& c, char correct);

    void viewQuestion() const override;
    bool checkAnswer(const std::string& userAnswer) const override;
    char getCorrectAnswer() const;
    std::string getAnswerA() const{return optionA;};
    std::string getAnswerB() const{return optionB;};
    std::string getAnswerC() const{return optionC;};
};

class MultipleChoiceQuestion : public Question {
    std::string answerA, answerB, answerC;
    std::set<char> correctAnswers;  

public:
    MultipleChoiceQuestion(int num, const std::string& text, const std::string& a, const std::string& b, const std::string& c, const std::set<char>& correct);

    void viewQuestion() const override;
    bool checkAnswer(const std::string& userAnswer) const override;
    std::string getAnswerA() const;
    std::string getAnswerB() const;
    std::string getAnswerC() const;
    std::set<char> getCorrectAnswers() const;
};

class Dictionary {
private:
    std::vector<Question*> questions; 
    const std::string fileName = "quest.txt";

public:
    ~Dictionary();

    void addSingleAnswerQuestion(int number, const std::string& questionText, const std::string& a, const std::string& b, const std::string& c, char correctAnswer);
    void addMultipleChoiceQuestion(int number, const std::string& questionText, const std::string& a, const std::string& b, const std::string& c, const std::set<char>& correctAnswers);
    void deleteQuestion(int number);
    void viewQuestions() const;
    void takeTest() const;
    void saveToFile() const;
    void loadFromFile();
};

#endif // DICTIONARY_H
